/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/kaneton/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [wed dec 28 18:50:27 2005]
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

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager interface.
 */

i_as			as_interface =
  {
    ia32_as_give,
    ia32_as_clone,
    ia32_as_reserve,
    ia32_as_release,
    ia32_as_init,
    ia32_as_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

t_error			ia32_as_give(t_asid			asid,
				     t_tskid			tskid)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_clone(t_tskid			tskid,
				      t_asid			old,
				      t_asid*			new)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) gets the as object.
 * 2) builds a new ldt for the as.
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  o_as*			o;
  t_uint16		nb = 10; /* XXX */

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (ldt_build(nb, (t_paddr)malloc(nb * sizeof(t_ldte)),
		&o->machdep.ldt, 1) != ERROR_NONE)
    AS_LEAVE(as, ERROR_NONE);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) gets address space object.
 * 2) gets the base of the ldt.
 * 3) destroys the ldt.
 * 4) frees the allocated table.
 */

t_error			ia32_as_release(t_asid			asid)
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

  if (ldt_base(&o->machdep.ldt, &base) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (ldt_destroy(&o->machdep.ldt) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  free((void*)base);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_init(void)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_clean(void)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}
