/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [mon feb 20 12:20:52 2006]
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

    ia32_as_show,
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

  cons_msg('#', "  page directory base: 0x%x\n", *o->machdep.pd);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) reserve a segment for the directory.
 * 3) reserve a region for the directory in the kernel address space.
 * 4) build a new page directory for the as.
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

  /*
   * 2)
   */

  if (segment_reserve(*asid, PAGESZ,
		      PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_get(seg, &oseg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (tskid == ktask)
    {
      /*
       * XXX we have to do the mapping by hand since the kernel pd is
       * not initialized.
       */



    }
  else
    {
      if (region_reserve(kasid, seg, 0, REGION_OPT_FORCE, seg, PAGESZ, &reg) !=
	  ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (pd_build(oseg->address, &o->machdep.pd, 1) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

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

  if (region_release(reg, kasid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_release(seg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
