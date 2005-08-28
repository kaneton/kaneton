/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/stats
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Aug 28 15:30:56 2005   mycure
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
 * 1) checks whether the segment manager was previously initialised.
 * 2) gets the size of the segment set.
 * 3) prints the header message.
 * 4) for each entry in the segment set, prints the area,
 *    its size and permissions.
 */

int			segment_dump(void)
{
  o_segment*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  segment_check(segment);

  /*
   * 2)
   */

  if (set_size(segment->container, &size) != 0)
    return (-1);

  /*
   * 3)
   */

  cons_msg('#', "dumping %qu segment(s) from the segment container:\n", size);

  /*
   * 4)
   */

  set_foreach(SET_OPT_FORWARD, segment->container, &i)
    {
      char		perms[4];

      if (set_object(segment->container, i, (void**)&data) != 0)
	{
	  cons_msg('!', "set: cannot find the segment object "
		   "corresponding to its identifier\n");

	  return (-1);
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

  return (0);
}

/*
 * this function reserves a segment given the desired size.
 *
 * steps:
 *
 * 1) checks whether the segment manager is initialised.
 * 2) gets the address space object given its identifier.
 * 3) chooses the correct fit.
 *   A) gets the first segment.
 *   B) tries to find space before the first segment.
 *   C) for each segment, tries to find space after it.
 *   D) gets the last segment.
 *   E) tries to find space after the last segment.
 */

int			segment_rsv(t_asid			asid,
				    t_psize			size,
				    t_perms			perms,
				    t_segid*			segid)
{
  o_segment*		head;
  o_segment*		tail;
  o_segment*		current;
  t_iterator		i;
  o_as*			as;

  /*
   * 1)
   */

  segment_check(segment);

  /*
   * 2)
   */

  if (as_get(asid, &as) != 0)
    return (-1);

  /*
   * 3)
   */

  switch (segment->fit)
    {
    case SEGMENT_FIT_FIRST:
      {
	/*
	 * A)
	 */

	if (set_head(segment->container, &i) != 0)
	  return (-1);

	if (set_object(segment->container, i, (void**)&head) != 0)
	  return (-1);

	/*
	 * B)
	 */

	if ((head->address - segment->start) >= size)
	  {
	    o_segment	o;

	    memset(&o, 0x0, sizeof(o_segment));

	    if (id_rsv(&segment->id, &o.segid) != 0)
	      return (-1);

	    o.address = head->address + head->size;
	    o.size = size;
	    o.perms = perms;

	    if (set_insert_after(segment->container, i, &o) != 0)
	      {
		id_rel(&segment->id, o.segid);

		return (-1);
	      }

	    if (set_add(as->segments, &o.segid) != 0)
	      {
		id_rel(&segment->id, o.segid);

		set_remove(segment->container, o.segid);
	      }

	    return (0);
	  }

	/*
	 * C)
	 */

	set_foreach(SET_OPT_FORWARD, segment->container, &i)
	  {
	    o_segment*	next;
	    t_iterator	j;

	    if (set_object(segment->container, i, (void**)&current) != 0)
	      {
		cons_msg('!', "set: cannot find the segment object "
			 "corresponding to its identifier\n");

		return (-1);
	      }

	    if (set_next(segment->container, i, &j) != 0)
	      break;

	    if (set_object(segment->container, j, (void**)&next) != 0)
	      return (-1);

	    if ((next->address - (current->address + current->size)) >= size)
	      {
		o_segment o;

		memset(&o, 0x0, sizeof(o_segment));

		if (id_rsv(&segment->id, &o.segid) != 0)
		  return (-1);

		o.address = current->address + current->size;
		o.size = size;
		o.perms = perms;

		if (set_insert_after(segment->container, i, &o) != 0)
		  {
		    id_rel(&segment->id, o.segid);

		    return (-1);
		  }

		if (set_add(as->segments, &o.segid) != 0)
		  {
		    id_rel(&segment->id, o.segid);

		    set_remove(segment->container, o.segid);
		  }

		return (0);
	      }
	  }

	/*
	 * D)
	 */

	if (set_tail(segment->container, &i) != 0)
	  return (-1);

	if (set_object(segment->container, i, (void**)&tail) != 0)
	  return (-1);

	/*
	 * E)
	 */

	if (((segment->start + segment->size) -
	     (tail->address + tail->size)) >= size)
	  {
	    o_segment	o;

	    memset(&o, 0x0, sizeof(o_segment));

	    if (id_rsv(&segment->id, &o.segid) != 0)
	      return (-1);

	    o.address = tail->address + tail->size;
	    o.size = size;
	    o.perms = perms;

	    if (set_insert_after(segment->container, i, &o) != 0)
	      {
		id_rel(&segment->id, o.segid);

		return (-1);
	      }

	    if (set_add(as->segments, &o.segid) != 0)
	      {
		id_rel(&segment->id, o.segid);

		set_remove(segment->container, o.segid);
	      }

	    return (0);
	  }
      }
    default:
      {
	return (-1);
      }
    }

  return (-1);
}

/*
 * this function releases a segment.
 *
 * steps:
 *
 * 1) checks whether the segment manager was previously initialised.
 * 2) gets the as object from its identifier.
 * 3) removes the segment from the address space.
 * 4) removes the segment from the segment container.
 */

int			segment_rel(t_asid			asid,
				    t_segid			segid)
{
  o_as*			as;

  /*
   * 1)
   */

  segment_check(segment);

  /*
   * 2)
   */

  if (as_get(asid, &as) != 0)
    return (-1);

  /*
   * 3)
   */

  if (set_remove(as->segments, segid) != 0)
    return (-1);

  /*
   * 4)
   */

  if (set_remove(segment->container, segid) != 0)
    return (-1);

  return (0);
}

/*
 * this function sets the permissions of a segment.
 */

int			segment_perms(t_asid			asid,
				      t_segid			segid,
				      t_perms			perms)
{
  o_as*			as;
  t_iterator		i;
  o_segment*		o;

  /*
   * 1)
   */

  segment_check(segment);

  /*
   * 2)
   */

  if (as_get(asid, &as) != 0)
    return (-1);

  /*
   * 3)
   */

  if (set_locate(as->segments, segid, &i) != 0)
    return (-1);

  /*
   * 4)
   */

  if (segment_get(segid, &o) != 0)
    return (-1);

  /*
   * 5)
   */

  o->perms = perms;

  return (0);
}

/*
 * XXX
 */

int			segment_flush(t_asid			asid)
{
  o_segment*		data;
  o_as*			as;
  t_iterator		i;

  /*
   * 1)
   */

  segment_check(segment);

  /*
   * 2)
   */

  if (as_get(asid, &as) != 0)
    return (-1);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, as->segments, &i)
    {
      if (set_object(as->segments, i, (void**)&data) != 0)
	{
	  cons_msg('!', "segment: cannot find the object "
		   "corresponding to its identifier\n");

	  return (-1);
	}

      cons_msg('#', "  %qd\n",
	       data->segid == ID_UNUSED ? -1 : data->segid);

      if (set_remove(segment->container, data->segid) != 0)
	{
	  cons_msg('!', "segment: cannot remove the previously found "
		   "object\n");

	  return (-1);
	}
    }

  /*
   * 4)
   */

  if (set_flush(as->segments) != 0)
    return (-1);

  return (0);
}

/*
 * XXX
 */

int			segment_get(t_segid			segid,
				    o_segment**			o)
{
  /*
   * 1)
   */

  segment_check(segment);

  /*
   * 2)
   */

  if (set_get(segment->container, segid, (void**)o) != 0)
    return (-1);

  return (0);
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
 * 6) if needed, dumps the segments.
 */

int			segment_init(t_fit			fit)
{
  t_uint32		i;

  /*
   * 1)
   */

  if ((segment = malloc(sizeof(m_segment))) == NULL)
    {
      cons_msg('!', "segment: cannot allocate memory for the segment "
	       "manager structure\n");

      return (-1);
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

  if (id_build(&segment->id) != 0)
    {
      cons_msg('!', "segment: unable to initialise the identifier object\n");

      return (-1);
    }

  /*
   * 4)
   */

  if (set_rsv(ll, SET_OPT_ALLOC, sizeof(o_segment), &segment->container) != 0)
    {
      cons_msg('!', "segment: unable to reserve the segment container\n");

      return (-1);
    }

  /*
   * 5)
   */

  for (i = 0; i < init->nsegments; i++)
    {
      if (id_rsv(&segment->id, &init->segments[i].segid) != 0)
	{
	  cons_msg('!', "segment: unable to generate a segment identifier "
		   "for the pre-reserved segments\n");

	  return (-1);
	}

      if (set_insert_tail(segment->container, &init->segments[i]) != 0)
	{
	  cons_msg('!', "segment: cannot add a pre-reserved segment in "
		   "the segment container\n");

	  return (-1);
	}
    }

  /*
   * 6)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_SEGMENT)
  segment_dump();
#endif

#if 1
 {
   t_segid	segid;

   if (segment_rsv(kas, PAGESZ, PERM_EXEC, &segid) != 0)
     printf("error: segment_rsv()\n");

   if (segment_rel(kas, 01) != 0)
     printf("error: segment_rel()\n");

   if (segment_rel(kas, 03) != 0)
     printf("error: segment_rel()\n");

   segment_dump();

   if (segment_flush(kas) != 0)
     printf("error: segment_flush()\n");
 }
#endif

  return (0);
}

/*
 * this function just reinitialises the segment manager.
 *
 * steps:
 *
 * 1) releases the segment container.
 * 2) destroys the id object.
 * 3) frees the segment manager structure's memory.
 */

int			segment_clean(void)
{
  /*
   * 1)
   */

  if (set_rel(segment->container) != 0)
    {
      cons_msg('!', "segment: unable to release the segment container\n");

      return (-1);
    }

  /*
   * 2)
   */

  if (id_destroy(&segment->id) != 0)
    {
      cons_msg('!', "segment: unable to destroy the identifier object\n");

      return (-1);
    }

  /*
   * 3)
   */

  free(segment);

  return (0);
}
