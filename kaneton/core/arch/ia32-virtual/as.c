/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun apr  9 17:19:53 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for as  manager on  ia32 with
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

extern m_as*		as;
extern t_tskid		ktask;
extern t_init*		init;

/*
 * ---------- globals ---------------------------------------------------------
 */


/*                                                                  [cut] k2 */

t_asid			kasid = ID_UNUSED;

/*                                                                 [cut] /k2 */

/*
 * the address space manager dispatch.
 */

d_as			as_dispatch =
  {

/*                                                                  [cut] k2 */

    ia32_as_show,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_as_reserve,
    NULL,
    NULL,
    NULL

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function displays architecture dependent data.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) display machdep data.
 */

t_error			ia32_as_show(t_asid			asid)
{
  o_as*			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  page directory base: 0x%x\n", o->machdep.pd);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) get the as object.
 *   kernel task:
 *  a) get the page directory from the init structure.
 *  b) add the mirroring entry.
 *  c) manually reserve a region for the mirror entry.
 *  d) clean the page directory.
 *  e) reinject the page tables in the kernel as.
 *  f) invalidate MMU caches.
 *   normal task:
 *  a) reserve a segment for the directory.
 *  b) reserve a region for the directory in the kernel address space.
 *  c) build a new page directory for the as.
 *  d) release the kernel-side region mapping the directory.
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  o_as*			o;
  t_segid	        seg;
  t_regid		reg;
  o_segment*		oseg;
  o_segment		pt_seg;
  o_region		oreg;
  t_paddr		base;
  t_ia32_table		pt;
  t_uint32		i;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (tskid == ktask)
    {
      kasid = *asid;

      /*
       * a)
       */

      o->machdep.pd = init->machdep.pd;

      if (pd_activate(o->machdep.pd) != ERROR_NONE)
	TASK_LEAVE(task, ERROR_UNKNOWN);

      /*
       * b)
       */

      pt.present = 1;
      pt.rw = 1;
      pt.user = 0;
      pt.entries = (t_paddr)o->machdep.pd;

      if (pd_add_table(&o->machdep.pd, PD_MIRROR, pt) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      oreg.address = ENTRY_ADDR(PD_MIRROR, 0);
      oreg.segid = (t_segid)((t_uint32)o->machdep.pd);
      oreg.offset = 0;
      oreg.size = PT_MAX_ENTRIES * PAGESZ;

      if (region_inject(*asid, &oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * d)
       */

      pde_start = 0;
      pte_start = 0;

      for (i = 0; i < init->nregions + 1; i++)
	{
	  if (i != init->nregions)
	    {
	      pde_end = PDE_ENTRY(init->regions[i].address);
	      pte_end = PTE_ENTRY(init->regions[i].address);
	    }
	  else
	    {
	      pde_end = PD_MAX_ENTRIES;
	      pte_end = PT_MAX_ENTRIES;
	    }

	  for (pde = pde_start; pde <= pde_end; pde++)
	    {
	      if (pde != PD_MIRROR &&
		  pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
		{
		  for (pte = (pde == pde_start ? pte_start : 0);
		       pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
		       pte++)
		    {
		      pt_delete_page(&pt, pte);
		    }
		}
	    }

	  if (i != init->nregions)
	    {
	      pde_start = PDE_ENTRY(init->regions[i].address +
				    init->regions[i].size);
	      pte_start = PTE_ENTRY(init->regions[i].address +
				    init->regions[i].size);
	    }
	}

      /*
       * e)
       */

      for (i = 0; i < init->nregions; i++)
	{
	  pde_start = PDE_ENTRY(init->regions[i].address);
	  pde_end = PDE_ENTRY(init->regions[i].address +
			      init->regions[i].size);

	  for (pde = pde_start; pde <= pde_end; pde++)
	    {
	      if (pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
		{
		  seg = pt.entries;

		  if (segment_get(seg, &oseg) != ERROR_NONE)
		    {
		      pt_seg.address = (t_paddr)pt.entries;
		      pt_seg.size = PAGESZ;
		      pt_seg.perms = PERM_READ | PERM_WRITE;

		      if (segment_inject(*asid, &pt_seg, &seg) != ERROR_NONE)
			AS_LEAVE(as, ERROR_UNKNOWN);
		    }
		}
	    }
	}

      /*
       * f)
       */

      tlb_flush();
    }
  else
    {
      /*
       * a)
       */

      if (segment_reserve(*asid, PAGESZ,
			  PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * b)
       */

      if (region_reserve(kasid, seg, 0, REGION_OPT_NONE, 0, PAGESZ, &reg) !=
	  ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      base = reg;

      /* XXX pas tres malin le proto de pd_build ... */

      if (pd_build(base, &o->machdep.pd, 1) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      o->machdep.pd = (t_ia32_directory)(t_uint32)seg;

      /*
       * d)
       */

      if (region_release(kasid, reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
