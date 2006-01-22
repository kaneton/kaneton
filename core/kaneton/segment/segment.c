/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/segment/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sat jan 21 02:58:32 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX les segments ont un identificateur pour etre retrouve facilement
 *     neanmoins ceux ci ne peuvent pas etre tries selon cet identificateur
 *     sinon l ensemble ne serait pas trier en fonction de l agencement
 *     de la memoire physique. on va donc generer des identifiants pour
 *     pouvoir les retrouver mais on va creer un set qu'on va organiser
 *     nous meme afin qu'il soit trier selon les adresses physiques.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(segment);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager structure.
 */

m_segment*		segment;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a segment.
 *
 * steps:
 *
 * 1) gets the segment object.
 * 2) computes the type string.
 * 3) computes the perms string.
 * 4) displays the entry.
 */

t_error			segment_show(t_segid			segid)
{
  char			perms[4];
  char*			type;
  o_segment*		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  cons_msg('#', "  segment %qd in address space %qd:\n",
	   segid,
	   o->asid);

  /*
   * 2)
   */

  switch (o->type)
    {
    case SEGMENT_TYPE_MEMORY:
      type = "memory";
      break;
    case SEGMENT_TYPE_CATCH:
      type = "catch";
      break;
    default:
      type = "(unknown)";
      break;
    }

  /*
   * 3)
   */

  memset(perms, '.', 3);
  perms[3] = 0;

  if (o->perms & PERM_READ)
    perms[0] = 'r';

  if (o->perms & PERM_WRITE)
    perms[1] = 'w';

  if (o->perms & PERM_EXEC)
    perms[2] = 'x';

  /*
   * 4)
   */

  cons_msg('#', "    [%s] 0x%08x - 0x%08x %s (%u bytes)\n",
	   type,
	   o->address,
	   o->address + o->size,
	   perms,
	   o->size);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function dumps the segments.
 *
 * steps:
 *
 * 1) gets the size of the segment set.
 * 2) prints the header message.
 * 3) for each entry in the segment set, prints the area,
 *    its size and permissions.
 */

t_error			segment_dump(void)
{
  t_state		state;
  o_segment*		data;
  t_setsz		size;
  t_iterator		i;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (set_size(segment->container, &size) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu segment(s) from the segment container:\n", size);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, segment->container, &i, state)
    {
      if (set_object(segment->container, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot find the segment object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      if (segment_show(data->segid) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function clones a segment.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) chooses the correct fit.
 * 3) calls the machine dependent code.
 */

t_error			segment_clone(t_asid			asid,
				      t_segid			old,
				      t_segid*			new)
{
  o_segment*		from;
  o_as*			as;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_get(old, &from) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (segment_reserve(asid, from->size, from->perms, new) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (machdep_call(segment, segment_clone, asid, old, new) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function injects a pre-allocated segment in an address space.
 *
 * steps:
 *
 * 1) gets the address space object.
 * 2) fills the segment.
 * 3) adds the segment.
 * 4) calls dependent code.
 */

t_error			segment_inject(o_segment*	seg,
				       t_asid		asid)
{
  o_as			*o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg->segid = (t_segid)seg->address;
  seg->asid = asid;

  /*
   * 3)
   */

  if (set_add(segment->container, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_add(o->segments, &seg->segid) != ERROR_NONE)
    {
      set_remove(segment->container, seg->segid);

      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machdep_call(segment, segment_inject, seg, asid) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function gives a segment to an address space.
 *
 * steps:
 *
 * 1) gets the segment object.
 * 2) finds the destination address space.
 * 3) finds the source address space.
 * 4) removes from the source address space.
 * 5) adds into the destination one.
 * 6) calls dependent code.
 */

t_error			segment_give(t_asid		asid,
				     t_segid		segid)
{
  o_segment*		o;
  o_as*			dest;
  o_as*			src;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (as_get(asid, &dest) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(o->asid, &src) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_remove(src->segments, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  o->asid = asid;
  if (set_add(dest->segments, &segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (machdep_call(segment, segment_give, asid, segid) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

t_error			segment_resize(t_segid		segid,
				       t_psize		new_size)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

t_error			segment_split(t_segid		segid,
				      t_psize		sz1,
				      t_segid*		s1,
				      t_segid*		s2)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

t_error			segment_coalesce(t_segid	s1,
					 t_segid	s2,
					 t_segid*	new_seg)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

t_error			segment_read(t_segid		segid,
				     t_paddr		offs,
				     const void*	buff,
				     t_psize		sz)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

t_error			segment_write(t_segid		segid,
				      t_paddr		offs,
				      void*		buff,
				      t_psize		sz)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

t_error			segment_copy(t_segid		dst,
				     t_paddr		offsd,
				     t_segid		src,
				     t_paddr		offss,
				     t_psize		sz)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

t_error			segment_vaddr(t_segid		segid,
				      t_vaddr*		address)
{
  SEGMENT_ENTER(segment);

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

/*
 * this function reserves a segment given the desired size.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) chooses the correct fit.
 * 3) builds the segment.
 * 4) calls the machine dependent code.
 */

t_error			segment_reserve(t_asid			asid,
					t_psize			size,
					t_perms			perms,
					t_segid*		segid)
{
  o_as*			as;
  o_segment		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_fit(as, size, &o.address) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  o.asid = as->asid;
  o.size = size;
  o.perms = perms;

  *segid = o.segid = (t_segid)o.address;

  if (set_add(segment->container, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_add(as->segments, &o.segid) != ERROR_NONE)
    {
      set_remove(segment->container, o.segid);

      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machdep_call(segment, segment_reserve, asid, size, perms, segid) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function releases a segment.
 *
 * steps:
 *
 * 1) calls the machine dependent code.
 * 2) gets the segment object.
 * 3) gets the as object from its identifier.
 * 4) removes the segment from the address space.
 * 5) removes the segment from the segment container.
 */

t_error			segment_release(t_segid			segid)
{
  o_as*			as;
  o_segment*		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machdep_call(segment, segment_release, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(o->asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_remove(as->segments, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (set_remove(segment->container, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function permits to get entire possession of a memory
 * area.
 *
 * the segments with the catch type are very special and generally used
 * for architecture specific use.
 *
 * steps:
 *
 * 1) gets the address space object.
 * 2) gets the segment object.
 * 3) checks the segment type.
 * 4) finally adds the segment into the address space object
 * 5) and marks the segment as classical memory segment.
 * 6) calls the machine-dependent code.
 */

t_error			segment_catch(t_asid			asid,
				      t_segid			segid)
{
  o_as*                 as;
  o_segment*		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (o->type != SEGMENT_TYPE_CATCH)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  /* XXX d'abord l'enlever de l'ancien asid si present, ensuite le rajouter
   mais egalement mettre l'asid correctement */

  if (set_add(as->segments, &o->segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  o->type = SEGMENT_TYPE_MEMORY;

  /*
   * 6)
   */

  if (machdep_call(segment, segment_catch, asid, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function sets the permissions of a segment.
 *
 * steps:
 *
 * 1) gets the segment object.
 * 2) checks the perms argument.
 * 3) finally, sets the new permissions.
 * 4) calls the machine-dependent code.
 */

t_error			segment_perms(t_segid			segid,
				      t_perms			perms)
{
  o_segment*		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (!(perms & PERM_EXEC) && !(perms & PERM_READ) & !(perms & PERM_WRITE))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  o->perms = perms;

  /*
   * 4)
   */

  if (machdep_call(segment, segment_perms, segid, perms) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function changes the type of a segment.
 *
 * steps:
 *
 * 1) gets the segment object.
 * 2) checks the type argument.
 * 3) finally, sets the new type.
 * 4) calls the machine-dependent code.
 */

t_error			segment_type(t_segid			segid,
				     t_type			type)
{
  o_segment*		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if ((type != SEGMENT_TYPE_MEMORY) && (type != SEGMENT_TYPE_CATCH))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  o->type = type;

  /*
   * 4)
   */

  if (machdep_call(segment, segment_type, segid, type) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function removes every segment that belongs to the address space
 * specified.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) gets the address space object.
 * 3) for every segment in the address space, removes the segment from
 *    the segment container to destroy it.
 */

t_error			segment_flush(t_asid			asid)
{
  t_state		state;
  t_segid*		data;
  o_as*			as;
  t_iterator		i;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machdep_call(segment, segment_flush, asid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, as->segments, &i, state)
    {
      if (set_object(as->segments, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot find the object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      if (segment_release(*data) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

 SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function returns a segment object.
 */

t_error			segment_get(t_segid			segid,
				    o_segment**			o)
{
  SEGMENT_ENTER(segment);

  if (set_get(segment->container, segid, (void**)o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function initialises the segment manager from the init
 * variable containing segments to keep safe.
 *
 * steps:
 *
 * 1) allocates and initialises the segment manager structure.
 * 2) initialises the segment manager structure fields from the init
 *    structure.
 * 3) reserves the segment set which will contain the system's segments.
 * 4) tries to reserve a statistics object.
 * 5) calls the machine-dependent code.
 * 6) if needed, dumps the segments.
 */

t_error			segment_init(void)
{
  /*
   * 1)
   */

  if ((segment = malloc(sizeof(m_segment))) == NULL)
    {
      cons_msg('!', "segment: cannot allocate memory for the segment "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(segment, 0x0, sizeof(m_segment));

  /*
   * 2)
   */

  segment->start = init->mem;
  segment->size = init->memsz;
  segment->fit = SEGMENT_FIT;

  /*
   * 3)
   */

  if (set_reserve(bpt, SET_OPT_ALLOC | SET_OPT_SORT, sizeof(o_segment),
		  SEGMENT_BPT_NODESZ, &segment->container) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to reserve the segment container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  STATS_RESERVE("segment", &segment->stats);

  /*
   * 5)
   */

  if (machdep_call(segment, segment_init, SEGMENT_FIT) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 6)
   */

#if (DEBUG & DEBUG_SEGMENT)
  segment_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the segment manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the stats object.
 * 3) releases the segment container.
 * 4) frees the segment manager structure's memory.
 */

t_error			segment_clean(void)
{
  t_state		state;
  o_segment*		data;
  t_iterator		i;

  /*
   * 1)
   */

  if (machdep_call(segment, segment_clean) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, segment->container, &i, state)
    {
      if (set_object(segment->container, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot find the object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      if (segment_release(data->segid) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  if (set_release(segment->container) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to release the segment container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  STATS_RELEASE(segment->stats);

  /*
   * 4)
   */

  free(segment);

  return (ERROR_NONE);
}
