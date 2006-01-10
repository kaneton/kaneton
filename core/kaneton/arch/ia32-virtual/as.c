/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue jan 10 00:28:25 2006]
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
 * 2) builds a new page directory for the as.
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  o_as*			o;
  t_uint16		nb = 10; /* XXX */
  void*			p;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

/*
  if (pd_build(???, &o->machdep.pd, 1) != ERROR_NONE)
    {
      free(p);
      AS_LEAVE(as, ERROR_NONE);
    }*/

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
 * 5) frees the allocated pages.
 * 6) frees page tables and directory.
 */

t_error			ia32_as_release(t_asid			asid)
{
  o_as*			o;
  t_paddr		base;
  t_table		tab;
  t_page		page;
  t_uint16		i;
  t_uint16		j;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */
#if 0
  for (i = 0; i < PD_MAX_ENTRIES; i++)
    {
      if (pd_get_table(&o->machdep.pd, i, &tab) != ERROR_NONE)
	continue;

      for (j = 0; j < PT_MAX_ENTRIES; j++)
	{
	  if (pt_get_page(&tab, j, &page) != ERROR_NONE)
	    continue;

	  /* XXX release page */
	}
    }

  /*
   * 3)
   */

  for (i = 0; i < PD_MAX_ENTRIES; i++)
    {
      if (pd_get_table(&o->machdep.pd, i, &tab) != ERROR_NONE)
	continue;

      /* XXX free table */
    }
#endif
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
