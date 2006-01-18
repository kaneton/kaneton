/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [tue jan 17 00:31:32 2006]
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

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager interface.
 */

i_region		region_interface =
  {
    ia32_region_paddr,
    ia32_region_reserve,
    ia32_region_release,
    ia32_region_flush,
    ia32_region_init,
    ia32_region_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this  function  computes  the  physical  address  given  a  virtual
 * address.
 *
 * 1) gets the region object.
 * 2) checks the offset.
 * 3) gets the segment object.
 * 4) gets the page table for given virtual address.
 * 5) gets the entry for the given virtual address.
 * 6) computes the offset.
 */

t_error			ia32_region_paddr(t_asid		asid,
					  t_regid		regid,
					  t_vaddr		virtual,
					  t_paddr		*physical)
{
  o_as*			o;
  o_region*		reg;
  t_table		tab;
  t_page		pg;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (region_get(asid, regid, &reg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (virtual < reg->address || virtual >= reg->address + reg->size)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (pd_get_table(&o->machdep.pd, PDE_ENTRY(virtual), &tab) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (pt_get_page(&tab, PTE_ENTRY(virtual), &pg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 6)
   */

  *physical = (t_paddr)pg.addr + MK_OFFSET(virtual);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * reserves a region.
 *
 * steps:
 *
 * 1) gets the as object.
 * 2) gets the task object (used to check privilege level).
 * 3) fills the t_page structure.
 * 4) gets the segment object.
 * 5) loops throught the virtual memory to map.
 * 6) checks if we changed of page table.
 *  a) checks if the table already exists.
 *  b) reserves a segment for a new table.
 *  c) adds the new table.
 * 7) adds the page.
 */

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_opts		opts,
					    t_vaddr		address,
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

  if (task_get(o->tskid, &otsk) != ERROR_NONE)
    {
      cons_msg('!', "region: unable to get task object\n");
      REGION_LEAVE(region, ERROR_UNKNOWN);
    }

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
    {
      cons_msg('!', "region: unable to get segment object\n");
      REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  for (vaddr = address, paddr = oseg->address;
       vaddr < address + oseg->size;
       vaddr += PAGESZ, paddr += PAGESZ)
    {

      /*
       * 6)
       */

      if (PDE_ENTRY(vaddr) != pde)
	{
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
		{
		  cons_msg('!', "region: unable to reserve segment for "
			   "page table\n");
		  REGION_LEAVE(region, ERROR_UNKNOWN);
		}

	      if (segment_get(segtbl, &otbl) != ERROR_NONE)
		{
		  cons_msg('!', "region: unable to get segment for "
			   "page table\n");
		  REGION_LEAVE(region, ERROR_UNKNOWN);
		}

	      /*
	       * c)
	       */

	      pt.entries = (void*)otbl->address;

	      if (pd_add_table(&o->machdep.pd, pde, pt) != ERROR_NONE)
		{
		  cons_msg('!', "region: unable to add table to the "
			   "directory\n");
		  REGION_LEAVE(region, ERROR_UNKNOWN);
		}
	    }
	}

      /*
       * 7)
       */

      pg.addr = (void*)paddr;

      cons_msg('#', "%u:%u (v = %u) mapped to %u\n", pde, PTE_ENTRY(vaddr),
	       vaddr, paddr);

      if (pt_add_page(&pt, PTE_ENTRY(vaddr), pg) != ERROR_NONE)
	{
	  cons_msg('!', "region: unable to add page to a table\n");
	  REGION_LEAVE(region, ERROR_UNKNOWN);
	}

    }

  printf("\n\n");

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
  t_vaddr		vaddr;

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

  for (vaddr = oreg->address; vaddr < oreg->address + oreg->size;
       vaddr += PAGESZ)
    {
      /* XXX */
    }

  REGION_LEAVE(region, ERROR_UNKNOWN);
}

/*
 * this function flushes all the regions, free page tables.
 *
 */

t_error			ia32_region_flush(t_asid		asid)
{
  REGION_ENTER(region);

  REGION_LEAVE(region, ERROR_UNKNOWN);
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