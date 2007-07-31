/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/as/as.c
 *
 * created       julien quintard   [tue dec 13 03:05:27 2005]
 * updated       matthieu bucchianeri   [tue apr  3 14:53:38 2007]
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

#include <libc.h>
#include <kaneton.h>

machine_include(as);

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

  if (machine_call(as, as_show, asid) != ERROR_NONE)
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

  if (machine_call(as, as_give, tskid, asid) != ERROR_NONE)
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

  ASSERT(virtual != NULL);

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

  if (machine_call(as, as_vaddr, asid, physical, virtual) !=
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

  ASSERT(physical != NULL);

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

  if (machine_call(as, as_paddr, asid, virtual, physical) !=
      ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * read data from an as. this function supports contiguous regions
 * mapped on non-contiguous segments.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) get the first region of the as.
 * 3) look for the first region to copy.
 * 4) compute the offset and size.
 * 5) read data from the segment.
 * 6) loop throught the remaining regions (if needed).
 * 7) get the next region (contiguously).
 * 8) compute the offset and size.
 * 9) read data from the segment.
 */

t_error			as_read(i_as				asid,
				t_vaddr				src,
				t_vsize				size,
				void*				dst)
{
  o_as*			o;
  o_region*		oreg;
  o_region*		prev;
  t_iterator		it;
  t_iterator		next;
  t_paddr		offs;
  t_psize		copy;
  t_uint8*		buff = dst;

  AS_ENTER(as);

  ASSERT(size != 0);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_head(o->regions, &it) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_object(o->regions, it, (void**)&oreg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  while (oreg->address + oreg->size <= src)
    {
      if (set_next(o->regions, it, &next) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (set_object(o->regions, next, (void**)&oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      memcpy(&it, &next, sizeof (t_iterator));
    }

  if (oreg->address > src)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  offs = src - oreg->address;
  copy = oreg->size - offs;
  if (copy > size)
    copy = size;
  offs += oreg->offset;

  /*
   * 5)
   */

  if (segment_read(oreg->segid, offs, buff, copy) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  buff += copy;
  size -= copy;

  if (size == 0)
    AS_LEAVE(as, ERROR_NONE);

  /*
   * 6)
   */

  for (;;)
    {
      prev = oreg;

      /*
       * 7)
       */

      if (set_next(o->regions, next, &it) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (set_object(o->regions, it, (void**)&oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (oreg->address != prev->address + prev->size)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * 8)
       */

      offs = oreg->offset;
      copy = oreg->size;
      if (copy > size)
	copy = size;

      /*
       * 9)
       */

      if (segment_read(oreg->segid, offs, buff, copy) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      buff += copy;
      size -= copy;

      if (size == 0)
	break;

      memcpy(&next, &it, sizeof (t_iterator));
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * write data to an as. this function supports contiguous regions
 * mapped on non-contiguous segments.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) get the first region of the as.
 * 3) look for the first region to copy.
 * 4) compute the offset and size.
 * 5) write data to the segment.
 * 6) loop throught the remaining regions (if needed).
 * 7) get the next region (contiguously).
 * 8) compute the offset and size.
 * 9) write data to the segment.
 */

t_error			as_write(i_as				asid,
				 void*				src,
				 t_vsize			size,
				 t_vaddr			dst)
{
  o_as*			o;
  o_region*		oreg;
  o_region*		prev;
  t_iterator		it;
  t_iterator		next;
  t_paddr		offs;
  t_psize		copy;
  t_uint8*		buff = src;

  AS_ENTER(as);

  ASSERT(size != 0);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_head(o->regions, &it) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_object(o->regions, it, (void**)&oreg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  while (oreg->address + oreg->size <= dst)
    {
      if (set_next(o->regions, it, &next) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (set_object(o->regions, next, (void**)&oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      memcpy(&it, &next, sizeof (t_iterator));
    }

  if (oreg->address > dst)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  offs = dst - oreg->address;
  copy = oreg->size - offs;
  if (copy > size)
    copy = size;
  offs += oreg->offset;

  /*
   * 5)
   */

  if (segment_write(oreg->segid, offs, buff, copy) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  buff += copy;
  size -= copy;

  if (size == 0)
    AS_LEAVE(as, ERROR_NONE);

  /*
   * 6)
   */

  for (;;)
    {
      prev = oreg;

      /*
       * 7)
       */

      if (set_next(o->regions, next, &it) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (set_object(o->regions, it, (void**)&oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (oreg->address != prev->address + prev->size)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * 8)
       */

      offs = oreg->offset;
      copy = oreg->size;
      if (copy > size)
	copy = size;

      /*
       * 9)
       */

      if (segment_write(oreg->segid, offs, buff, copy) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      buff += copy;
      size -= copy;

      if (size == 0)
	break;

      memcpy(&next, &it, sizeof (t_iterator));
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function copies from an as to another. this function supports
 * contiguous regions mapped on non-contiguous segments.
 *
 * steps:
 *
 * 1) get address space object.
 * 2) look for the first region of the source area.
 * 3) look for the first region of the destination area.
 * 4) compute start offsets and size to copy.
 * 5) do the copy.
 * 6) goto the next source region if necessary.
 * 7) goto the next destination region if necessary.
 */

t_error			as_copy(i_as				src_as,
				t_vaddr				src,
				i_as				dst_as,
				t_vaddr				dst,
				t_vsize				size)
{
  o_as*			src_o;
  o_region*		src_oreg;
  o_segment*		src_oseg;
  o_as*			dst_o;
  o_region*		dst_oreg;
  o_segment*		dst_oseg;
  t_iterator		src_it;
  t_iterator		src_next;
  t_iterator		dst_it;
  t_iterator		dst_next;
  t_paddr		src_offs;
  t_paddr		dst_offs;
  t_psize		copy;
  t_psize		to_copy_src;
  t_psize		to_copy_dst;
  o_region*		prev;
  t_iterator		next;

  AS_ENTER(as);

  ASSERT(size != 0);
  ASSERT(src_as != dst_as);

  /*
   * 1)
   */

  if (as_get(src_as, &src_o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (as_get(dst_as, &dst_o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (set_head(src_o->regions, &src_it) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_object(src_o->regions, src_it, (void**)&src_oreg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  while (src_oreg->address + src_oreg->size <= src)
    {
      if (set_next(src_o->regions, src_it, &src_next) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (set_object(src_o->regions, src_next, (void**)&src_oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      memcpy(&src_it, &src_next, sizeof (t_iterator));
    }

  if (src_oreg->address > src)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_get(src_oreg->segid, &src_oseg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (set_head(dst_o->regions, &dst_it) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (set_object(dst_o->regions, dst_it, (void**)&dst_oreg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  while (dst_oreg->address + dst_oreg->size <= dst)
    {
      if (set_next(dst_o->regions, dst_it, &dst_next) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (set_object(dst_o->regions, dst_next, (void**)&dst_oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      memcpy(&dst_it, &dst_next, sizeof (t_iterator));
    }

  if (dst_oreg->address > dst)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_get(dst_oreg->segid, &dst_oseg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 4)
   */

  to_copy_src = src_oreg->size - (src - src_oreg->address);
  to_copy_dst = dst_oreg->size - (dst - dst_oreg->address);
  src_offs = src_oreg->offset + (src - src_oreg->address);
  dst_offs = dst_oreg->offset + (dst - dst_oreg->address);

  for (;;)
    {
      if (to_copy_src < to_copy_dst)
	copy = to_copy_src;
      else
	copy = to_copy_dst;

      if(copy > size)
	copy = size;

      /*
       * 5)
       */

      if (segment_copy(dst_oreg->segid, dst_offs,
		       src_oreg->segid, src_offs,
		       copy) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      src_offs += copy;
      dst_offs += copy;
      size -= copy;
      to_copy_src -= copy;
      to_copy_dst -= copy;

      if (!size)
	break;

      /*
       * 6)
       */

      if (to_copy_src == 0)
	{
	  prev = src_oreg;

	  if (set_next(src_o->regions, src_it, &next) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  if (set_object(src_o->regions, next, (void**)&src_oreg) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  if (src_oreg->address != prev->address + prev->size)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  memcpy(&src_it, &next, sizeof (t_iterator));

	  if (segment_get(src_oreg->segid, &src_oseg) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  src_offs = src_oreg->offset;
	  to_copy_src = src_oreg->size;
	}

      /*
       * 7)
       */


      if (to_copy_dst == 0)
	{
	  prev = dst_oreg;

	  if (set_next(dst_o->regions, dst_it, &next) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  if (set_object(dst_o->regions, next, (void**)&dst_oreg) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  if (dst_oreg->address != prev->address + prev->size)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  memcpy(&dst_it, &next, sizeof (t_iterator));

	  if (segment_get(dst_oreg->segid, &dst_oseg) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);

	  dst_offs = dst_oreg->offset;
	  to_copy_dst = dst_oreg->size;
	}
    }

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

  ASSERT(new != NULL);

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
      o_segment*	o;

      if (set_object(from->segments, i, (void**)&data) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (segment_get(*data, &o) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (o->type == SEGMENT_TYPE_SYSTEM)
	continue;

      if (segment_clone(to->asid, *data, &needless) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      map[0] = *data;
      map[1] = needless;

      if (set_add(mapping, map) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

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
	continue;

      if (region_reserve(to->asid, map[1], data->offset,
			 data->opts | REGION_OPT_FORCE,
			 data->address, data->size, &needless) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * 8)
   */

  if (machine_call(as, as_clone, tskid, old, new) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) gets the task object.
 * 2) initializes the address space object.
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

  ASSERT(asid != NULL);

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

  if (set_reserve(array, SET_OPT_SORT | SET_OPT_FREE,  AS_REGIONS_INITSZ,
		  sizeof(o_region), &o.regions) != ERROR_NONE)
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

  if (machine_call(as, as_reserve, tskid, asid) != ERROR_NONE)
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

  if (machine_call(as, as_release, asid) != ERROR_NONE)
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

  ASSERT(o != NULL);

  if (set_get(as->ass, asid, (void**)o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function initializes the address space manager.
 *
 * this function takes care of initializing and builing the kernel
 * address space.
 *
 * steps:
 *
 * 1) allocates and initializes the address space manager structure.
 * 2) initializes the identifier object to be able to generate
 *    the address space identifiers.
 * 3) reserves the addres space set which will contain the address space
 *    built later.
 * 4) calls the machine-dependent code.
 */

t_error			as_initialize(void)
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
      cons_msg('!', "as: unable to initialize the identifier object\n");

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

  if (machine_call(as, as_initialize) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just reinitializes the address space manager.
 *
 * steps:
 *
 * 1) calls the machine-dependent code.
 * 2) releases the address space's set.
 * 3) destroys the id object.
 * 4) frees the address space manager structure's memory.
 */

t_error			as_clean(void)
{
  o_as*			data;
  t_iterator		i;

  /*
   * 1)
   */

  if (machine_call(as, as_clean) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
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
   * 3)
   */

  if (id_destroy(&as->id) != ERROR_NONE)
    {
      cons_msg('!', "as: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  free(as);

  return (ERROR_NONE);
}
