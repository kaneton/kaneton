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

#include <kaneton.h>

machine_include(segment);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager structure.
 */

m_segment*		_segment = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

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
 *
 *
 */

t_error			segment_show(i_segment			segid)
{
  char			perms[4];
  char*			type;
  o_segment*		o;

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  module_call(console, console_message,
	      '#', "  segment %qd in address space %qd:\n",
	      segid,
	      o->as);

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
    case SEGMENT_TYPE_SYSTEM:
      type = "system";
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

  if (o->permissions & PERMISSION_READ)
    perms[0] = 'r';

  if (o->permissions & PERMISSION_WRITE)
    perms[1] = 'w';

  if (o->permissions & PERMISSION_EXEC)
    perms[2] = 'x';

  /*
   * 4)
   */

  module_call(console, console_message,
	      '#', "    [%s] 0x%08x - 0x%08x %s (%u bytes)\n",
	      type,
	      o->address,
	      o->address + o->size,
	      perms,
	      o->size);

  /*
   * 5)
   */

  if (machine_call(segment, segment_show, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_dump(void)
{
  t_state		state;
  o_segment*		data;
  t_setsz		size;
  t_iterator		i;

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (set_size(_segment->segments, &size) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu segment(s) from the segment set:\n",
	      size);

  /*
   * 3)
   */

  set_foreach(SET_OPTION_FORWARD, _segment->segments, &i, state)
    {
      if (set_object(_segment->segments, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "segment: cannot find the segment object "
		      "corresponding to its identifier\n");

	  SEGMENT_LEAVE(_segment, ERROR_KO);
	}

      if (segment_show(data->id) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);
    }

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_clone(i_as			asid,
				      i_segment			old,
				      i_segment*		new)
{
  o_segment*		from;
  t_permissions		perms;

  SEGMENT_ENTER(_segment);

  assert(new != NULL);

  /*
   * 1)
   */

  if (segment_get(old, &from) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  assert(from->type != SEGMENT_TYPE_SYSTEM);

  /*
   * 2)
   */

  if (segment_reserve(asid, from->size, PERMISSION_WRITE, new) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  perms = from->permissions;

  if (!(perms & PERMISSION_READ))
    {
      if (segment_permissions(old, PERMISSION_READ) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);

      if (segment_copy(*new, 0, old, 0, from->size) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);

      if (segment_permissions(old, perms) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);
    }
  else
    if (segment_copy(*new, 0, old, 0, from->size) != ERROR_OK)
      SEGMENT_LEAVE(_segment, ERROR_KO);

  if (segment_permissions(*new, perms) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 4)
   */

  if (machine_call(segment, segment_clone, asid, old, new) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_inject(i_as		asid,
				       o_segment*	o,
				       i_segment*	segid)
{
  o_as*			as;

  SEGMENT_ENTER(_segment);

  assert(o != NULL);
  assert(o->type == SEGMENT_TYPE_MEMORY ||
	 o->type == SEGMENT_TYPE_CATCH ||
	 o->type == SEGMENT_TYPE_SYSTEM);
  assert(o->size != 0);
  assert((o->permissions & PERMISSION_INVALID) == 0);
  assert(segid != NULL);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  o->id = (i_segment)o->address;
  o->as = asid;
  *segid = o->id;

  /*
   * 3)
   */

  if (set_add(_segment->segments, o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (set_add(as->segments, &o->id) != ERROR_OK)
    {
      set_remove(_segment->segments, o->id);

      SEGMENT_LEAVE(_segment, ERROR_KO);
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_inject, asid, o, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (as_get(asid, &dest) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  if (as_get(o->as, &src) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 4)
   */

  if (set_remove(src->segments, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 5)
   */

  o->as = asid;
  if (set_add(dest->segments, &segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 6)
   */

  if (machine_call(segment, segment_give, asid, segid) !=
      ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *     and release previous one.
 * 3) otherwise, simply change the size field.
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
  t_permissions		perms;

  SEGMENT_ENTER(_segment);

  assert(size != 0);
  assert(new != NULL);

  /*
   * 1)
   */

  if (set_locate(_segment->segments, old, &it) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (set_object(_segment->segments, it, (void**)&o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (size > o->size)
    {

      /*
       * a)
       */

      if (set_next(_segment->segments, it, &next) == ERROR_OK)
	{
	  if (set_object(_segment->segments, next, (void**)&onext) !=
	      ERROR_OK)
	    SEGMENT_LEAVE(_segment, ERROR_KO);

	  /*
	   * b)
	   */

	  if (o->address + size > onext->address)
	    {
/*	      if (options & SEGLENT_OPTION_NORELOCATE)
		SEGMENT_LEAVE(_segment, ERROR_KO); XXX */

	      perms = o->permissions;

	      if (segment_reserve(o->as, size, PERMISSION_WRITE, new) !=
		  ERROR_OK)
		SEGMENT_LEAVE(_segment, ERROR_KO);

	      if (segment_permissions(old, PERMISSION_READ) != ERROR_OK)
		SEGMENT_LEAVE(_segment, ERROR_KO);

	      if (segment_copy(*new, 0, old, 0, o->size) != ERROR_OK)
		SEGMENT_LEAVE(_segment, ERROR_KO);

	      if (segment_permissions(*new, perms) != ERROR_OK)
		SEGMENT_LEAVE(_segment, ERROR_KO);

	      if (segment_release(old) != ERROR_OK)
		SEGMENT_LEAVE(_segment, ERROR_KO);

	      changed = 1;
	    }
	}
      else
	{
	  if (o->address + size > _segment->start + _segment->size)
	    SEGMENT_LEAVE(_segment, ERROR_KO);

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

  if (machine_call(segment, segment_resize, old, size, new) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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

  SEGMENT_ENTER(_segment);

  assert(size != 0);
  assert(left != NULL);
  assert(right != NULL);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (as_get(o->as, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (size > o->size)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if ((n = malloc(sizeof (o_segment))) == NULL)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  n->size = o->size - size;
  o->size = size;
  *left = segid;

  /*
   * 3)
   */

  n->as = o->as;
  n->permissions = o->permissions;
  n->address = o->address + size;
  n->type = o->type;

  *right = n->id = (i_segment)n->address;

  if (segment_inject(o->as, n, &useless) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 4)
   */

  if (machine_call(segment, segment_split, segid, size, left, right) !=
      ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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

  SEGMENT_ENTER(_segment);

  assert(left != right);
  assert(segid != NULL);

  /*
   * 1)
   */

  if (segment_get(left, &seg1) != ERROR_OK ||
      segment_get(right, &seg2) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (seg1->address + seg1->size != seg2->address ||
      seg1->as != seg2->as ||
      seg1->permissions != seg2->permissions ||
      seg1->type != seg2->type)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  seg1->size += seg2->size;
  *segid = seg1->id;

  /*
   * 4)
   */

  if (segment_release(right) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 5)
   */

  if (machine_call(segment,
		   segment_coalesce,
		   left,
		   right,
		   segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
}

/*
 * this function reads directly from a segment to a buffer.
 *
 * steps:
 *
 * 1) get segment object.
 * 2) check permissions and boundaries.
 * 3) call machine dependent code.
 *
 *
 */

t_error			segment_read(i_segment		segid,
				     t_paddr		offs,
				     void*		buff,
				     t_psize		sz)
{
  o_segment*		o;

  SEGMENT_ENTER(_segment);

  assert(sz != 0);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (!(o->permissions & PERMISSION_READ))
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (offs + sz > o->size)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  if (machine_call(segment, segment_read, segid, offs, buff, sz) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
}

/*
 * this function write directly to a segment from a buffer.
 *
 * steps:
 *
 * 1) get segment object.
 * 2) check permissions and boundaries.
 * 3) call the dependent code.
 *
 *
 */

t_error			segment_write(i_segment		segid,
				      t_paddr		offs,
				      const void*	buff,
				      t_psize		sz)
{
  o_segment*		o;

  SEGMENT_ENTER(_segment);

  assert(sz != 0);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (!(o->permissions & PERMISSION_WRITE))
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (offs + sz > o->size)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  if (machine_call(segment, segment_write, segid, offs, buff, sz) !=
      ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 * steps:
 *
 * 1) get segment objects.
 * 2) check permissions and boundaries.
 * 3) call machine dependent code.
 *
 *
 */

t_error			segment_copy(i_segment		dst,
				     t_paddr		offsd,
				     i_segment		src,
				     t_paddr		offss,
				     t_psize		sz)
{
  o_segment*		o1;
  o_segment*		o2;

  SEGMENT_ENTER(_segment);

  assert(sz != 0);

  /*
   * 1)
   */

  if (segment_get(dst, &o1) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (segment_get(src, &o2) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (!((o1->permissions & PERMISSION_WRITE) &&
	(o2->permissions & PERMISSION_READ)))
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (offsd + sz > o1->size)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (offss + sz > o2->size)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  if (machine_call(segment, segment_copy, dst, offsd, src, offss, sz) !=
      ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_reserve(i_as			asid,
					t_psize			size,
					t_permissions		perms,
					i_segment*		segid)
{
  o_as*			as;
  o_segment*		o;

  SEGMENT_ENTER(_segment);

  assert(size != 0);
  assert((perms & PERMISSION_INVALID) == 0);
  assert(segid != NULL);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if ((o = malloc(sizeof(o_segment))) == NULL)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (segment_space(asid, size, &o->address) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  o->as = as->id;
  o->size = size;
  o->permissions = perms;
  o->type = SEGMENT_TYPE_MEMORY;

  *segid = o->id = (i_segment)o->address;

  if (set_add(_segment->segments, o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  if (set_add(as->segments, &o->id) != ERROR_OK)
    {
      set_remove(_segment->segments, o->id);

      SEGMENT_LEAVE(_segment, ERROR_KO);
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_reserve, asid, size, perms, segid) !=
      ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_release(i_segment		segid)
{
  o_as*			as;
  o_segment*		o;

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (machine_call(segment, segment_release, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  if (as_get(o->as, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 4)
   */

  if (set_remove(as->segments, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 5)
   */

  if (set_remove(_segment->segments, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
}

/*
 * XXX[this function should not exist anymore. the more general functionality
 *     consists in reserving protected objects including memory areas, IRQ
 *     lines etc.]
 *
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

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (as_get(asid, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  if (o->type != SEGMENT_TYPE_CATCH)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 4)
   */

  if (o->as != ID_UNUSED && as_get(o->as, &old) == ERROR_OK)
    {
      if (set_remove(old->segments, o->id) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);
    }

  /*
   * 5)
   */

  if (set_add(as->segments, &o->id) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 6)
   */

  o->type = SEGMENT_TYPE_MEMORY;
  o->as = asid;

  /*
   * 7)
   */

  if (machine_call(segment, segment_catch, asid, segid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_permissions(i_segment		segid,
					    t_permissions	perms)
{
  o_segment*		o;

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (!(perms & PERMISSION_EXEC) &&
      !(perms & PERMISSION_READ) &&
      !(perms & PERMISSION_WRITE))
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  o->permissions = perms;

  /*
   * 4)
   */

  if (machine_call(segment, segment_permissions, segid, perms) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if ((type != SEGMENT_TYPE_MEMORY) &&
      (type != SEGMENT_TYPE_CATCH) &&
      (type != SEGMENT_TYPE_SYSTEM))
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  o->type = type;

  /*
   * 4)
   */

  if (machine_call(segment, segment_type, segid, type) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 *
 */

t_error			segment_flush(i_as			asid)
{
  i_segment*		data;
  o_as*			as;
  t_iterator		i;

  SEGMENT_ENTER(_segment);

  /*
   * 1)
   */

  if (machine_call(segment, segment_flush, asid) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 2)
   */

  if (as_get(asid, &as) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  /*
   * 3)
   */

  while (set_head(as->segments, &i) == ERROR_OK)
    {
      if (set_object(as->segments, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "segment: cannot find the object "
		      "corresponding to its identifier\n");

	  SEGMENT_LEAVE(_segment, ERROR_KO);
	}

      if (segment_release(*data) != ERROR_OK)
	SEGMENT_LEAVE(_segment, ERROR_KO);
    }

 SEGMENT_LEAVE(_segment, ERROR_OK);
}

/*
 * this function returns a segment object.
 *
 */

t_error			segment_get(i_segment			segid,
				    o_segment**			o)
{
  SEGMENT_ENTER(_segment);

  assert(o != NULL);

  if (set_get(_segment->segments, segid, (void**)o) != ERROR_OK)
    SEGMENT_LEAVE(_segment, ERROR_KO);

  SEGMENT_LEAVE(_segment, ERROR_OK);
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
 *
 */

t_error			segment_initialize(void)
{
  /*
   * 1)
   */

  if ((_segment = malloc(sizeof(m_segment))) == NULL)
    {
      module_call(console, console_message,
		  '!', "segment: cannot allocate memory for the segment "
		  "manager structure\n");

      return (ERROR_KO);
    }

  memset(_segment, 0x0, sizeof(m_segment));

  /*
   * 2)
   */

  _segment->start = _init->mem;
  _segment->size = _init->memsz;

  /*
   * 3)
   */

  if (set_reserve(bpt, SET_OPTION_SORT | SET_OPTION_FREE, sizeof(o_segment),
		  SEGMENT_BPT_NODESZ, &_segment->segments) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "segment: unable to reserve the segment set\n");

      return (ERROR_KO);
    }

  /*
   * 4)
   */

  if (machine_call(segment, segment_initialize) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}

/*
 * this function just reinitializes the segment manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the segment set.
 * 3) frees the segment manager structure's memory.
 *
 *
 */

t_error			segment_clean(void)
{
  o_segment*		data;
  t_iterator		i;

  /*
   * 1)
   */

  if (machine_call(segment, segment_clean) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  while (set_head(_segment->segments, &i) == ERROR_OK)
    {
      if (set_object(_segment->segments, i, (void**)&data) != ERROR_OK)
	{
	  module_call(console, console_message,
		      '!', "segment: cannot find the object "
		      "corresponding to its identifier\n");

	  return (ERROR_KO);
	}

      if (segment_release(data->id) != ERROR_OK)
	return (ERROR_KO);
    }

  if (set_release(_segment->segments) != ERROR_OK)
    {
      module_call(console, console_message,
		  '!', "segment: unable to release the segment set\n");

      return (ERROR_KO);
    }

  /*
   * 3)
   */

  free(_segment);

  return (ERROR_OK);
}
