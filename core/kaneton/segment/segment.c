/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/segment/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [mon feb 20 11:59:34 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the segment manager manages physical memory.
 *
 * it  is  able   to  reserve  and  release  memory   areas  (see  the
 * segment-fit.c for  allocation algorithm)  and to operated  on these
 * area: resize, split, copy, etc.
 *
 * a segment  is identified by  a 64 bits identifier  corresponding to
 * its physical  address. so  it is easy  to retrieve  this identifier
 * given the base address of a segment.
 *
 * remember  that  segments are  global:  there  is  only one  set  of
 * segments objects for  the entire kernel. the set  of segments in an
 * address space is just a set of identifiers.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students have  to write the entire manager:  this file implementing
 * independent code plus  an architecture dependant file (ia32-virtual
 * or ia32-segment for example).
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

/*                                                                  [cut] k2 */

/*
 * this function shows a segment.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) compute the type string.
 * 3) compute the perms string.
 * 4) display the entry.
 * 5) call machine dependent code.
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

  /*
   * 5)
   */

  if (machdep_call(segment, segment_show, segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

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

/*
 * this function  resizes a  segment. if there  is not enough  room to
 * enlarge  the segment,  a new  one  is allocated  elsewhere and  the
 * previous one is cleared.
 *
 * steps:
 *
 * 1) search for the designated segment.
 * 2) if we are increasing the size.
 *  a) check for the next segment.
 *  b) if there is no room for the segment, create a new one, copy data
       and release previous one.
   3) otherwise, simply change the size field.
 * 4) call machine dependent code.
 */

t_error			segment_resize(t_segid		segid,
				       t_psize		new_size,
				       t_segid*		new_seg)
{
  o_segment*		o;
  o_segment*		onext;
  t_iterator		it;
  t_iterator		next;
  t_uint8		changed = 0;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (set_locate(segment->container, segid, &it) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_object(segment->container, it, (void**)&o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (new_size > o->size)
    {

      /*
       * a)
       */

      if (set_next(segment->container, it, &next) == ERROR_NONE)
	{
	  if (set_object(segment->container, next, (void**)&onext) !=
	      ERROR_NONE)
	    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	  /*
	   * b)
	   */

	  if (o->address + new_size >= onext->address)
	    {
	      if (segment_reserve(o->asid, new_size, o->perms, new_seg) !=
		  ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (segment_copy(*new_seg, 0, segid, 0, o->size) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (segment_release(segid) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      changed = 1;
	    }
	}
    }

  /*
   * 3)
   */

  if (!changed)
    {
      o->size = new_size;
      *new_seg = segid;
    }

  /*
   * 4)
   */

  if (machdep_call(segment, segment_resize, segid, new_size) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function splits a segment into two new segments.
 *
 * steps:
 *
 * 1) get address space and segment objects.
 * 2) check for split point and cut first segment.
 * 3) create the second segment.
 * 4) call machine-dependent code.
 */

t_error			segment_split(t_segid		segid,
				      t_psize		sz1,
				      t_segid*		s1,
				      t_segid*		s2)
{
  o_as*			as;
  o_segment*		o;
  o_segment		n;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (as_get(o->asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (sz1 >= o->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  o->size = sz1;
  *s1 = segid;

  /*
   * 3)
   */

  n.asid = o->asid;
  n.size = o->size - sz1;
  n.perms = o->perms;
  n.address = o->address + sz1;

  *s2 = n.segid = (t_segid)n.address;

  if (segment_inject(&n, o->asid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machdep_call(segment, segment_split, segid, sz1, s1, s2) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function coalesce two adjacent segments.
 *
 * steps:
 *
 * 1) swap the two segments if necessary so the s1 is the lower address segment
 * 2) get the two segment objects.
 * 3) ensure the two segments are adjacent.
 * 4) enlarge the first one so it overlap with the second one.
 * 5) release the now useless second segment.
 * 6) call machine-dependent code.
 */

t_error			segment_coalesce(t_segid	s1,
					 t_segid	s2,
					 t_segid*	new_seg)
{
  o_segment*		seg1;
  o_segment*		seg2;
  t_segid		tmp;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (s2 < s1)
    {
      tmp = s1;
      s1 = s2;
      s2 = tmp;
    }

  /*
   * 2)
   */

  if (segment_get(s1, &seg1) != ERROR_NONE ||
      segment_get(s2, &seg1) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (seg1->address + seg1->size != seg2->address)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  seg1->size += seg2->size;

  /*
   * 5)
   */

  if (segment_release(s2) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (machdep_call(segment, segment_coalesce, s1, s2, new_seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function reads directly from a segment to a buffer.
 *
 * steps:
 *
 * 1) call machine dependent code.
 */

t_error			segment_read(t_segid		segid,
				     t_paddr		offs,
				     void*		buff,
				     t_psize		sz)
{
  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machdep_call(segment, segment_read, segid, offs, buff, sz) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function write directly to a segment from a buffer.
 *
 * steps:
 *
 * 1) call the dependent code.
 */

t_error			segment_write(t_segid		segid,
				      t_paddr		offs,
				      const void*	buff,
				      t_psize		sz)
{
  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machdep_call(segment, segment_write, segid, offs, buff, sz) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 * steps:
 *
 * 1) call machine dependent code.
 */

t_error			segment_copy(t_segid		dst,
				     t_paddr		offsd,
				     t_segid		src,
				     t_paddr		offss,
				     t_psize		sz)
{
  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machdep_call(segment, segment_copy, dst, offsd, src, offss, sz) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
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
 * 1) get the address space object.
 * 2) get the segment object.
 * 3) check the segment type.
 * 4) remove the segment from the previous as.
 * 5) finally add the segment into the address space object.
 * 6) mark the segment as classical memory segment and update the asid.
 * 7) call the machine-dependent code.
 */

t_error			segment_catch(t_asid			asid,
				      t_segid			segid)
{
  o_as*                 as;
  o_as*			old;
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

  if (o->asid != ID_UNUSED && as_get(o->asid, &old) == ERROR_NONE)
    {
      if (set_remove(old->segments, o->segid) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (set_add(as->segments, &o->segid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 6)
   */

  o->type = SEGMENT_TYPE_MEMORY;
  o->asid = asid;

  /*
   * 7)
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

/*                                                                 [cut] /k2 */
