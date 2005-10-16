/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri Oct 14 10:47:52 2005   mycure
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
 * XXX SEGMENT_ENTER et SEGMENT_LEAVE
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
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
      char		perms[4];

      if (set_object(segment->container, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "set: cannot find the segment object "
		   "corresponding to its identifier\n");

	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	}

      memset(perms, '.', 3);
      perms[3] = 0;

      if (data->perms & PERM_READ)
	perms[0] = 'r';

      if (data->perms & PERM_WRITE)
	perms[1] = 'w';

      if (data->perms & PERM_EXEC)
	perms[2] = 'x';

      cons_msg('#', "  [%02qd] 0x%08x - 0x%08x %s (%u bytes)\n",
	       data->segid,
	       data->address,
	       data->address + data->size,
	       perms,
	       data->size);
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
 *   A) gets the first segment.
 *   B) tries to find space before the first segment.
 *   C) for each segment, tries to find space after it.
 *   D) gets the last segment.
 *   E) tries to find space after the last segment.
 */

t_error			segment_rsv(t_asid			asid,
				    t_psize			size,
				    t_perms			perms,
				    t_segid*			segid)
{
  o_segment*		current;
  t_state		state;
  o_segment*		head;
  o_segment*		tail;
  t_iterator		i;
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
	/*
	 * A)
	 */

	if (set_head(segment->container, &i) != ERROR_NONE)
	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	if (set_object(segment->container, i, (void**)&head) != ERROR_NONE)
	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	/*
	 * B)
	 */

	if ((head->address - segment->start) >= size)
	  {
	    o_segment	o;

	    memset(&o, 0x0, sizeof(o_segment));

	    if (id_rsv(&segment->id, &o.segid) != ERROR_NONE)
	      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	    o.address = head->address + head->size;
	    o.size = size;
	    o.perms = perms;

	    if (set_insert_after(segment->container, i, &o) != ERROR_NONE)
	      {
		id_rel(&segment->id, o.segid);

		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	      }

	    if (set_add(as->segments, &o.segid) != ERROR_NONE)
	      {
		id_rel(&segment->id, o.segid);

		set_remove(segment->container, o.segid);

		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	      }

	    SEGMENT_LEAVE(segment, ERROR_NONE);
	  }

	/*
	 * C)
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

		if (id_rsv(&segment->id, &o.segid) != ERROR_NONE)
		  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		o.address = current->address + current->size;
		o.size = size;
		o.perms = perms;

		if (set_insert_after(segment->container, i, &o) != ERROR_NONE)
		  {
		    id_rel(&segment->id, o.segid);

		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
		  }

		if (set_add(as->segments, &o.segid) != ERROR_NONE)
		  {
		    id_rel(&segment->id, o.segid);

		    set_remove(segment->container, o.segid);

		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
		  }

		SEGMENT_LEAVE(segment, ERROR_NONE);
	      }
	  }

	/*
	 * D)
	 */

	if (set_tail(segment->container, &i) != ERROR_NONE)
	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	if (set_object(segment->container, i, (void**)&tail) != ERROR_NONE)
	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	/*
	 * E)
	 */

	if (((segment->start + segment->size) -
	     (tail->address + tail->size)) >= size)
	  {
	    o_segment	o;

	    memset(&o, 0x0, sizeof(o_segment));

	    if (id_rsv(&segment->id, &o.segid) != ERROR_NONE)
	      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	    o.address = tail->address + tail->size;
	    o.size = size;
	    o.perms = perms;

	    if (set_insert_after(segment->container, i, &o) != ERROR_NONE)
	      {
		id_rel(&segment->id, o.segid);

		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	      }

	    if (set_add(as->segments, &o.segid) != ERROR_NONE)
	      {
		id_rel(&segment->id, o.segid);

		set_remove(segment->container, o.segid);

		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
	      }

	    SEGMENT_LEAVE(segment, ERROR_NONE);
	  }
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

t_error			segment_rel(t_asid			asid,
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
   * 3)
   */

  if (set_remove(as->segments, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (set_remove(segment->container, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

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

  o->perms = perms;

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

      cons_msg('#', "  %qd\n",
	       data->segid == ID_UNUSED ? -1 : data->segid);

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
 * 3) initialises the identifier object to be able to generate
 *    the segment identifiers.
 * 4) reserves the segment set which will contain the system's segments.
 * 5) for each pre-reserved segment, inserts it into the segment container.
 * 6) tries to reserve a statistics object.
 * 7) if needed, dumps the segments.
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

  if (id_build(&segment->id) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (set_rsv(ll, SET_OPT_ALLOC, sizeof(o_segment),
	      &segment->container) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to reserve the segment container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  for (i = 0; i < init->nsegments; i++)
    {
      if (id_rsv(&segment->id, &init->segments[i].segid) != ERROR_NONE)
	{
	  cons_msg('!', "segment: unable to generate a segment identifier "
		   "for the pre-reserved segments\n");

	  return (ERROR_UNKNOWN);
	}

      if (set_insert_tail(segment->container, &init->segments[i]) !=
	  ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot add a pre-reserved segment in "
		   "the segment container\n");

	  return (ERROR_UNKNOWN);
	}
    }

  /*
   * 6)
   */

  STATS_RSV("segment", &segment->stats);

  /*
   * 7)
   */

#if (DEBUG & DEBUG_SEGMENT)
  segment_dump();

  segment_test();
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
 * 3) destroys the id object.
 * 4) frees the segment manager structure's memory.
 */

t_error			segment_clean(void)
{
  /*
   * 1)
   */

  STATS_REL(segment->stats);

  /*
   * 2)
   */

  if (set_rel(segment->container) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to release the segment container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (id_destroy(&segment->id) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
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

  if (segment_rsv(kas, PAGESZ, PERM_EXEC, &segid) != ERROR_NONE)
    printf("error: segment_rsv()\n");

  if (segment_rel(kas, 01) != ERROR_NONE)
    printf("error: segment_rel()\n");

  if (segment_rel(kas, 03) != ERROR_NONE)
    printf("error: segment_rel()\n");

  /*
   * XXX continue tests
   */

  segment_dump();

  if (segment_flush(kas) != ERROR_NONE)
    printf("error: segment_flush()\n");
}
