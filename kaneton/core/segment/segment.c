/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/segment/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [fri may 25 11:33:16 2007]
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
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

machine_include(segment);

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

/*                                                                  [cut] k1 */

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

t_error			segment_show(i_segment			segid)
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

  if (machine_call(segment, segment_show, segid) != ERROR_NONE)
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

  if (set_size(segment->segments, &size) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu segment(s) from the segment set:\n", size);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, segment->segments, &i, state)
    {
      if (set_object(segment->segments, i, (void**)&data) != ERROR_NONE)
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
 * 1) get the original segment object.
 * 2) reserve a new segment of same size with same permissions.
 * 3) copy the data from the old segment.
 * 4) call machine-dependent code.
 */

t_error			segment_clone(i_as			asid,
				      i_segment			old,
				      i_segment*		new)
{
  o_segment*		from;
  t_perms		perms;

  SEGMENT_ENTER(segment);

  ASSERT(new != NULL);

  /*
   * 1)
   */

  if (segment_get(old, &from) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_reserve(asid, from->size, PERM_WRITE, new) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  perms = from->perms;

  if (segment_perms(old, PERM_READ) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (segment_copy(*new, 0, old, 0, from->size) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (segment_perms(old, perms) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (segment_perms(*new, perms) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machine_call(segment, segment_clone, asid, old, new) != ERROR_NONE)
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

t_error			segment_inject(i_as		asid,
				       o_segment*	o,
				       i_segment*	segid)
{
  o_as			*as;

  SEGMENT_ENTER(segment);

  ASSERT(o != NULL);
  ASSERT(o->type == SEGMENT_TYPE_MEMORY || o->type == SEGMENT_TYPE_CATCH);
  ASSERT(o->size != 0);
  ASSERT((o->perms & PERM_INVALID) == 0);
  ASSERT(segid != NULL);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  o->segid = (i_segment)o->address;
  o->asid = asid;
  *segid = o->segid;

  /*
   * 3)
   */

  if (set_add(segment->segments, o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_add(as->segments, &o->segid) != ERROR_NONE)
    {
      set_remove(segment->segments, o->segid);

      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_inject, asid, o, segid) != ERROR_NONE)
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

t_error			segment_give(i_as		asid,
				     i_segment		segid)
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

  if (machine_call(segment, segment_give, asid, segid) !=
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

t_error			segment_resize(i_segment	old,
				       t_psize		size,
				       i_segment*	new)
{
  o_segment*		o;
  o_segment*		onext;
  t_iterator		it;
  t_iterator		next;
  t_uint8		changed = 0;
  t_perms		perms;

  SEGMENT_ENTER(segment);

  ASSERT(size != 0);
  ASSERT(new != NULL);

  /*
   * 1)
   */

  if (set_locate(segment->segments, old, &it) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_object(segment->segments, it, (void**)&o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (size > o->size)
    {

      /*
       * a)
       */

      if (set_next(segment->segments, it, &next) == ERROR_NONE)
	{
	  if (set_object(segment->segments, next, (void**)&onext) !=
	      ERROR_NONE)
	    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	  /*
	   * b)
	   */

	  if (o->address + size > onext->address)
	    {
/*	      if (opts & SEGLENT_OPT_NORELOCATE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN); XXX */

	      perms = o->perms;

	      if (segment_reserve(o->asid, size, PERM_WRITE, new) !=
		  ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (segment_perms(old, PERM_READ) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (segment_copy(*new, 0, old, 0, o->size) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (segment_perms(*new, perms) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (segment_release(old) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      changed = 1;
	    }
	}
      else
	{
	  if (o->address + size > segment->start + segment->size)
	    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	  /* XXX look for a new location ? */
	}
    }

  /*
   * 3)
   */

  if (!changed)
    {
      o->size = size;
      *new = old;
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_resize, old, size, new) != ERROR_NONE)
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

t_error			segment_split(i_segment		segid,
				      t_psize		size,
				      i_segment*	left,
				      i_segment*	right)
{
  o_as*			as;
  o_segment*		o;
  o_segment*		n;
  i_segment		useless;

  SEGMENT_ENTER(segment);

  ASSERT(size != 0);
  ASSERT(left != NULL);
  ASSERT(right != NULL);

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

  if (size > o->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if ((n = malloc(sizeof (o_segment))) == NULL)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  n->size = o->size - size;
  o->size = size;
  *left = segid;

  /*
   * 3)
   */

  n->asid = o->asid;
  n->perms = o->perms;
  n->address = o->address + size;
  n->type = o->type;

  *right = n->segid = (i_segment)n->address;

  if (segment_inject(o->asid, n, &useless) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (machine_call(segment, segment_split, segid, size, left, right) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function coalesce two adjacent segments.
 *
 * steps:
 *
 * 1) get the two segment objects.
 * 2) ensure the two segments are adjacent.
 * 3) enlarge the first one so it overlap with the second one.
 * 4) release the now useless second segment.
 * 5) call machine-dependent code.
 */

t_error			segment_coalesce(i_segment	left,
					 i_segment	right,
					 i_segment*	segid)
{
  o_segment*		seg1;
  o_segment*		seg2;

  SEGMENT_ENTER(segment);

  ASSERT(left != right);
  ASSERT(segid != NULL);

  /*
   * 1)
   */

  if (segment_get(left, &seg1) != ERROR_NONE ||
      segment_get(right, &seg2) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (seg1->address + seg1->size != seg2->address ||
      seg1->asid != seg2->asid ||
      seg1->perms != seg2->perms ||
      seg1->type != seg2->type)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  seg1->size += seg2->size;
  *segid = seg1->segid;

  /*
   * 4)
   */

  if (segment_release(right) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (machine_call(segment, segment_coalesce, left, right, segid) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function reads directly from a segment to a buffer.
 *
 * steps:
 *
 * 1) get segment object.
 * 2) check permissions and boundaries.
 * 3) call machine dependent code.
 */

t_error			segment_read(i_segment		segid,
				     t_paddr		offs,
				     void*		buff,
				     t_psize		sz)
{
  o_segment*		o;

  SEGMENT_ENTER(segment);

  ASSERT(sz != 0);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (!(o->perms & PERM_READ))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (offs + sz > o->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machine_call(segment, segment_read, segid, offs, buff, sz) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function write directly to a segment from a buffer.
 *
 * steps:
 *
 * 1) get segment object.
 * 2) check permissions and boundaries.
 * 3) call the dependent code.
 */

t_error			segment_write(i_segment		segid,
				      t_paddr		offs,
				      const void*	buff,
				      t_psize		sz)
{
  o_segment*		o;

  SEGMENT_ENTER(segment);

  ASSERT(sz != 0);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (!(o->perms & PERM_WRITE))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (offs + sz > o->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machine_call(segment, segment_write, segid, offs, buff, sz) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 * steps:
 *
 * 1) get segment objects.
 * 2) check permissions and boundaries.
 * 3) call machine dependent code.
 */

t_error			segment_copy(i_segment		dst,
				     t_paddr		offsd,
				     i_segment		src,
				     t_paddr		offss,
				     t_psize		sz)
{
  o_segment*		o1;
  o_segment*		o2;

  SEGMENT_ENTER(segment);

  ASSERT(sz != 0);

  /*
   * 1)
   */

  if (segment_get(dst, &o1) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (segment_get(src, &o2) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (!((o1->perms & PERM_WRITE) && (o2->perms & PERM_READ)))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (offsd + sz > o1->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (offss + sz > o2->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (machine_call(segment, segment_copy, dst, offsd, src, offss, sz) !=
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
 * 2) find some free space.
 * 3) builds the segment.
 * 4) calls the machine dependent code.
 */

t_error			segment_reserve(i_as			asid,
					t_psize			size,
					t_perms			perms,
					i_segment*		segid)
{
  o_as*			as;
  o_segment*		o;

  SEGMENT_ENTER(segment);

  ASSERT(size != 0);
  ASSERT((perms & PERM_INVALID) == 0);
  ASSERT(segid != NULL);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if ((o = malloc(sizeof(o_segment))) == NULL)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_space(asid, size, &o->address) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  o->asid = as->asid;
  o->size = size;
  o->perms = perms;
  o->type = SEGMENT_TYPE_MEMORY;

  *segid = o->segid = (i_segment)o->address;

  if (set_add(segment->segments, o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (set_add(as->segments, &o->segid) != ERROR_NONE)
    {
      set_remove(segment->segments, o->segid);

      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_reserve, asid, size, perms, segid) !=
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
 * 5) removes the segment from the segment set.
 */

t_error			segment_release(i_segment		segid)
{
  o_as*			as;
  o_segment*		o;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machine_call(segment, segment_release, segid) != ERROR_NONE)
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

  if (set_remove(segment->segments, segid) != ERROR_NONE)
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

t_error			segment_catch(i_as			asid,
				      i_segment			segid)
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

  if (machine_call(segment, segment_catch, asid, segid) != ERROR_NONE)
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

t_error			segment_perms(i_segment			segid,
				      t_perms			perms)
{
  o_segment*		o;

  SEGMENT_ENTER(segment);

  ASSERT((perms & PERM_INVALID) == 0);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (!(perms & PERM_EXEC) && !(perms & PERM_READ) && !(perms & PERM_WRITE))
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  o->perms = perms;

  /*
   * 4)
   */

  if (machine_call(segment, segment_perms, segid, perms) != ERROR_NONE)
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

t_error			segment_type(i_segment			segid,
				     t_type			type)
{
  o_segment*		o;

  SEGMENT_ENTER(segment);

  ASSERT(type == SEGMENT_TYPE_MEMORY || type == SEGMENT_TYPE_CATCH);

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

  if (machine_call(segment, segment_type, segid, type) != ERROR_NONE)
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
 *    the segment set to destroy it.
 */

t_error			segment_flush(i_as			asid)
{
  i_segment*		data;
  o_as*			as;
  t_iterator		i;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (machine_call(segment, segment_flush, asid) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  while (set_head(as->segments, &i) == ERROR_NONE)
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

t_error			segment_get(i_segment			segid,
				    o_segment**			o)
{
  SEGMENT_ENTER(segment);

  ASSERT(o != NULL);

  if (set_get(segment->segments, segid, (void**)o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function initializes the segment manager from the init
 * variable containing segments to keep safe.
 *
 * steps:
 *
 * 1) allocates and initializes the segment manager structure.
 * 2) initializes the segment manager structure fields from the init
 *    structure.
 * 3) reserves the segment set which will contain the system's segments.
 * 4) calls the machine-dependent code.
 */

t_error			segment_initialize(void)
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

  /*
   * 3)
   */

  if (set_reserve(bpt, SET_OPT_SORT | SET_OPT_FREE, sizeof(o_segment),
		  SEGMENT_BPT_NODESZ, &segment->segments) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to reserve the segment set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_initialize) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just reinitializes the segment manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the segment set.
 * 3) frees the segment manager structure's memory.
 */

t_error			segment_clean(void)
{
  o_segment*		data;
  t_iterator		i;

  /*
   * 1)
   */

  if (machine_call(segment, segment_clean) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  while (set_head(segment->segments, &i) == ERROR_NONE)
    {
      if (set_object(segment->segments, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "segment: cannot find the object "
		   "corresponding to its identifier\n");

	  return (ERROR_UNKNOWN);
	}

      if (segment_release(data->segid) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  if (set_release(segment->segments) != ERROR_NONE)
    {
      cons_msg('!', "segment: unable to release the segment set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  free(segment);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k1 */
