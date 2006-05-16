/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [fri may 12 18:49:37 2006]
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
 * a look at the dispatch structure declaration to fill it correctly
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

#define MIRROR_PD	0
#define MIRROR_PT	1


/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager dispatch.
 */

d_region		region_dispatch =
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

t_vaddr			ia32_region_map(t_uint32		pte,
					t_paddr			p)
{
  t_vaddr		v;
  t_ia32_table		pt;
  t_ia32_page		pg;

  if (pd_get_table(NULL, PD_MIRROR, &pt) != ERROR_NONE)
    {
      cons_msg('!', "%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
      while (1)
	;
    }

  pg.present = 1;
  pg.rw = 1;
  pg.user = 0;
  pg.addr = p;

  if (pt_add_page(&pt, pte, pg) != ERROR_NONE)
    {
      cons_msg('!', "%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
      while (1)
	;
    }

  v = ENTRY_ADDR(PD_MIRROR, pte);

  tlb_invalidate(v);

  return v;
}

void			ia32_region_unmap(t_uint32		pte)
{
  t_vaddr		v;
  t_ia32_table		pt;

  if (pd_get_table(NULL, PD_MIRROR, &pt) != ERROR_NONE)
    {
      cons_msg('!', "%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
      while (1)
	;
    }

  if (pt_delete_page(&pt, pte) != ERROR_NONE)
    {
      cons_msg('!', "%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
      while (1)
	;
    }

  v = ENTRY_ADDR(PD_MIRROR, pte);

  tlb_invalidate(v);
}

/*
 * reserves a region.
 *
 * steps:
 *
 * XXX
 */

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    t_regid*		regid)
{
  o_as*			as;
  t_vaddr		virt;
  t_paddr		phys;
  t_uint32		pde;
  t_uint32		pte;
  t_ia32_directory	pd;
  t_ia32_table		pt;
  t_ia32_page		pg;
  t_segid		seg;
  int			clear;


  REGION_ENTER(region);

  if (as_get(asid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * map the pd.
   */

  pd = ia32_region_map(MIRROR_PD, as->machdep.pd);

  phys = segid + offset;
  virt = address;

  for (; virt < address + size; virt += PAGESZ, phys += PAGESZ)
    {
      pde = PDE_ENTRY(virt);
      pte = PTE_ENTRY(virt);

      clear = 0;
      if (pd_get_table(&pd, pde, &pt) != ERROR_NONE)
	{
	  /*
	   * create the pt.
	   */

	  if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE, &seg) !=
	      ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  pt.present = 1;
	  pt.rw = 1;
	  pt.user = 0;
	  pt.entries = seg;

	  if (pd_add_table(&pd, pde, pt) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  clear = 1;
	}

      /*
       * map the pt.
       */

      pt.entries = ia32_region_map(MIRROR_PT, pt.entries);

      /*
       * clear if newly created.
       */

      if (clear)
	memset((void*)pt.entries, 0, PAGESZ);

      /*
       * add the page.
       */

      pg.present = 1;
      pg.rw = 1;
      pg.user = 0;
      pg.addr = phys;

      if (pt_add_page(&pt, pte, pg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      /*
       * tlb.
       */

      if (asid == 0)
	tlb_invalidate(virt);

      /*
       * unmap pt.
       */

      ia32_region_unmap(MIRROR_PT);
    }

  /*
   * unmap pd.
   */

  ia32_region_unmap(MIRROR_PD);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function  releases a region.  we unmap the region  and release
 * page tables.
 *
 * steps:
 *
 * XXX
 */

t_error			ia32_region_release(t_asid		asid,
					    t_regid		regid)
{
  REGION_ENTER(region);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function just initialises the machine-dependent region manager.
 *
 * for the intel 32-bit architecture we just initialise the paging mode.
 */

t_error			ia32_region_init(t_vaddr		start,
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
