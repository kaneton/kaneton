/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/kaneton/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [mon jan  2 22:51:53 2006]
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
    ia32_region_reserve,
    NULL,
    NULL,
    ia32_region_init,
    ia32_region_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * reserves a region.
 *
 * steps:
 *
 * 1) gets as and segment objects.
 * 2) updates the flags for future pages.
 * 3) adds a new page table if necessary.
 * 4) reserves a new page.
 * 5) adds the new page.
 */

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_opts		opts,
					    t_vaddr		address,
					    t_regid*		regid)
{
  o_segment*		seg;
  o_as*			oas;
  o_task*		task;
  t_vaddr		p;
  t_table		tab;
  t_page		page;
  t_uint16		entry;
  t_uint8		rw;
  t_uint8		user;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(asid, &oas) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (segment_get(segid, &seg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (task_get(oas->tskid, &task) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  rw = (seg->perms & PERM_WRITE);
  user = (task->class == TASK_CLASS_PROGRAM);

  for (entry = PD_MAX_ENTRIES + 1, p = address;
       p < address + seg->size;
       p += PAGE_SIZE)
    {
      /*
       * 3)
       */

      if (entry != PDE_ENTRY(p))
	{
	  entry = PDE_ENTRY(p);

	  /* XXX init tab */

	  if (pd_add_table(&oas->machdep.pd, entry, tab) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);
	}

      /*
       * 4)
       */

      if (page_reserve(&page) != ERROR_NONE)
	{
	  /* XXX mise en swap */
	}

      page.rw = rw;
      page.user = user;

      /*
       * 5)
       */

      if (pt_add_page(&tab, PTE_ENTRY(p), page) != ERROR_NONE)
	{
	  /* XXX virer ce qu'on vient de creer */
	  REGION_LEAVE(region, ERROR_UNKNOWN);
	}
    }

  /* XXX regid ?? */

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
