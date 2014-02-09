/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/segment/segment-fit.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [fri apr  8 09:54:03 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the segment manager provides functionalities regarding the physical
 * memory management.
 *
 * among these functionalities, the manager can reserve and release
 * segments but also modify some of their properties such as a segment's
 * permissions, owner, options etc.
 *
 * the manager also provides functions for manipulating existing segments
 * such as resizing, splitting and copying.
 *
 * every segment is identifier by a 64-bit identified which the caller
 * must use to manipulate its segment.
 *
 * unlike regions, segments are system-wide i.e every segment is unique.
 * therefore, a single set of segments is maintained by the segment
 * manager while every segment is linked to its owner i.e its address
 * space.
 *
 * however, the address space manager also maintains a set for the segments
 * belonging to a specific address space. for more information, please
 * refer to the address space manager.
 *
 * finally, the term segment must not be confound with architecture-specific
 * components. for instance, the Intel x86 architecture terminology also
 * makes use of segment to describe segmentation elements.
 *
 * [XXX:improvement] implement a segment_grab() to reserve a specific segment
 *                   or add options to segment_reserve() very much like
 *                   region_reserve(). this function will be useful for
 *                   reserving memory-mapped devices.
 * [XXX:improvement] the segment_resize() function can render some regions
 *                   invalid. the system would have no choice but to either
 *                   scan all the regions and update them or refuse the
 *                   segment resizing; or keep a set of the regions mapping
 *                   a segment in order to perform this operation more
 *                   efficiently.
 */

/*
 * ---------- algorithm -------------------------------------------------------
 *
 * this implementation relies on the fit algorithm which can be further
 * configured through the SEGMENT_FIT macro:
 *
 *  o FIT_FIRST: first fit algorithm - the first large enough space is taken.
 *  o FIT_BEST: best fit algorithm - the smaller space is taken.
 *  o etc.
 *
 * finally, note that this implementation assigns segment identifiers
 * in order to maintain order within the set of segments. therefore, there
 * is a direct association between a segment's address and its identifier.
 * indeed, every segment identifier is computed as follows:
 *
 *   id = address / frame size
 *
 * this way a segment identifier easily represents the frame---physical
 * memory unit---number of the first segment's address.
 *
 * note however that on most architectures, the smallest unit of physical
 * memory is 1 byte, in which case: id = address.
 */

#if (SEGMENT_ALGORITHM == SEGMENT_ALGORITHM_FIT)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(segment);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager.
 */

m_segment		_segment;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a segment's attributes.
 *
 * steps:
 *
 * 1) retrieve the segment object.
 * 2) compute the options string.
 * 3) compute the permissions string.
 * 4) display the segment's attributes.
 * 5) call the machine.
 */

t_status		segment_show(i_segment			segid,
				     mt_margin			margin)
{
  char			perms[4];
  char			options[2];
  o_segment*		o;

  /*
   * 1)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (o->options & SEGMENT_OPTION_SYSTEM)
    options[0] = 's';
  else
    options[0] = '.';

  options[1] = '\0';

  /*
   * 3)
   */

  if (o->permissions & PERMISSION_READ)
    perms[0] = 'r';
  else
    perms[0] = '.';

  if (o->permissions & PERMISSION_WRITE)
    perms[1] = 'w';
  else
    perms[1] = '.';

  if (o->permissions & PERMISSION_EXEC)
    perms[2] = 'x';
  else
    perms[2] = '.';

  perms[3] = '\0';

  /*
   * 4)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "segment:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  core: id(%qd) range(0x%08x - 0x%08x) permissions(%s) "
	      "size(0x%x) options(%s) as(%qd)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->address,
	      o->address + o->size - 1,
	      perms,
	      o->size,
	      options,
	      o->as);

  /*
   * 5)
   */

  if (machine_call(segment, show, segid, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the segment manager.
 *
 * steps:
 *
 * 1) display general information.
 * 2) retrieve the size of the set of segments.
 * 3) show every segment object.
 * 4) call the machine.
 */

t_status		segment_dump(void)
{
  o_segment*		data;
  t_setsz		size;
  s_iterator		i;
  t_state		s;
  t_uint32		j;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "segment manager:\n");

  module_call(console, message,
	      '#', "  core: base(0x%08x) size(0x%08x) segments(%qd)\n",
	      _segment.base,
	      _segment.size,
	      _segment.segments);

  /*
   * 2)
   */

  if (set_size(_segment.segments, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of segments");

  /*
   * 3)
   */

  module_call(console, message,
	      '#', "    segments: id(%qd) size(%qd)\n",
	      _segment.segments,
	      size);

  set_foreach(SET_OPTION_FORWARD, _segment.segments, &i, s)
    {
      if (set_object(_segment.segments, i, (void**)&data) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object's identifier from the set");

      if (segment_show(data->id,
		       3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object");
    }

  /*
   * 4)
   */

  module_call(console, message,
	      '#', "    vault: size(%u)\n",
	      SEGMENT_VAULT_SIZE);

  for (j = 0; j < SEGMENT_VAULT_SIZE; j++)
    {
      if (_segment.vault[j].state == SEGMENT_VAULT_STATE_AVAILABLE)
	continue;

      module_call(console, message,
		  '#', "      core: address(0x%08x) size(0x%x)\n",
		  _segment.vault[j].address,
		  _segment.vault[j].size);
    }

  /*
   * 5)
   */

  if (machine_call(segment, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function marks an area of physical memory as being temporarily
 * unreservable.
 *
 * this functionality has been introduced in order to counter the following
 * scenario.
 *
 * let us recall that the function segment_inject() calls set_add(). should
 * set_add() need to expand the array, malloc() would be called which, may
 * it need extra memory, would call map_reserve() and, in turn,
 * segment_reserve() and segment_space(). segment_space() could then return
 * the address of the memory area that the caller was trying to inject in
 * order to secure it.
 *
 * the following function can therefore be used to mark a segment in order
 * to render it unreservable until enough memory is allocated to the set
 * manager to handle an additional element.
 *
 * steps:
 *
 * 1) go through the vault entries.
 *   a) if the entry is available, stop looking for one.
 * 2) if no available entry has been found, return a fatal error.
 * 3) register the given zone and mark the entry as being used.
 * 4) call the machine.
 */

t_status		segment_protect(t_paddr			address,
					t_psize			size)
{
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < SEGMENT_VAULT_SIZE; i++)
    {
      /*
       * a)
       */

      if (_segment.vault[i].state == SEGMENT_VAULT_STATE_AVAILABLE)
	break;
    }

  /*
   * 2)
   */

  if (i == SEGMENT_VAULT_SIZE)
    CORE_ESCAPE("unable to protect this zone as the vault is already full");

  /*
   * 3)
   */

  _segment.vault[i].state = SEGMENT_VAULT_STATE_USED;
  _segment.vault[i].address = address;
  _segment.vault[i].size = size;

  /*
   * 4)
   */

  if (machine_call(segment, protect, address, size) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function performs the opposite action of segment_protect() i.e
 * it unmarks a memory area.
 *
 * this function is usally called after the segment has been successfully
 * registered in the manager so that it no longer needs to be protected
 * against reservation.
 *
 * steps:
 *
 * 1) go through the vault entries.
 *   a) if the zone to unprotect is found, stop.
 * 2) if the zone has not been located, return an error.
 * 3) mark the entry as available.
 * 4) call the machine.
 */

t_status		segment_unprotect(t_paddr		address,
					  t_psize		size)
{
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < SEGMENT_VAULT_SIZE; i++)
    {
      /*
       * a)
       */

      if ((_segment.vault[i].state == SEGMENT_VAULT_STATE_USED) &&
	  (_segment.vault[i].address == address) &&
	  (_segment.vault[i].size == size))
	break;
    }

  /*
   * 2)
   */

  if (i == SEGMENT_VAULT_SIZE)
    CORE_ESCAPE("unable to locate the zone to unprotect");

  /*
   * 3)
   */

  memset(&_segment.vault[i], 0x0, sizeof (s_segment_zone));

  /*
   * 4)
   */

  if (machine_call(segment, unprotect, address, size) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns true if the given address lies in a protected
 * zone, false otherwise.
 *
 * steps:
 *
 * 1) go through the vault entries.
 *   a) if the given address lies in the entry's zone and matches the
 *      address space, return true.
 */

t_bool			segment_protected(t_paddr		address)
{
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < SEGMENT_VAULT_SIZE; i++)
    {
      /*
       * a)
       */

      if ((_segment.vault[i].state == SEGMENT_VAULT_STATE_USED) &&
	  (_segment.vault[i].address <= address) &&
	  (address < (_segment.vault[i].address + _segment.vault[i].size)))
	CORE_TRUE();
    }

  CORE_FALSE();
}

/*
 * this function tries to find free space in the segment space via through
 * first fit algorithm.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) if there is no segment...
 *   a) if there is not enough space in the whole segment space to
 *      satisfy the request, return an error.
 *   b) if there is, return the base address.
 * 3) retrieve the very first segment object.
 * 4) if there is enough space between the beginning of the segment space
 *    and the first segment, use it.
 * 5) go through the segments following the first one.
 *   a) retrieve the segment object.
 *   b) try to locate the next segment or exit to loop otherwise.
 *   c) retrieve the next object.
 *   d) if there is enough space between the current object and the next
 *      one, use it.
 * 6) the function has gone through the segments without finding space.
 *    retrieve the last segment object.
 * 7) if there is enough space between the end of the last segment and
 *    the end of the segment space, use it.
 * 8) if the function reaches this point, the request cannot be satisfied
 *    and an error is returned.
 */

t_status		segment_fit_first(i_as			asid,
					  t_psize		size,
					  t_paddr*		address)
{
  o_segment*		current;
  t_state		state;
  o_segment*		head;
  o_segment*		tail;
  s_iterator		i;
  o_as*			as;

  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to find space for zero bytes");

  if (address == NULL)
    CORE_ESCAPE("the 'address' argument is null");

  if ((size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  /*
   * 1)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(_segment.segments, &i) == FALSE)
    {
      /*
       * a)
       */

      if (_segment.size < size)
	CORE_ESCAPE("there is not enough memory to satisfy the segment "
		    "reservation");

      /*
       * b)
       */

      for (*address = _segment.base;
	   *address < (_segment.base + _segment.size);
	   *address = *address + ___kaneton$framesz)
	{
	  /*
	   * i)
	   */

	  if (segment_protected(*address) == FALSE)
	    CORE_LEAVE();
	}

      /*
       * c)
       */

      CORE_ESCAPE("unable to find available memory");
    }

  /*
   * 3)
   */

  if (set_object(_segment.segments, i, (void**)&head) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the very first segment object");

  /*
   * 4)
   */

  for (*address = _segment.base;
       (*address + size) <= head->address;
       *address = *address + ___kaneton$framesz)
    {
      if (segment_protected(*address) == FALSE)
	CORE_LEAVE();
    }

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, _segment.segments, &i, state)
    {
      o_segment*	next;
      s_iterator	j;

      /*
       * a)
       */

      if (set_object(_segment.segments,
		     i,
		     (void**)&current) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the segment object corresponding "
		    "to its identifier");

      /*
       * b)
       */

      if (set_next(_segment.segments, i, &j) == FALSE)
	break;

      /*
       * c)
       */

      if (set_object(_segment.segments, j, (void**)&next) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the next segment in the set");

      /*
       * d)
       */

      for (*address = current->address + current->size;
	   (*address + size) <= next->address;
	   *address = *address + ___kaneton$framesz)
	{
	  if (segment_protected(*address) == FALSE)
	    CORE_LEAVE();
	}
    }

  /*
   * 6)
   */

  if (set_tail(_segment.segments, &i) == FALSE)
    CORE_ESCAPE("unable to locate the last segment in the set");

  if (set_object(_segment.segments, i, (void**)&tail) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 7)
   */

  for (*address = tail->address + tail->size;
       (*address + size) <= (_segment.base + _segment.size);
       *address = *address + ___kaneton$framesz)
    {
      if (segment_protected(*address) == FALSE)
	CORE_LEAVE();
    }

  /*
   * 8)
   */

  CORE_ESCAPE("unable to locate enough space between the existing segments "
	      "to satisfy the reservation request");
}

/*
 * this function tries to locate an available slot of the given size.
 *
 * note that this function is a wrapper above the suppoted algorithms.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) forward the call to the appropriate algorithm
 */

t_status		segment_space(i_as			asid,
				      t_psize			size,
				      t_paddr*			address)
{
  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to find space for zero bytes");

  if (address == NULL)
    CORE_ESCAPE("the 'address' argument is null");

  if ((size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  /*
   * 1)
   */

  switch (SEGMENT_FIT)
    {
    case FIT_FIRST:
      {
	if (segment_fit_first(asid, size, address) != STATUS_OK)
	  CORE_ESCAPE("unable to locate space through the first-fit "
		      "algorithm");

	break;
      }
    default:
      CORE_ESCAPE("unknown segment algorithm '%u'",
		  SEGMENT_FIT);
    }

  CORE_LEAVE();
}

/*
 * this function clones a segment i.e reserves an identical segment with
 * the same content.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object to clone.
 * 2) check if the segment to clone is a system segment, in which case
 *    an error is returned.
 * 3) reserve a new segment of the same size with the write permission
 *    so that data can be copied to this segment.
 * 4) same the segment-to-clone's permissions.
 * 5) copy the data depending on the segment-to-clone's permissions.
 *   A) if the segment-to-clone is not readable...
 *     a) change its permissions to readable.
 *     b) copy the data to the new segment.
 *     c) set the permissions back to what they were.
 *   B) if it is readable...
 *     a) copy the data to the new segment.
 * 6) set the permissions to the new segment so that they match the ones
 *    of the original one's.
 * 7) call the machine.
 */

t_status		segment_clone(i_as			asid,
				      i_segment			old,
				      i_segment*		new)
{
  o_segment*		from;
  t_permissions		perms;

  /*
   * 0)
   */

  if (new == NULL)
    CORE_ESCAPE("the 'new' argument is null");

  /*
   * 1)
   */

  if (segment_get(old, &from) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (from->options & SEGMENT_OPTION_SYSTEM)
    CORE_ESCAPE("unable to clone a system segment");

  /*
   * 3)
   */

  if (segment_reserve(asid,
		      from->size,
		      PERMISSION_WRITE,
		      from->options,
		      new) != STATUS_OK)
    CORE_ESCAPE("unable to reserve a segment");

  /*
   * 4)
   */

  perms = from->permissions;

  /*
   * 5)
   */

  if (!(perms & PERMISSION_READ))
    {
      /*
       * A)
       */

      /*
       * a)
       */

      if (segment_permissions(old, PERMISSION_READ) != STATUS_OK)
	CORE_ESCAPE("unable to modify the segment's permissions");

      /*
       * b)
       */

      if (segment_copy(*new, 0, old, 0, from->size) != STATUS_OK)
	CORE_ESCAPE("unable to copy from the source segment to "
		    "the cloned one");

      /*
       * c)
       */

      if (segment_permissions(old, perms) != STATUS_OK)
	CORE_ESCAPE("unable to modify the segment to its original "
		    "permissions");
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (segment_copy(*new, 0, old, 0, from->size) != STATUS_OK)
	CORE_ESCAPE("unable to copy from the source segment to "
		    "the cloned one");
    }

  /*
   * 6)
   */

  if (segment_permissions(*new, perms) != STATUS_OK)
    CORE_ESCAPE("unable to set the permissions on the cloned segment");

  /*
   * 7)
   */

  if (machine_call(segment, clone, asid, old, new) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function injects a pre-allocated segment object in an address space.
 *
 * steps:
 *
 * 0) retrieve the address space object.
 * 1) fill the remaining object's attributes, especially the segment
 *    identifier.
 * 2) set the segment identifier to return to the caller.
 * 3) protect the zone so that nobody can reserve it until it is added
 *    to the sets.
 * 4) add the segment object to the set of segments but also the segment
 *    identifier to the address space's set of segments.
 * 5) unprotect the zone.
 * 6) call the machine.
 */

t_status		segment_inject(i_as			asid,
				       o_segment*		object,
				       i_segment*		id)
{
  o_as*			as;
 
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  if (object->size == 0)
    CORE_ESCAPE("unable to inject a zero-sized segment object");

  if ((object->size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  if (object->permissions & PERMISSION_INVALID)
    CORE_ESCAPE("invalid object's permissions");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space segment");

  /*
   * 2)
   */

  object->id = SEGMENT_IDENTIFIER(object);
  object->as = asid;

  /*
   * 3)
   */

  *id = object->id;

  /*
   * 4)
   */

  if (segment_protect(object->address, object->size) != STATUS_OK)
    CORE_ESCAPE("unable to protect the zone");

  /*
   * 5)
   */

  if (set_add(_segment.segments, object) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the set of segments");

  if (set_add(as->segments, &object->id) != STATUS_OK)
    CORE_ESCAPE("unable to add the segment identifier to the address space");

  /*
   * 6)
   */

  if (segment_unprotect(object->address, object->size) != STATUS_OK)
    CORE_ESCAPE("unable to unprotect the zone");

  /*
   * 7)
   */

  if (machine_call(segment, inject, asid, object, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function gives the ownership over a segment to another address space,
 * hence migrating the segment from one to the other.
 *
 * steps:
 *
 * 1) retrieve the segment object.
 * 2) retrieve the source and destination address space objects.
 * 3) remove the segment identifier from the source address space's set
 *    of segments.
 * 4) set the segment's new address space owner.
 * 5) add the segment identifier to the destination address space's set
 *    of segments.
 * 6) call the machine.
 */

t_status		segment_give(i_segment			segid,
				     i_as			asid)
{
  o_segment*		o;
  o_as*			dest;
  o_as*			src;

  /*
   * 1)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (as_get(o->as, &src) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  if (as_get(asid, &dest) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  if (set_remove(src->segments, segid) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the set of segments");

  /*
   * 4)
   */

  o->as = asid;

  /*
   * 5)
   */

  if (set_add(dest->segments, &segid) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the set of segments");

  /*
   * 6)
   */

  if (machine_call(segment, give, segid, asid) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function resizes a segment.
 *
 * note that if there is not enough space to enlarge the segment where it
 * resides, a new segment is allocated elsewhere and the content is
 * transferred to the new one. however, this behaviour can be prevented by
 * activating the NORELOCATE option.
 *
 * the caller should be careful as resizing a segment may render the
 * regions mapping it, invalid!
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object to resize, taking care to keep an iterator
 *    on it.
 * 2) perform the resizing depending on the future size.
 *   A) if the future size is smaller than the current one...
 *     a) update the object's size.
 *     b) set the future segment identifier as being the same.
 *   B) if the future size is larger...
 *     a) try to locate the following segment.
 *       #A) if there is such a segment...
 *         i) retrieve the object.
 *         ii) keep the next segment's address in a temporary variable.
 *       #B) otherwise, the segment to resize is the last one...
 *         i) keep the next used address as being the end of the segment space.
 *     b) continue depending on the size available until the next used address.
 *       #A) there is enough space for the segment to expand...
 *         #1) update the object's size.
 *         #2) set the future identifier as being the same.
 *       #B) if there is no way for the segment to be resized in-place...
 *         #1) if the NORELOCATE option has been activated, do not relocate
 *             and return an error.
 *         #2) save the segment's permissions.
 *         #3) reserve another segment with the write permission activated.
 *         #4) activate the read permission.
 *         #5) transfer the content from to the new segment.
 *         #6) set the new segment's permissions to match the ones of the
 *             original's.
 *         #7) release the segment that had to be resized, since no longer
 *             used.
 * 3) call the machine.
 */

t_status		segment_resize(i_segment		old,
				       t_psize			size,
				       t_options		options,
				       i_segment*		new)
{
  t_permissions		perms;
  s_iterator		it;
  o_segment*		o;

  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to resize the segment to a zero-byte size");

  if (new == NULL)
    CORE_ESCAPE("the 'new' argument is null");

  if ((size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  /*
   * 1)
   */

  if (set_locate(_segment.segments, old, &it) != STATUS_OK)
    CORE_ESCAPE("unable to locate the object in the set of segments");

  if (set_object(_segment.segments, it, (void**)&o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (size < o->size)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      o->size = size;

      /*
       * b)
       */

      *new = old;
    }
  else
    {
      t_paddr		address;
      s_iterator	next;

      /*
       * B)
       */

      /*
       * a)
       */

      if (set_next(_segment.segments, it, &next) == TRUE)
	{
	  /*
	   * #A)
	   */

	  o_segment*	onext;

	  /*
	   * i)
	   */

	  if (set_object(_segment.segments, next, (void**)&onext) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the segment object");

	  /*
	   * ii)
	   */

	  address = onext->address;
	}
      else
	{
	  /*
	   * #B)
	   */

	  /*
	   * i)
	   */

	  address = _segment.base + _segment.size;
	}

      /*
       * b)
       */

      if ((o->address + size) < address)
	{
	  /*
	   * #A)
	   */

	  /*
	   * #1)
	   */

	  o->size = size;

	  /*
	   * #2)
	   */

	  *new = old;
	}
      else
	{
	  /*
	   * #B)
	   */

	  /*
	   * #1)
	   */

	  if (options & SEGMENT_OPTION_NORELOCATE)
	    CORE_ESCAPE("unable to resize the segment in-place");

	  /*
	   * #2)
	   */

	  perms = o->permissions;

	  /*
	   * #3
	   */

	  if (segment_reserve(o->as,
			      size,
			      PERMISSION_WRITE,
			      o->options,
			      new) != STATUS_OK)
	    CORE_ESCAPE("unable to reserve a segment");

	  /*
	   * #4)
	   */

	  if (segment_permissions(old, PERMISSION_READ) != STATUS_OK)
	    CORE_ESCAPE("unable to modify the segment's permissions");

	  /*
	   * #5)
	   */

	  if (segment_copy(*new, 0, old, 0, o->size) != STATUS_OK)
	    CORE_ESCAPE("unable to copy from one segment to the other");

	  /*
	   * #6)
	   */

	  if (segment_permissions(*new, perms) != STATUS_OK)
	    CORE_ESCAPE("unable to modify the segment's permissions");

	  /*
	   * #7)
	   */

	  if (segment_release(old) != STATUS_OK)
	    CORE_ESCAPE("unable to release the segment");
	}
    }

  /*
   * 3)
   */

  if (machine_call(segment, resize, old, size, new) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function splits a segment into two new segments.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment and address space objects.
 * 2) if the splitting size is larger than the segment's size, return
 *    an error.
 * 3) allocate and build the segment object for the 'right' part of the
 *    splitting.
 * 4) update the current segment i.e the future-'left' part's size.
 * 5) set the future segment's identifiers: the left takes the identifier
 *    of the current segment while the right is computed.
 * 6) inject the new 'right' object in the address space.
 * 7) call the machine.
 */

t_status		segment_split(i_segment			segid,
				      t_psize			size,
				      i_segment*		left,
				      i_segment*		right)
{
  o_as*			as;
  o_segment*		o;
  o_segment*		n;
  i_segment		useless;

  /*
   * 0
   */

  if (size == 0)
    CORE_ESCAPE("unable to split a segment to an empty size");

  if (left == NULL)
    CORE_ESCAPE("the 'left' argument is null");

  if (right == NULL)
    CORE_ESCAPE("the 'right' argument is null");

  if ((size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  if (as_get(o->as, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (size > o->size)
    CORE_ESCAPE("the splitted size must be smaller than the current size");

  /*
   * 3)
   */

  if ((n = malloc(sizeof (o_segment))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the segment object");

  n->as = o->as;
  n->permissions = o->permissions;
  n->options = o->options;

  n->address = o->address + size;
  n->size = o->size - size;

  n->id = SEGMENT_IDENTIFIER(n);

  /*
   * 4)
   */

  o->size = size;

  /*
   * 5)
   */

  *left = segid;
  *right = n->id;

  /*
   * 6)
   */

  if (segment_inject(o->as, n, &useless) != STATUS_OK)
    CORE_ESCAPE("unable to inject the segment object");

  /*
   * 7)
   */

  if (machine_call(segment, split,
		   segid, size, left, right) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function coalesces two adjacent segments into a single one.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve both the left and right segment objects.
 * 2) verify that the segments are adjacent, with the same options and
 *    permissions etc.
 * 3) compute the new segment size.
 * 4) return the future identifier i.e the identifier of the left
 *    segment while the right one will be destroyed.
 * 5) release the right segment.
 * 6) call the machine.
 */

t_status		segment_coalesce(i_segment		left,
					 i_segment		right,
					 i_segment*		id)
{
  o_segment*		seg1;
  o_segment*		seg2;

  /*
   * 0)
   */

  if (left == right)
    CORE_ESCAPE("unable to coalesce the same segment");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  if (segment_get(left, &seg1) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  if (segment_get(right, &seg2) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if ((seg1->address + seg1->size) != seg2->address)
    CORE_ESCAPE("unable to coalesce non-adjacent segments");

  if (seg1->as != seg2->as)
    CORE_ESCAPE("unable to coalesce segments from different address spaces");

  if (seg1->permissions != seg2->permissions)
    CORE_ESCAPE("unable to coalesce segments with different permissions");

  if (seg1->options != seg2->options)
    CORE_ESCAPE("unable to coalesce segments with different options");

  /*
   * 3)
   */

  seg1->size += seg2->size;

  /*
   * 4)
   */

  *id = seg1->id;

  /*
   * 5)
   */

  if (segment_release(right) != STATUS_OK)
    CORE_ESCAPE("unable to release the segment");

  /*
   * 6)
   */

  if (machine_call(segment, coalesce,
		   left, right, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reads data from a segment.
 *
 * note that this function relies on the machine for performing the
 * operation since the kaneton kernel is assumed to evolve in a virtual
 * environment. the machine probably benefits from advanced functionalities
 * for performing such an operation.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object.
 * 2) check the segment's permissions along with the operation's boundaries.
 * 3) call the machine.
 */

t_status		segment_read(i_segment			segid,
				     t_paddr			offs,
				     void*			buffer,
				     t_psize			sz)
{
  o_segment*		o;

  /*
   * 0)
   */

  if (sz == 0)
    CORE_ESCAPE("unable to perform a zero-sized reading");

  if (buffer == NULL)
    CORE_ESCAPE("the 'buffer' argument is null");

  /*
   * 1)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (!(o->permissions & PERMISSION_READ))
    CORE_ESCAPE("unable to read data from a non-readable segment");

  if ((offs + sz) > o->size)
    CORE_ESCAPE("out-of-bound operation");

  /*
   * 3)
   */

  if (machine_call(segment, read, segid, offs, buffer, sz) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function write data to a given segment.
 *
 * note that this function relies on the machine for performing the
 * operation since the kaneton kernel is assumed to evolve in a virtual
 * environment. the machine probably benefits from advanced functionalities
 * for performing such an operation.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object.
 * 2) check the segment's permissions along with the operation's boundaries.
 * 3) call the machine.
 */

t_status		segment_write(i_segment			segid,
				      t_paddr			offs,
				      const void*		buffer,
				      t_psize			sz)
{
  o_segment*		o;

  /*
   * 0)
   */

  if (sz == 0)
    CORE_ESCAPE("unable to perform a zero-sized writing");

  if (buffer == NULL)
    CORE_ESCAPE("the 'buffer' argument is null");

  /*
   * 1)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (!(o->permissions & PERMISSION_WRITE))
    CORE_ESCAPE("unable to write a non-writable segment");

  if (offs + sz > o->size)
    CORE_ESCAPE("out-of-bound operation");

  /*
   * 3)
   */

  if (machine_call(segment, write,
		   segid, offs, buffer, sz) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function copies data from one segment to another.
 *
 * note that this function relies on the machine for performing the
 * operation since the kaneton kernel is assumed to evolve in a virtual
 * environment. the machine probably benefits from advanced functionalities
 * for performing such an operation.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment objects.
 * 2) check the segments' permissions along with the operation's boundaries.
 * 3) call the machine.
 */

t_status		segment_copy(i_segment			dst,
				     t_paddr			offsd,
				     i_segment			src,
				     t_paddr			offss,
				     t_psize			sz)
{
  o_segment*		o1;
  o_segment*		o2;

  /*
   * 0)
   */

  if (sz == 0)
    CORE_ESCAPE("unable to perform a zero-sized copy");

  /*
   * 1)
   */

  if (segment_get(dst, &o1) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  if (segment_get(src, &o2) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (!(o1->permissions & PERMISSION_WRITE))
    CORE_ESCAPE("unable to copy from a non-readable segment");

  if (!(o2->permissions & PERMISSION_READ))
    CORE_ESCAPE("unable to copy to a non-writable segment");

  if (offsd + sz > o1->size)
    CORE_ESCAPE("the source address is out-of-bound");

  if (offss + sz > o2->size)
    CORE_ESCAPE("the destination address is out-of-bound");

  /*
   * 3)
   */

  if (machine_call(segment, copy,
		   dst, offsd, src, offss, sz) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function reserves a segment given the desired size and permissions.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) allocate memory for the segment object.
 * 3) try to locate available space for the segment according to the
 *    requested size.
 * 4) protect the located space from being reserved.
 * 5) fill the object's attributes.
 * 6) set the identifier to return.
 * 7) add the object to the set of segments and the identifier to the
 *    address space's set of segments.
 * 8) unprotect the zone.
 * 9) call the machine.
 */

t_status		segment_reserve(i_as			asid,
					t_psize			size,
					t_permissions		perms,
					t_options		options,
					i_segment*		id)
{
  o_as*			as;
  o_segment*		o;

  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to reserve a segment with an empty size");

  if (perms & PERMISSION_INVALID)
    CORE_ESCAPE("invalid permissions");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  if ((size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  /*
   * 1)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if ((o = malloc(sizeof (o_segment))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the segment object");


  /*
   * 3)
   */

  if (segment_space(asid, size, &o->address) != STATUS_OK)
    CORE_ESCAPE("unable to locate unused space for the segment resevation");

  /*
   * 4)
   */

  if (segment_protect(o->address, size) != STATUS_OK)
    CORE_ESCAPE("unable to protect the zone");

  /*
   * 5)
   */

  o->as = as->id;
  o->size = size;
  o->permissions = perms;
  o->options = options;
  o->id = SEGMENT_IDENTIFIER(o);

  /*
   * 6)
   */

  *id = o->id;

  /*
   * 7)
   */

  if (set_add(_segment.segments, o) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the set of segments");

  if (set_add(as->segments, &o->id) != STATUS_OK)
    CORE_ESCAPE("unable to add the segment identifier to the addresss space");

  /*
   * 8)
   */

  if (segment_unprotect(o->address, size) != STATUS_OK)
    CORE_ESCAPE("unable to unprotect the zone");

  /*
   * 9)
   */

  if (machine_call(segment, reserve,
		   asid, size, perms, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function releases a segment.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the segment and address space objects.
 * 4) remove the object from the set of segments and the identifier from
 *    the address space's set of segments.
 */

t_status		segment_release(i_segment		segid)
{
  o_as*			as;
  o_segment*		o;

  /*
   * 1)
   */

  if (machine_call(segment, release, segid) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  if (as_get(o->as, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  if (set_remove(as->segments, segid) != STATUS_OK)
    CORE_ESCAPE("unable to remove the segment identifier from the "
		"address space");

  if (set_remove(_segment.segments, segid) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the set of segments");

  CORE_LEAVE();
}

/*
 * this function modifies the permissions of a given segment.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object.
 * 3) assign the new permissions.
 * 4) call the machine.
 */

t_status		segment_permissions(i_segment		segid,
					    t_permissions	perms)
{
  o_segment*		o;

  /*
   * 0)
   */

  if (perms & PERMISSION_INVALID)
    CORE_ESCAPE("invalid permissions");

  /*
   * 1)
   */

  if (segment_get(segid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  o->permissions = perms;

  /*
   * 3)
   */

  if (machine_call(segment, permissions, segid, perms) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function removes every segment that belongs to the given address space.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the address space object.
 * 3) go through the segments of the address space.
 *   a) retrieve the segment identifier.
 *   b) release the segment.
 */

t_status		segment_flush(i_as			asid)
{
  i_segment*		data;
  o_as*			as;
  s_iterator		i;

  /*
   * 1)
   */

  if (machine_call(segment, flush, asid) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  while (set_head(as->segments, &i) == TRUE)
    {
      /*
       * a)
       */

      if (set_object(as->segments, i, (void**)&data) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object corresponding to "
		    "its identifier");

      /*
       * b)
       */

      if (segment_release(*data) != STATUS_OK)
	CORE_ESCAPE("unable to release the segment");
    }

  CORE_LEAVE();
}

/*
 * this functions takes a physical address and returns the segment which
 * holds this address.
 *
 * note that this function returns either true or false depending on the
 * success of the look up.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) go through the segments of the set.
 *   a) retrieve the object.
 *   b) if the address lies in this segment, return its identifier.
 * 2) if no segment has been found, return false;
 */

t_bool			segment_locate(t_paddr			address,
				       i_segment*		id)
{
  o_segment*		object;
  t_state		state;
  s_iterator		i;

  /*
   * 0)
   */

  assert(id != NULL);

  /*
   * 1)
   */

  set_foreach(SET_OPTION_FORWARD, _segment.segments, &i, state)
    {
      /*
       * a)
       */

      assert(set_object(_segment.segments, i, (void**)&object) == STATUS_OK);

      /*
       * b)
       */

      if ((object->address <= address) &&
	  (address < (object->address + object->size)))
	{
	  *id = object->id;

	  CORE_TRUE();
	}
    }

  /*
   * 2)
   */

  CORE_FALSE();
}

/*
 * this function returns true if the segment exists.
 */

t_bool			segment_exist(i_segment			segid)
{
  if (set_exist(_segment.segments, segid) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns a segment object according to its identifier.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the manager's set of segments.
 */

t_status		segment_get(i_segment			segid,
				    o_segment**			object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_segment.segments, segid, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of segments");

  CORE_LEAVE();
}

/*
 * this function initializes the segment manager according to the base
 * and size argument received representing the space of physical memory.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the segment manager's structure.
 * 3) record the base and size in the manager's structure.
 * 4) reserve the set of segments which will contain the system's segments.
 * 5) call the machine.
 */

t_status		segment_initialize(t_paddr		base,
					   t_psize		size)
{
  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to initialize the segment manager with a "
		"size of zero");

  if ((base % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the base is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  if ((size % ___kaneton$framesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's frame size: '%u'",
		___kaneton$framesz);

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the segment manager\n");

  /*
   * 2)
   */

  memset(&_segment, 0x0, sizeof (m_segment));

  /*
   * 3)
   */

  _segment.base = base;
  _segment.size = size;

  /*
   * 4)
   */

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_FREE,
		  sizeof (o_segment),
		  SEGMENT_BPT_NODESZ,
		  &_segment.segments) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the segments set");

  /*
   * 5)
   */

  if (machine_call(segment, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the segment manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 * 3) release all the segments.
 *   a) retrieve the segment object.
 *   b) release the segment.
 * 4) release the set of segments.
 */

t_status		segment_clean(void)
{
  o_segment*		data;
  s_iterator		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the segment manager\n");

  /*
   * 2)
   */

  if (machine_call(segment, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 3)
   */

  while (set_head(_segment.segments, &i) == TRUE)
    {
      /*
       * a)
       */

      if (set_object(_segment.segments, i, (void**)&data) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the object corresponding to "
		    "its identifier");

      /*
       * b)
       */

      if (segment_release(data->id) != STATUS_OK)
	CORE_ESCAPE("unable to release the segment");
    }

  /*
   * 4)
   */

  if (set_release(_segment.segments) != STATUS_OK)
    CORE_ESCAPE("unable to release the segments set");

  CORE_LEAVE();
}

#endif
