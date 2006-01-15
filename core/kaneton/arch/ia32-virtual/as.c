/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun jan 15 19:28:57 2006]
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

/*
 * the address space manager interface.
 */

i_as			as_interface =
  {
    NULL,
    NULL,
    ia32_as_reserve,
    ia32_as_release,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) gets the as object.
 * 2) reserves a segment for the directory or get the current one if kernel
 *    task
 * 3) builds a new page directory for the as.
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  o_as*			o;
  t_id			seg;
  o_segment*		oseg;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (tskid == ktask)
    {
      o->machdep.pd = init->machdep.pd;
    }
  else
    {
      if (segment_reserve(*asid, PAGE_SIZE,
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
       * 3)
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
 * 1) gets the as object.
 * 2) releases the page directory.
 */

t_error			ia32_as_release(t_asid		asid)
{
  o_as*			o;
  t_paddr		base;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */
/*
  XXX merdique: on release la pd avant les pt !

  if (pd_base(&o->machdep.pd, &base) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_release((t_segid)base) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);
*/
  AS_LEAVE(as, ERROR_NONE);
}
