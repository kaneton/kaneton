/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri Jul 22 14:02:32 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the address space manager manages address spaces.
 *
 * in kaneton, the address spaces are not directly linked to processes.
 *
 * in fact, an address space has to be reserved to, then, be attached
 * to a task.
 *
 * an address space describes process' useable memory. each address space
 * is composed of two set.
 *
 * the first describes the segments hold by this address space, in other
 * words the physical memory.
 *
 * the latter describes the regions, the virtual areas which reference
 * some segments.
 *
 * note the the address space manager specifically manages the kernel
 * address space because there is no task object for the kernel so
 * no address space linked to it.
 *
 * we wanted to be able to dump the memory used by the kernel so we
 * decided to create a static kernel address space.
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
 * 1) checks whether the address space manager was previously initialised.
 * 2) gets the set's size.
 * 3) for each addres space hold by the address space container, dumps
 *    the address space identifier.
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
      if (set_object(as->container, i, (void**)&data) != 0)
	return (-1);

      cons_msg('#', "  %qu\n", data->asid);
    }

  return (0);
}
#endif

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) first, checks whether the address space manager is initialised.
 * 2) initialises the address space object.
 * 3) reserves an identifier for the address space object.
 * 4) reserves the set of segments for the new address space object.
 * 5) reserves the set of regions for the new address space object.
 * 6) adds the new address space object in the address space container.
 */

int			as_rsv(t_asid*				asid)
{
  o_as			o;

  /*
   * 1)
   */

  as_check(set);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_as));

  /*
   * 3)
   */

  if (id_rsv(&as->id, &o.asid) != 0)
    return (-1);

  /*
   * 4)
   */

  if (set_rsv(array, SET_OPT_SORT | SET_OPT_ALLOC, AS_SEGMENTS_INITSZ,
	      sizeof(t_segid), &o.segments) != 0)
    {
      id_rel(&as->id, o.asid);

      return (-1);
    }

  /*
   * 5)
   */

  if (set_rsv(array, SET_OPT_SORT | SET_OPT_ALLOC, AS_REGIONS_INITSZ,
	      sizeof(o_region), &o.regions) != 0)
    {
      id_rel(&as->id, o.asid);

      set_rel(o.segments);

      return (-1);
    }

  /*
   * 6)
   */

  if (set_add(as->container, &o) != 0)
    {
      id_rel(&as->id, o.asid);

      set_rel(o.segments);

      set_rel(o.regions);

      return (-1);
    }

  return (0);
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) checks whether the address space manager was previously initialised.
 * 2) gets the address space object given its identifier.
 * 3) releases the address space object identifier.
 * 4) releases the address space object's set of segments.
 * 5) releases the address space object's set of regions.
 * 6) removes the address space object from the address space container.
 */

int			as_rel(t_asid				asid)
{
  t_iterator		iterator;
  o_as			*o;

  /*
   * 1)
   */

  as_check(set);

  /*
   * 2)
   */

  if (as_get(asid, &o) != 0)
    return (-1);

  /*
   * 3)
   */

  if (id_rel(&as->id, o->asid) != 0)
    return (-1);

  /*
   * 4)
   */

  if (set_rel(o->segments) != 0)
    return (-1);

  /*
   * 5)
   */

  if (set_rel(o->regions) != 0)
    return (-1);

  /*
   * 6)
   */

  if (set_remove(as->container, o->asid) != 0)
    return (-1);

  return (0);
}

/*
 * this function gets an address space object from the address space
 * container from its identifier.
 *
 * steps:
 *
 * 1) checks whether the address space manager was previously initialised.
 * 3) gets the address space object in the address space container.
 */

int			as_get(t_asid				asid,
			       o_as**				o)
{
  t_iterator		iterator;

  /*
   * 1)
   */

  as_check(set);

  /*
   * 2)
   */

  if (set_get(as->container, asid, (void**)o) != 0)
    return (-1);

  return (0);
}

/*
 * this functions initialises the address space manager from the init
 * variable.
 *
 * this function takes care of initialising and builing the kernel
 * address space.
 *
 * steps:
 *
 * 1) allocates and initialises the address space manager structure.
 * 2) initialises the identifier object to be able to generate
 *    the address space identifiers.
 * 3) reserves the addres space container set which will contain
 *    the address space build later.
 * 4) reserves the kernel address space.
 * 5) if asked, dumps the address space manager.
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
      cons_msg('!', "as: unable to initialise the identifier object\n");

      return (-1);
    }

  /*
   * 3)
   */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_as), &as->container) != 0)
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
