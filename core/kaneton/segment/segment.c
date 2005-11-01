/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/segment
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Tue Nov  1 16:05:19 2005   mycure
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
 *
 * XXX resize, split, coalesce, read, write, copy, get, perms, type
 *     flush, seg->paddr (addr), asid
 *
 * XXX give (as, segment)
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

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		init;

/*
 * the kernel address space identifier.
 */

extern t_asid		kas;

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

t_error			segment_dump(t_segid			segid)
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

  cons_msg('#', "  segment %qd:\n", segid);

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

      if (set_show(data->segid) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function tries to find free space in the segment set via the
 * first fit algorithm.
 *
 * steps:
 *
 * 1) gets the first segment.
 * 2) tries to find space before the first segment.
 * 3) for each segment, tries to find space after it.
 * 4) gets the last segment.
 * 5) tries to find space after the last segment.
 */

t_error			segment_first_fit(o_as*			as,
					  t_psize		size,
					  t_perms		perms,
					  t_segid*		segid)
{
  o_segment*		current;
  t_state		state;
  o_segment*		head;
  o_segment*		tail;
  t_iterator		i;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (set_head(segment->container, &i) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_object(segment->container, i, (void**)&head) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if ((head->address - segment->start) >= size)
    {
      o_segment	o;

      memset(&o, 0x0, sizeof(o_segment));

      o.address = head->address + head->size;
      o.size = size;
      o.perms = perms;

      o.segid = (t_segid)o.address;

      if (set_add(segment->container, &o) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      if (set_add(as->segments, &o.segid) != ERROR_NONE)
	{
	  set_remove(segment->container, o.segid);

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      SEGMENT_LEAVE(segment, ERROR_NONE);
    }

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, segment->container, &i, state)
    {
      o_segment*	next;
      t_iterator	j;

      if (set_object(segment->container, i, (void**)&current) !=
	  ERROR_NONE)
	{
	  cons_msg('!', "set: cannot find the segment object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      if (set_next(segment->container, i, &j) != ERROR_NONE)
	break;

      if (set_object(segment->container, j, (void**)&next) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      if ((next->address - (current->address + current->size)) >= size)
	{
	  o_segment o;

	  memset(&o, 0x0, sizeof(o_segment));

	  o.address = current->address + current->size;
	  o.size = size;
	  o.perms = perms;

	  o.segid = (t_segid)o.address;

	  if (set_add(segment->container, &o) != ERROR_NONE)
	    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	  if (set_add(as->segments, &o.segid) != ERROR_NONE)
	    {
	      set_remove(segment->container, o.segid);

	      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	    }

	  SEGMENT_LEAVE(segment, ERROR_NONE);
	}
    }

  /*
   * 4)
   */

  if (set_tail(segment->container, &i) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_object(segment->container, i, (void**)&tail) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (((segment->start + segment->size) -
       (tail->address + tail->size)) >= size)
    {
      o_segment	o;

      memset(&o, 0x0, sizeof(o_segment));

      o.address = tail->address + tail->size;
      o.size = size;
      o.perms = perms;

      o.segid = (t_segid)o.address;

      if (set_add(segment->container, &o) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      if (set_add(as->segments, &o.segid) != ERROR_NONE)
	{
	  set_remove(segment->container, o.segid);

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      SEGMENT_LEAVE(segment, ERROR_NONE);
    }

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function reserves a segment given the desired size.
 *
 * steps:
 *
 * 1) gets the address space object given its identifier.
 * 2) chooses the correct fit.
 */

t_error			segment_reserve(t_asid			asid,
					t_psize			size,
					t_perms			perms,
					t_segid*		segid)
{
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

  switch (segment->fit)
    {
    case FIT_FIRST:
      {
	return (segment_first_fit(as, size, perms, segid));
      }
    default:
      {
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
      }
    }

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

/*
 * this function releases a segment.
 *
 * steps:
 *
 * 1) gets the as object from its identifier.
 * 2) removes the segment from the address space.
 * 3) removes the segment from the segment container.
 */

t_error			segment_release(t_asid			asid,
					t_segid			segid)
{
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

  if (set_remove(as->segments, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
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

  if (set_add(as->segments, &o->segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  o->type = SEGMENT_TYPE_MEMORY;

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function sets the permissions of a segment.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			segment_perms(t_asid			asid,
				      t_segid			segid,
				      t_perms			perms)
{
  o_as*			as;
  t_iterator		i;
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

  if (set_locate(as->segments, segid, &i) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (!(perms & PERM_EXEC) && !(perms & PERM_READ) & !(perms & PERM_WRITE))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  o->perms = perms;

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function changes the type of a segment.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			segment_type(t_asid			asid,
				     t_segid			segid,
				     t_type			type)
{
  o_as*			as;
  t_iterator		i;
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

  if (set_locate(as->segments, segid, &i) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((type != SEGMENT_TYPE_MEMORY) && (type != SEGMENT_TYPE_CATCH))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  o->type = type;

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * XXX peut etre a changer pour la clarete du design + machdep calls
 */

t_error			segment_flush(t_asid			asid)
{
  t_state		state;
  o_segment*		data;
  o_as*			as;
  t_iterator		i;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  set_foreach(SET_OPT_FORWARD, as->segments, &i, state)
    {
      if (set_object(as->segments, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot find the object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      if (set_remove(segment->container, data->segid) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot remove the previously found "
		   "object\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}
    }

  /*
   * 3)
   */

  if (set_flush(as->segments) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * XXX
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
 * 5) for each pre-reserved segment, inserts it into the segment container.
 *    note that we use the address as the identifier. this is not very
 *    elegant but we have no choice to be able to use every sorted set.
 *    moreover we could use the set without the SET_ALLOC option but
 *    we prefered use it to be more coherent.
 * 6) if needed, dumps the segments.
 */

t_error			segment_init(t_fit			fit)
{
  t_uint32		i;

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
  segment->fit = fit;

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

  for (i = 0; i < init->nsegments; i++)
    {
      init->segments[i].segid = (t_segid)init->segments[i].address;

      if (set_add(segment->container, &init->segments[i]) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot add a pre-reserved segment in "
		   "the segment container\n");

	  return (ERROR_UNKNOWN);
	}
    }

  /*
   * 6)
   */

#if (DEBUG & DEBUG_SEGMENT)
  // XXX segment_dump();

  // XXX segment_test();
#endif

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the segment manager.
 *
 * steps:
 *
 * 1) releases the stats object.
 * 2) releases the segment container.
 * 3) frees the segment manager structure's memory.
 */

t_error			segment_clean(void)
{
  /*
   * 1)
   */

  if (set_release(segment->container) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to release the segment container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  STATS_RELEASE(segment->stats);

  /*
   * 3)
   */

  free(segment);

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			segment_test(void)
{
  t_segid		segid;

  if (segment_reserve(kas, 42, PERM_EXEC, &segid) != ERROR_NONE)
    printf("error: segment_reserve()\n");

  /*
   * XXX continue tests
   */

  segment_dump();

  if (segment_flush(kas) != ERROR_NONE)
    printf("error: segment_flush()\n");
}
