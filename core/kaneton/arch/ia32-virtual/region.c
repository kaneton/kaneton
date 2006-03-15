/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [wed mar 15 18:22:11 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for region  manager on  ia32 with
 * paging architecture.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students will have to develop this entire part of the project. take
 * a look at the interface  structure declaration to fill it correctly
 * and to write needed functions.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_region*	region;
extern t_asid		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager interface.
 */

i_region		region_interface =
  {

/*                                                                  [cut] k2 */

    NULL,
    NULL,
    ia32_region_reserve,
    ia32_region_release,
    NULL,
    ia32_region_init,
    ia32_region_clean

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function map a page table into the kernel.
 *
 * steps:
 *
 *  1) add the new table.
 *  2) now we map the table in the kernel, first check if there is a pt
 *  3) reserve a segment for the new kernel pt
 *  4) add the table
 *  5) inject the corresponding region
 *  6) finally, via the mirror entry, map the pt
 */

t_error		ia32_region_map_table(void*		v,
				      t_uint16		pde,
				      t_directory	kpd,
				      void*		address)
{
  o_as*			o = v;
  t_page		pg;
  t_table		pt;
  o_region		optreg;
  o_segment*		otbl;
  t_segid		segtbl;

  REGION_ENTER(region);

  /*
   * 1)
   */

  pt.rw = 1;
  pt.present = 1;
  pt.user = 0;
  pt.entries = address;

  pg.rw = 1;
  pg.present = 1;
  pg.user = 0;

  if (pd_add_table(&o->machdep.pd, pde, pt) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  pg.addr = (void*)pt.entries;

  if (pd_get_table(&kpd, PDE_ENTRY((t_paddr)pg.addr), &pt) !=
      ERROR_NONE)
    {
      /*
       * 2)
       */

      if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			  &segtbl) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (segment_get(segtbl, &otbl) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.entries = (void*)otbl->address;

      /*
       * 3)
       */

      if (pd_add_table(&kpd, PDE_ENTRY((t_paddr)pg.addr), pt) !=
	  ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      optreg.address = (t_paddr)pg.addr;
      optreg.segid = (t_segid)optreg.address;
      optreg.offset = 0;
      optreg.size = PAGESZ;

      /*
       * 4)
       */

      if (region_inject(kasid, &optreg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY((t_paddr)pg.addr));

  if (pt_add_page(&pt, PTE_ENTRY((t_paddr)pg.addr), pg) !=
      ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 6)
   */

  tlb_invalidate((t_vaddr)pg.addr);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * reserves a region.
 *
 * steps:
 *
 * 1) gets the as object and the kernel page directory.
 * 2) gets the task object (used to check privilege level).
 * 3) fills the t_page structure.
 * 4) gets the segment object.
 * 5) loops throught the virtual memory to map.
 * 6) checks if we changed of page table.
 *  a) checks if the table already exists.
 *  b) reserves a segment for a new table.
 * 7) adds the page.
 */

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    t_regid*		regid)
{
  o_as*			o;
  o_segment*		oseg;
  o_segment*		otbl;
  o_task*		otsk;
  t_vaddr		vaddr;
  t_paddr		paddr;
  t_uint16		pde = (t_uint16)-1;
  t_table		pt;
  t_page		pg;
  t_segid		segtbl;
  t_directory		kpd;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(kasid, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  kpd = o->machdep.pd;

  if (asid != kasid)
    if (as_get(asid, &o) != ERROR_NONE)
      REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(o->tskid, &otsk) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  pg.rw = 1;
  pg.present = 1;
  pg.user = (otsk->class == TASK_CLASS_PROGRAM);

  /*
   * 4)
   */

  if (segment_get(segid, &oseg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 5)
   */

  for (vaddr = address, paddr = oseg->address + offset;
       vaddr < address + size;
       vaddr += PAGESZ, paddr += PAGESZ)
    {

      /*
       * 6)
       */

      if (PDE_ENTRY(vaddr) != pde)
	{
	  if (!(opts & REGION_OPT_PERSISTENT) && pde != (t_uint16)-1)
	    {
/*	      if (ia32_region_unmap_table(o, pde, kpd, pt.entries) !=
		  ERROR_NONE)
		REGION_LEAVE(region, ERROR_UNKNOWN);
	*/    }

	  pde = PDE_ENTRY(vaddr);

	  /*
	   * a)
	   */

	  if (pd_get_table(&o->machdep.pd, pde, &pt) != ERROR_NONE)
	    {
	      pt.rw = 1;
	      pt.present = 1;
	      pt.user = 0;

	      /*
	       * b)
	       */

	      if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE,
				  &segtbl) != ERROR_NONE)
		REGION_LEAVE(region, ERROR_UNKNOWN);

	      if (segment_get(segtbl, &otbl) != ERROR_NONE)
		REGION_LEAVE(region, ERROR_UNKNOWN);

	      /*
	       * c)
	       */


	      if (ia32_region_map_table(o, pde, kpd, (void*)otbl->address) !=
		  ERROR_NONE)
		REGION_LEAVE(region, ERROR_UNKNOWN);
	    }

	  pd_get_table(&o->machdep.pd, pde, &pt);
	}

      /*
       * 7)
       */

      pg.addr = (void*)paddr;

      if (pt_add_page(&pt, PTE_ENTRY(vaddr), pg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function  releases a region.  we unmap the region  and release
 * page tables.
 *
 */

t_error			ia32_region_release(t_asid		asid,
					    t_regid		regid)
{
  o_as*			o;
  o_region*		oreg;
  t_table		pt;
  t_uint32		pde_start;
  t_uint32		pde_end;
  t_uint32		pte_start;
  t_uint32		pte_end;
  t_uint32		pde;
  t_uint32		pte;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    {
      cons_msg('!', "region: unable to get as object\n");
      REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  if (region_get(asid, regid, &oreg) != ERROR_NONE)
    {
      cons_msg('!', "region: unable to get region object\n");
      REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  pde_start = PDE_ENTRY(oreg->address);
  pte_start = PTE_ENTRY(oreg->address);
  pde_end = PDE_ENTRY(oreg->address + oreg->size);
  pte_end = PTE_ENTRY(oreg->address + oreg->size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      if (pd_get_table(&o->machdep.pd, pde, &pt) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
	   pte++)
	{
	  if (pt_delete_page(&pt, pte) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      if (pde != pde_start && pde != pde_end)
	{
	  /* release pt */
	}
    }

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function just initialises the machine-dependent region manager.
 *
 * for the intel 32-bit architecture we just initialise the paging mode.
 */

t_error			ia32_region_init(t_fit			fit,
					 t_vaddr		start,
					 t_vsize		size)
{
  REGION_ENTER(region);

  if (paging_init() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent region manager.
 */

t_error			ia32_region_clean(void)
{
  REGION_ENTER(region);

  if (paging_clean() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
