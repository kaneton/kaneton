/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/as/as.c
 *
 * created       julien quintard   [tue dec 13 03:05:27 2005]
 * updated       matthieu bucchianeri   [fri aug 18 19:53:23 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the address space manager manages address spaces.
 *
 * an address space describes process' useable memory. each address space
 * is composed of two sets.
 *
 * the first describes the segments held by this address space, in other
 * words the physical memory.
 *
 * the latter describes the regions, the virtual areas which reference
 * some segments.
 *
 * a task can give its address space to another with as_give().
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
 * kernel address space.
 */

i_as			kasid = ID_UNUSED;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function shows a precise address space.
 *
 * steps:
 *
 * 1) get the address space object from its identifier.
 * 2) display the segments held by the address space.
 * 3) display the regions held by the address space.
 * 4) call machine dependent code.
 */

t_error			as_show(i_as				asid)
{
  t_state		state;
  i_segment		seg;
  i_region		reg;
  t_iterator		i;
  o_as*			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  cons_msg('#', "  address space %qd in task %qd:\n",
	   asid,
	   o->tskid);

  /*
   * 2)
   */

  cons_msg('#', "    segments %qd:\n", o->segments);

  set_foreach(SET_OPT_FORWARD, o->segments, &i, state)
    {
      if (set_object(o->segments, i, (void**)&seg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      cons_msg('#', "      %qd\n", seg);
    }

  /*
   * 3)
   */

  cons_msg('#', "    regions %qd:\n", o->regions);

  set_foreach(SET_OPT_FORWARD, o->regions, &i, state)
    {
      if (set_object(o->regions, i, (void**)&reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      cons_msg('#', "      %qd\n", reg);
    }

  /*
   * 4)
   */

  if (machdep_call(as, as_show, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function dumps the address spaces managed by the kernel.
 *
 * steps:
 *
 * 1) gets the set's size.
 * 2) for each address space hold by the address space set, dumps
 *    the address space identifier.
 */

t_error			as_dump(void)
{
  t_state		state;
  o_as*			data;
  t_setsz		size;
  t_iterator		i;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (set_size(as->ass, &size) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu address space(s):\n", size);

  set_foreach(SET_OPT_FORWARD, as->ass, &i, state)
    {
      if (set_object(as->ass, i, (void**)&data) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (as_show(data->asid) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function gives an address space from a task to another.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) gets the source task object.
 * 3) gets the destination task object.
 * 4) swaps the address space identifier.
 * 5) calls the machine-dependent code.
 */

t_error			as_give(i_task			tskid,
				i_as			asid)
{
  o_task*		from;
  o_task*		to;
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

  if (task_get(o->tskid, &from) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (task_get(tskid, &to) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (to->asid != ID_UNUSED)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  from->asid = ID_UNUSED;
  to->asid = asid;
  o->tskid = tskid;

  /*
   * 5)
   */

  if (machdep_call(as, as_give, tskid, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function translate a physical address to its virtual address.
 */

t_error			as_vaddr(i_as			asid,
				 t_paddr		physical,
				 t_vaddr*		virtual)
{
  o_as*			o;
  o_region*		oreg;
  o_segment*		oseg;
  t_iterator		i;
  t_state		state;
  t_uint32		found = 0;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, o->regions, &i, state)
    {
      if (set_object(o->regions, i, (void**)&oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (segment_get(oreg->segid, &oseg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (oseg->address + oreg->offset <= physical &&
	  physical < oseg->address + oreg->offset + oreg->size)
	{
	  found = 1;
	  break;
	}
    }

  /*
   * 3)
   */

  if (!found)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  *virtual = oreg->address - oreg->offset + (physical - oseg->address);

  /*
   * 5)
   */

  if (machdep_call(as, as_vaddr, asid, physical, virtual) !=
      ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function translate a virtual address to its physical one.
 *
 * steps:
 *
 * 1) get the region object.
 * 2) look for the good region.
 * 3) check if the region was found.
 * 4) compute the physical address.
 * 5) call dependent code.
 */

t_error			as_paddr(i_as		asid,
				 t_vaddr	virtual,
				 t_paddr*	physical)
{
  o_as*			o;
  o_region*		oreg;
  t_iterator		i;
  t_state		state;
  t_uint32		found = 0;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, o->regions, &i, state)
    {
      if (set_object(o->regions, i, (void**)&oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (oreg->address <= virtual && virtual < oreg->address + oreg->size)
	{
	  found = 1;
	  break;
	}
      if (oreg->address > virtual)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (!found)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  *physical = (t_paddr)oreg->segid + oreg->offset + (virtual - oreg->address);

  /*
   * 5)
   */

  if (machdep_call(as, as_paddr, asid, virtual, physical) !=
      ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function clones an address space.
 *
 * steps:
 *
 * 1) gets the source address space object given its identifier.
 * 2) gets the task object.
 * 3) reserves the cloned address space object.
 * 4) gets the destination address space object previously reserved.
 * 5) prepare a temporary set for the mapping between segments and regions.
 * 6) clones the segment set from the source address space object.
 * 7) clones the region set from the source address space object.
 * 8) calls the machine-dependent code.
 */

t_error			as_clone(i_task				tskid,
				 i_as				old,
				 i_as*				new)
{
  t_state		state;
  o_task*		task;
  o_as*			from;
  o_as*			to;
  t_iterator		i;
  t_setsz		nb_segments;
  i_set			mapping;
  i_segment		foo[2];
  i_segment*		map = foo;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(old, &from) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(tskid, &task) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (task->asid != ID_UNUSED)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_reserve(tskid, new) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (as_get(*new, &to) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_size (from->segments, &nb_segments) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_reserve(array, SET_OPT_ALLOC, nb_segments,
		  sizeof (foo), &mapping) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 6)
   */

  set_foreach(SET_OPT_FORWARD, from->segments, &i, state)
    {
      i_segment		needless;
      i_segment*	data;

      if (set_object(from->segments, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (segment_clone(to->asid, *data, &needless) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      map[0] = *data;
      map[1] = needless;

      printf ("cloning segment %qd -> %qd\n", *data, needless);

      if (set_add(mapping, map) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  set_show_array(mapping);

  set_foreach(SET_OPT_FORWARD, from->regions, &i, state)
    {
      i_region		needless;
      o_region*		data;

      if (set_object(from->regions, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (set_get(mapping, data->segid, (void**)&map) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      printf ("map = %qd -> %qd\n", map[0], map[1]);
      printf ("mapping region to segment %qd (prev. %qd)\n", map[1], data->segid);

      // XXX priv
      if (region_reserve(to->asid, map[1], data->offset, REGION_OPT_FORCE,
			 data->address, data->size, &needless) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  if (machdep_call(as, as_clone, tskid, old, new) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) gets the task object.
 * 2) initialises the address space object.
 * 3) reserves an identifier for the address space object.
 * 4) reserves the set of segments for the new address space object.
 * 5) reserves the set of regions for the new address space object.
 * 6) adds the new address space object in the address space set.
 * 7) sets the address space into the task object.
 * 8) calls the machine-dependent code.
 */

t_error			as_reserve(i_task			tskid,
				   i_as*			asid)
{
  o_task*		task;
  o_as			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (task_get(tskid, &task) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (task->asid != ID_UNUSED)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof(o_as));

  o.tskid = tskid;

  o.segments = ID_UNUSED;
  o.regions = ID_UNUSED;

  /*
   * 3)
   */

  if (id_reserve(&as->id, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  o.asid = *asid;

  /*
   * 4)
   */

  if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, AS_SEGMENTS_INITSZ,
		  sizeof(i_segment), &o.segments) != ERROR_NONE)
    {
      id_release(&as->id, o.asid);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (set_reserve(ll, SET_OPT_SORT | SET_OPT_FREE, /* AS_REGIONS_INITSZ,*/
		  sizeof(o_region), &o.regions) != ERROR_NONE) /* XXX */
    {
      id_release(&as->id, o.asid);

      set_release(o.segments);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  if (set_add(as->ass, &o) != ERROR_NONE)
    {
      id_release(&as->id, o.asid);

      set_release(o.segments);
      set_release(o.regions);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  task->asid = *asid;

  /*
   * 8)
   */

  if (machdep_call(as, as_reserve, tskid, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) gets the task object and removes the address space from it.
 * 3) releases the address space object identifier.
 * 4) releases the address space object's set of regions.
 * 5) releases the address space object's set of segments.
 * 6) calls the machine-dependent code.
 * 7) removes the address space object from the address space set.
 */

t_error			as_release(i_as			asid)
{
  o_task*		task;
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

  if (task_get(o->tskid, &task) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  task->asid = ID_UNUSED;

  /*
   * 3)
   */

  if (id_release(&as->id, o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (region_flush(o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_release(o->regions) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (segment_flush(o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_release(o->segments) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (machdep_call(as, as_release, asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);
  /*
   * 7)
   */

  if (set_remove(as->ass, o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function gets an address space object from the address space
 * set from its identifier.
 */

t_error			as_get(i_as				asid,
			       o_as**				o)
{
  AS_ENTER(as);

  if (set_get(as->ass, asid, (void**)o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function initialises the address space manager.
 *
 * this function takes care of initialising and builing the kernel
 * address space.
 *
 * steps:
 *
 * 1) allocates and initialises the address space manager structure.
 * 2) initialises the identifier object to be able to generate
 *    the address space identifiers.
 * 3) reserves the addres space set which will contain the address space
 *    built later.
 * 4) tries to reserve a statistics object.
 * 5) calls the machine-dependent code.
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

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_as), &as->ass) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to reserve the address space set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  STATS_RESERVE("as", &as->stats);

  /*
   * 5)
   */

  if (machdep_call(as, as_init) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the address space manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the statistics object.
 * 3) releases the address space's set.
 * 4) destroys the id object.
 * 5) frees the address space manager structure's memory.
 */

t_error			as_clean(void)
{
  o_as*			data;
  t_iterator		i;

  /*
   * 1)
   */

  if (machdep_call(as, as_clean) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  STATS_RELEASE(as->stats);

  /*
   * 3)
   */

  while (set_head(as->ass, &i) == ERROR_NONE)
    {
      if (set_object(as->ass, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the address space object "
		   "corresponding to its identifier\n");

	  return (ERROR_UNKNOWN);
	}

      if (data->asid == kasid)
	set_remove(as->ass, data->asid);
      else
	if (as_release(data->asid) != ERROR_NONE)
	  return (ERROR_UNKNOWN);
    }

  if (set_release(as->ass) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to release the as' set\n");

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
