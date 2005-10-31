/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Oct 30 23:09:23 2005   mycure
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
 * the first describes the segments held by this address space, in other
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
 *
 * moreover, this will be useful for creating kernel threads.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the address space manager builds address space objects.
 *
 * every address space object is essentially composed of two sets: a
 * set of segments which list the segments held by this address space and
 * a set of regions which describes the useable virtual address ranges.
 *
 * the student just has to write the functions to reserve, release, find etc..
 * an address space.
 *
 * note that the address space does nothing more. indeed, the segment
 * manager and the region manager will add and/or remove the segments/regions
 * to/from the address space by their own.
 *
 * the address space manager also builds a very special address space object
 * for the kernel.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(as);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager variable.
 */

m_as*			as = NULL;

/*
 * the kernel address space.
 */

t_asid			kas = ID_UNUSED;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function shows a precise address space.
 *
 * steps:
 *
 * 1) gets the address space object from its identifier.
 * 2) displays the segments held by the address space.
 * 3) displays the regions held by the address space.
 */

t_error			as_show(t_asid				asid)
{
  t_state		state;
  t_segid		seg;
  t_regid		reg;
  t_iterator		i;
  o_as*			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  cons_msg('#', "showing address space %qd:\n", asid);

  /*
   * 2)
   */

  cons_msg('#', "  segments %qd:\n", o->segments);

  set_foreach(SET_OPT_FORWARD, o->segments, &i, state)
    {
      if (set_object(o->segments, i, (void**)&seg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      cons_msg('#', "    %qd\n", seg);
    }

  /*
   * 3)
   */

  cons_msg('#', "  regions %qd:\n", o->regions);

  set_foreach(SET_OPT_FORWARD, o->regions, &i, state)
    {
      if (set_object(o->regions, i, (void**)&reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      cons_msg('#', "    %qd\n", reg);
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function dumps the address spaces managed by the kernel.
 *
 * steps:
 *
 * 1) gets the set's size.
 * 2) for each addres space hold by the address space container, dumps
 *    the address space identifier.
 */

t_error			as_dump(void)
{
  t_state		state;
  o_as*			data;
  t_setsz		size;
  o_set*		o;
  t_iterator		i;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (set_size(as->container, &size) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu address space(s):\n", size);

  set_foreach(SET_OPT_FORWARD, as->container, &i, state)
    {
      if (set_object(as->container, i, (void**)&data) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      cons_msg('#', "  %qd [%qd, %qd]\n",
	       data->asid,
	       data->segments,
	       data->regions);
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function clones an address space.
 *
 * steps:
 *
 * 1) gets the source address space object given its identifier.
 * 2) reserves the cloned address space object.
 * 3) gets the destination address space object previously reserved.
 * 4) releases the prereserved segment and region sets because we will
 *    clone these sets from the source address space object so we have to
 *    release them before.
 * 5) clones the segment and region sets from the source address space
 *    object.
 */

t_error			as_clone(t_asid				old,
				 t_asid*			new)
{
  o_as*			from;
  o_as*			to;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(old, &from) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (as_rsv(new) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(*new, &to) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_rel(to->segments) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_rel(to->regions) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_clone(from->segments, &to->segments) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_clone(from->regions, &to->regions) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) initialises the address space object.
 * 2) reserves an identifier for the address space object.
 * 3) reserves the set of segments for the new address space object.
 * 4) reserves the set of regions for the new address space object.
 * 5) adds the new address space object in the address space container.
 */

t_error			as_rsv(t_asid*				asid)
{
  o_as			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_as));

  /*
   * 2)
   */

  if (id_rsv(&as->id, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  o.asid = *asid;

  /*
   * 3)
   */

  if (set_rsv(array, SET_OPT_SORT | SET_OPT_ALLOC, AS_SEGMENTS_INITSZ,
	      sizeof(t_segid), &o.segments) != ERROR_NONE)
    {
      id_rel(&as->id, o.asid);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (set_rsv(array, SET_OPT_SORT | SET_OPT_ALLOC, AS_REGIONS_INITSZ,
	      sizeof(t_regid), &o.regions) != ERROR_NONE)
    {
      id_rel(&as->id, o.asid);

      set_rel(o.segments);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (set_add(as->container, &o) != ERROR_NONE)
    {
      id_rel(&as->id, o.asid);

      set_rel(o.segments);
      set_rel(o.regions);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) releases the address space object identifier.
 * 3) releases the address space object's set of segments.
 * 4) releases the address space object's set of regions.
 * 5) removes the address space object from the address space container.
 */

t_error			as_rel(t_asid				asid)
{
  t_iterator		iterator;
  o_as			*o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (id_rel(&as->id, o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_rel(o->segments) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_rel(o->regions) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_remove(as->container, o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function gets an address space object from the address space
 * container from its identifier.
 *
 * steps:
 *
 * 1) gets the address space object in the address space container.
 */

t_error			as_get(t_asid				asid,
			       o_as**				o)
{
  AS_ENTER(as);

  /*
   * 1)
   */

  if (set_get(as->container, asid, (void**)o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
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
 * 4) tries to reserve a statistics object.
 * 5) reserves the kernel address space.
 * 6) if asked, dumps the address space manager.
 */

t_error			as_init(void)
{
  /*
   * 1)
   */

  if ((as = malloc(sizeof(m_as))) == NULL)
    {
      cons_msg('!', "as: cannot allocate memory for the address space manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(as, 0x0, sizeof(m_as));

  /*
   * 2)
   */

  if (id_build(&as->id) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_as), &as->container) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to reserve the address space container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  STATS_RSV("as", &as->stats);

  /*
   * 5)
   */

  if (as_rsv(&kas) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to reserve the kernel address space\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

#if (DEBUG & DEBUG_AS)
  as_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the address space manager.
 *
 * steps:
 *
 * 1) releases the kernel address space.
 * 2) releases the statistics object.
 * 3) releases the address space's container.
 * 4) destroys the id object.
 * 5) frees the address space manager structure's memory.
 */

t_error			as_clean(void)
{
  /*
   * 1)
   */

  if (as_rel(kas) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to release the kernel address space\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  STATS_REL(as->stats);

  /*
   * 3)
   */

  if (set_rel(as->container) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to release the as' container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (id_destroy(&as->id) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  free(as);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k2 */
