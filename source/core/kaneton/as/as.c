/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Wed Jul 20 14:47:16 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager variable.
 */

m_as*			as = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the address spaces managed by the kernel.
 *
 * steps:
 *
 * XXX)
 */

#if (KANETON_DEBUG & KANETON_DEBUG_AS)
int			as_dump(void)
{
  o_as*			data;
  t_setsz		size;
  o_set*		o;
  t_iterator		i;

  /*
   * 1)
   */

  as_check(set);

  /*
   * 2)
   */

  if (set_size(as->container, &size) != 0)
    return (-1);

  /*
   * 3)
   */

  cons_msg('#', "dumping %qu address space(s):\n", size);

  set_foreach(SET_OPT_FORWARD, as->container, &i)
    {
      if (set_get(as->container, i, (void**)&data) != 0)
	return (-1);

      cons_msg('#', "%qu\n", data->asid);
    }

  return (0);
}
#endif

/*
 * this function reserves an address space
 */

int			as_rsv(t_asid*				asid)
{
  o_as			o;

  memset(&o, 0x0, sizeof(o_as));

  if (id_rsv(&as->id, &o.asid) != 0)
    return (-1);

  if (set_rsv(ll, SET_OPT_ALLOC, sizeof(t_segid), &o.segments) != 0)
    {
      id_rel(&as->id, o.asid);

      return (-1);
    }

  if (set_rsv(ll, SET_OPT_ALLOC, sizeof(o_region), &o.regions) != 0)
    {
      id_rel(&as->id, o.asid);

      // XXX set_rel(o.segments);

      return (-1);
    }

  if (set_add(as->container, &o) != 0)
    {
      id_rel(&as->id, o.asid);

      // XXX set_rel(o.segments);

      // XXX set_rel(o.regions);

      return (-1);
    }

  return (0);
}

/*
 * this functions initializes the address space manager from the init
 * variable.
 *
 * steps:
 *
 * 1) XXX
 */

int			as_init(void)
{
  /*
   * 1)
   */

  if ((as = malloc(sizeof(m_as))) == NULL)
    {
      cons_msg('!', "as: cannot allocate memory for the address space manager "
	       "structure\n");

      return (-1);
    }

  memset(as, 0x0, sizeof(m_as));

  /*
   * 2)
   */

  if (id_build(&as->id) != 0)
    {
      cons_msg('!', "as: unable to initialize the identifier object\n");

      return (-1);
    }

  /*
   * 3)
   */

  if (set_rsv(ll, SET_OPT_ALLOC, sizeof(o_as), &as->container) != 0)
    {
      cons_msg('!', "as: unable to reserve the address space container\n");

      return (-1);
    }

  /*
   * 4)
   */

  if (as_rsv(&as->kas) != 0)
    {
      cons_msg('!', "as: unable to reserve the kernel address space\n");

      return (-1);
    }

  /*
   * 5)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_AS)
  as_dump();
#endif

  return (0);
}
