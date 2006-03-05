/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sat mar  4 17:28:39 2006]
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
 *   kernel task:
 *  a) get the page directory from the init structure.
 *  b) add the mirroring entry.
 *  c) manually reserve a region for the mirror entry.
 *  d) clean the page directory.
 *   normal task:
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
  o_region		oreg;
  t_paddr		base;
  t_table		pt;

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

      o->machdep.pd = (t_directory)init->segments[10].address;

      /*
       * b)
       */

      pt.present = 1;
      pt.rw = 1;
      pt.user = 0;
      pt.entries = (void*)o->machdep.pd;

      if (pd_add_table(&o->machdep.pd, PD_MIRROR, pt) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      oreg.address = (t_vaddr)ENTRY_ADDR(PD_MIRROR, 0);
      oreg.regid = (t_regid)oreg.address;
      oreg.segid = (t_segid)o->machdep.pd;
      oreg.offset = 0;
      oreg.size = PT_MAX_ENTRIES * PAGESZ;

      if (set_add(o->regions, &oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * d)
       */

      /* XXX */
    }
  else
    {
      /*
       * a)
       */

      if (segment_reserve(*asid, PAGESZ,
			  PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (segment_get(seg, &oseg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      base = oseg->address;

      /*
       * b)
       */

      if (region_reserve(kasid, seg, 0, REGION_OPT_FORCE, seg, PAGESZ, &reg) !=
	  ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      if (pd_build(base, &o->machdep.pd, 1) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

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

  if (region_release(reg, kasid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_release(seg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
