/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/as/as.c
 *
 * created       julien quintard   [tue dec 13 03:05:27 2005]
 * updated       matthieu bucchianeri   [tue mar 14 19:02:50 2006]
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
 * a task can give its address space to another with as_give.
 *
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
 * the student just has to write the functions to reserve, release, get etc..
 * an address space.
 *
 * note that the address space does nothing more. indeed, the segment
 * manager and the region manager will add and/or remove the segments/regions
 * to/from the address space by their own.
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
 * 2) for each address space hold by the address space container, dumps
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

t_error			as_give(t_asid			asid,
				t_tskid			tskid)
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

  /*
   * 5)
   */

  if (machdep_call(as, as_give, asid, tskid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function translate a physical address to its virtual address.
 */

t_error			as_vaddr(t_asid			as,
				 t_segid		segid,
				 t_paddr		physical,
				 t_vaddr*		virtual)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_UNKNOWN);
}

/*
 * this function translate a virtual address to its physical one.
 *
 * steps:
 *
 * 1) get the region object.
 * 2) check boudaries.
 * 3) compute the physical address.
 * 4) call dependent code.
 */

t_error			as_paddr(t_asid		asid,
				 t_regid	regid,
				 t_vaddr	virtual,
				 t_paddr*	physical)
{
  o_region*		o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (region_get(asid, regid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (virtual < o->address || virtual >= o->address + o->size)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  *physical = (t_paddr)o->segid + o->offset + (virtual - o->address);

  /*
   * 4)
   */

  if (machdep_call(as, as_paddr, asid, regid, virtual, physical) !=
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
 * 6) clones the segment set from the source address space object.
 * 7) clones the region set from the source address space object.
 * 8) calls the machine-dependent code.
 */

t_error			as_clone(t_tskid			tskid,
				 t_asid				old,
				 t_asid*			new)
{
  t_state		state;
  o_task*		task;
  o_as*			from;
  o_as*			to;
  t_iterator		i;

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

  set_foreach(SET_OPT_FORWARD, from->segments, &i, state)
    {
      t_segid		needless;
      t_segid*		data;

      if (set_object(from->segments, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (segment_clone(to->segments, *data, &needless) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  set_foreach(SET_OPT_FORWARD, from->regions, &i, state)
    {
      /* XXX t_regid		needless; */
      o_region*		data;

      if (set_object(from->regions, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      /* XXX attention ici on va faire deux mapping sur un segment
	 sans aucun controle... en fait il faut creer une region pas sur
	 le data->segid mais sur le segid qu on a duplique a la boucle
	 precedente

	 il faudrait se faire une table de correspondance

      if (region_reserve(to->asid, data->segid, XXX, &needless) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
      */
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
 * 6) adds the new address space object in the address space container.
 * 7) sets the address space into the task object.
 * 8) calls the machine-dependent code.
 */

t_error			as_reserve(t_tskid			tskid,
				   t_asid*			asid)
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
		  sizeof(t_segid), &o.segments) != ERROR_NONE)
    {
      id_release(&as->id, o.asid);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, AS_REGIONS_INITSZ,
		  sizeof(o_region), &o.regions) != ERROR_NONE)
    {
      id_release(&as->id, o.asid);

      set_release(o.segments);

      AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  if (set_add(as->container, &o) != ERROR_NONE)
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
 * 7) removes the address space object from the address space container.
 */

t_error			as_release(t_asid			asid)
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

  if (set_remove(as->container, o->asid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function gets an address space object from the address space
 * container from its identifier.
 */

t_error			as_get(t_asid				asid,
			       o_as**				o)
{
  AS_ENTER(as);

  if (set_get(as->container, asid, (void**)o) != ERROR_NONE)
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
 * 3) reserves the addres space container set which will contain
 *    the address space build later.
 * 4) tries to reserve a statistics object.
 * 5) calls the machine-dependent code.
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

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
		  sizeof(o_as), &as->container) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to reserve the address space container\n");

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
 * 1) calls the machine-dependent code.
 * 2) releases the statistics object.
 * 3) releases the address space's container.
 * 4) destroys the id object.
 * 5) frees the address space manager structure's memory.
 */

t_error			as_clean(void)
{
  t_state		state;
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

  set_foreach(SET_OPT_FORWARD, as->container, &i, state)
    {
      if (set_object(as->container, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the address space object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (as_release(data->asid) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  if (set_release(as->container) != ERROR_NONE)
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
