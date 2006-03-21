/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [tue mar 21 12:21:39 2006]
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
 * this function directly maps a chunk of memory.
 *
 * steps:
 *
 * XXX
 */

static t_error		ia32_region_map_chunk(o_as*		o,
					      t_vaddr		v,
					      t_paddr		p)
{
  o_region		oreg;
  t_table		pt;
  t_page		pg;
/*  t_segid		seg;
  t_vaddr		chunk;
*/

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (pd_get_table(NULL, PDE_ENTRY(v), &pt) != ERROR_NONE)
    {
/*      if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			  &seg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.rw = 1;
      pt.present = 1;
      pt.user = 0;

      if (region_fit(o, PAGESZ, &chunk) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.entries = chunk;
*/
      printf("RETENTE TA CHANCE !\n");
      while (1);
    }

  /*
   * 2)
   */

  pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(v));

  pg.rw = 1;
  pg.present = 1;
  pg.user = 0;
  pg.addr = p;

  if (pt_add_page(&pt, PTE_ENTRY(v), pg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  oreg.segid = p;
  oreg.address = v;
  oreg.offset = 0;
  oreg.size = PAGESZ;

  if (region_inject(kasid, &oreg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  tlb_invalidate(v);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * XXX
 */

static t_error		ia32_region_unmap_chunk(o_as*		o,
						t_vaddr		v)
{
  t_table		pt;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (pd_get_table(NULL, PDE_ENTRY(v), &pt) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(v));

  if (pt_delete_page(&pt, PTE_ENTRY(v)) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  o_as* as;

  if (as_get(kasid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_remove(as->regions, v) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  tlb_invalidate(v);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * reserves a region.
 *
 * steps:
 *
 * 1) get the as object and the kernel page directory.
 * 2) get the task object (used to check privilege level).
 * 3) get the segment object.
 * 4) fill the t_page structure.
 * 5) map the as page directory in the kernel.
 * 6) loop throught the virtual memory to map.
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
  o_task*		otsk;
  t_vaddr		vaddr;
  t_paddr		paddr;
  t_directory		pd;
  t_table		pt;
  t_page		pg;
  t_directory		kpd;
  t_uint32		pde_start;
  t_uint32		pde_end;
  t_uint32		pte_start;
  t_uint32		pte_end;
  t_uint32		pde;
  t_uint32		pte;
  t_vaddr		chunk;
  t_segid		ptseg;
  t_paddr		chiche;

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

  if (segment_get(segid, &oseg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  pg.rw = oseg->perms & PERM_WRITE;
  pg.present = 1;
  pg.user = (otsk->class == TASK_CLASS_PROGRAM);

  /*
   * 5)
   */

  if (region_fit(o, PAGESZ, &chunk) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  pd = (t_directory)(t_uint32)chunk;

  if (ia32_region_map_chunk(o, (t_vaddr)pd,
			    (t_paddr)o->machdep.pd) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 6)
   */

  paddr = oseg->address + offset;
  vaddr = address + offset;

  pde_start = PDE_ENTRY(vaddr);
  pte_start = PTE_ENTRY(vaddr);
  pde_end = PDE_ENTRY(vaddr + size);
  pte_end = PTE_ENTRY(vaddr + size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      /*
       * a)
       */

      if (pd_get_table(&pd, pde, &pt) != ERROR_NONE)
	{
	  pt.rw = 1;
	  pt.present = 1;
	  pt.user = 0;

	  if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			      &ptseg) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  pt.entries = ptseg;

	  if (pd_add_table(&pd, pde, pt) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      /*
       * b)
       */

      if (region_fit(o, PAGESZ, &chunk) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      chiche = chunk;

      if (ia32_region_map_chunk(o, (t_vaddr)chiche,
				(t_paddr)pt.entries) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.entries = chiche;

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte <= (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
	   pte++)
	{
	  /*
	   * d)
	   */

	  pg.addr = paddr;
	  paddr += PAGESZ;

	  if (pt_add_page(&pt, pte, pg) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  /*
	   * e)
	   */

	  tlb_invalidate((t_vaddr)ENTRY_ADDR(pde, pte));
	}

      /*
       * f)
       */

      if (ia32_region_unmap_chunk(o, (t_vaddr)pt.entries) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  if (ia32_region_unmap_chunk(o, (t_vaddr)pd) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

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

  return ERROR_UNKNOWN;
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

  /* map pd into kernel */

  pde_start = PDE_ENTRY(oreg->address);
  pte_start = PTE_ENTRY(oreg->address);
  pde_end = PDE_ENTRY(oreg->address + oreg->size);
  pte_end = PTE_ENTRY(oreg->address + oreg->size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      if (pd_get_table(&o->machdep.pd, pde, &pt) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      /* map pt in the kernel */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
	   pte++)
	{
	  if (pt_delete_page(&pt, pte) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  /* invalidate tlb */
	}

      /* unmap pt */

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
