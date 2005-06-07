/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [wed feb 15 22:16:08 2006]
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
 * the address space manager interface.
 */

i_as			as_interface =
  {

/*                                                                  [cut] k2 */

    NULL,
    NULL,
    NULL,
    NULL,
    ia32_as_reserve,
    ia32_as_release,
    NULL,
    NULL

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) kernel task case:
 *  a) get the page directory from the init variable.
 *  b) inject current page tables and map them so the kernel can access them.
 * 3) normal address space:
 *  a) reserve a segment for the directory.
 *  b) reserve a region for the directory in the kernel address space.
 *  c) build a new page directory for the as.
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  o_as*			o;
  t_segid	        seg;
  t_regid		reg;
  o_segment*		oseg;
  t_uint32		pde;
  t_table		table;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (tskid == ktask)
    {
      /*
       * 2)
       */

      /*
       * a)
       */

      memcpy(&o->machdep.pd, &init->machdep.pd, sizeof (t_directory));
      kasid = *asid;

      /*
       * b)
       */

      o_segment		seg;

      for (pde = 0; pde < PD_MAX_ENTRIES; pde++)
	{
	  if (pd_get_table(&o->machdep.pd, pde, &table) == ERROR_NONE)
	    {
	      seg.address = (t_paddr)table.entries;
	      seg.size = PAGESZ;
	      seg.perms = PERM_READ | PERM_WRITE;

#if (DEBUG & DEBUG_AS)
	      printf("injecting segment %p for pde %d\n", seg.address, pde);
#endif

	      if (segment_inject(&seg, *asid) != ERROR_NONE)
		REGION_LEAVE(region, ERROR_UNKNOWN);

	      if (region_reserve(kasid, (t_segid)seg.address, 0,
				 REGION_OPT_FORCE, (t_vaddr)seg.address,
				 PAGESZ, &reg) != ERROR_NONE)
		REGION_LEAVE(region, ERROR_UNKNOWN);
	    }
	}
    }
  else
    {
      /*
       * 3)
       */

      /*
       * a)
       */

      if (segment_reserve(*asid, PAGESZ,
			  PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot reserve a segment for page-directory.\n");
	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (segment_get(seg, &oseg) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot get a segment for page-directory.\n");
	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      /*
       * b)
       */

      if (region_reserve(kasid, seg, 0, REGION_OPT_FORCE, seg, PAGESZ, &reg) !=
	  ERROR_NONE)
	{
	  cons_msg('!', "as: cannot reserve a region for page-directory.\n");
	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      /*
       * c)
       */

      if (pd_build(oseg->address, &o->machdep.pd, 1) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot build a page-directory.\n");
	  AS_LEAVE(as, ERROR_UNKNOWN);
	}
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) release page-directory segment and region.
 */

t_error			ia32_as_release(t_asid			asid)
{
  o_as*			o;
  t_segid		seg;
  t_regid		reg;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg = (t_segid)(t_uint32)(o->machdep.pd);
  reg = seg;

  if (region_release(reg, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_release(seg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
