/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/region/region-fit.c
 *
 * created       julien quintard   [wed nov 23 09:19:43 2005]
 * updated       julien quintard   [fri apr  8 09:28:15 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the region manager provides functionalities for mapping segments through
 * so called 'regions'. therefore the terms "mapping" and "reserving a
 * region" can be interchangeably used.
 *
 * unlike segments, regions live in an address space. therefore a region 
 * itself is meanginless outside its address space. besides, there is no
 * global set of regions as there is for segments. instead, the address
 * space maintains a set containing its region objects.
 */

/*
 * ---------- algorithm -------------------------------------------------------
 *
 * this implementation has a specificity regarding region identifiers. indeed,
 * like for segments, region identifiers are based on the object's address.
 * a region's identifier represents the page number in the virtual space,
 * starting at one. in other words: id = address / page size. regions
 * are identified this way in order to ensure that the regions are always
 * sorted in their set.
 *
 * this implementation relies on the fit algorithm though this algorithm can
 * be further configured through the REGION_FIT macro:
 *
 *  o FIT_FIRST: first fit algorithm - the first large enough space is used.
 *  o FIT_BEST: best fit algorithm - the smaller space is used.
 *  o etc.
 */

#if (REGION_ALGORITHM == REGION_ALGORITHM_FIT)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(region);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager structure.
 */

m_region		_region;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a region's attributes.
 *
 * steps:
 *
 * 1) retrieve the region object.
 * 2) build the options string.
 * 3) display the attributes.
 * 4) call the machine.
 */

t_status		region_show(i_as			asid,
				    i_region			regid,
				    mt_margin			margin)
{
  o_region*		o;
  char			options[2];

  /*
   * 1)
   */

  if (region_get(asid, regid, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 2)
   */

  memset(options, 0x0, sizeof (options));

  if (o->options & REGION_OPTION_FORCE)
    options[0] = 'f';
  else
    options[0] = '.';

  options[1] = '\0';

  /*
   * 3)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "region:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  core: id(%qd) range(0x%08x - 0x%08x) segment(%qd) "
	      "offset(0x%x) size(0x%x) options(%s)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->address,
	      o->address + o->size - 1,
	      o->segment,
	      o->offset,
	      o->size,
	      options);

  /*
   * 3)
   */

  if (machine_call(region, show, asid, regid, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps all the regions of the given address space.
 *
 * steps:
 *
 * 1) display general information.
 * 2) display the content of the vault.
 * 3) call the machine.
 */

t_status		region_dump(void)
{
  t_uint32		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "region manager:\n");

  module_call(console, message,
	      '#', "  core: base(0x%08x) size(0x%08x)\n",
	      _region.base,
	      _region.size);

  /*
   * 2)
   */

  module_call(console, message,
	      '#', "    vault: size(%u)\n",
	      REGION_VAULT_SIZE);

  for (i = 0; i < REGION_VAULT_SIZE; i++)
    {
      if (_region.vault[i].state == REGION_VAULT_STATE_AVAILABLE)
	continue;

      module_call(console, message,
		  '#', "      core: as(%qd) address(0x%08x) size(0x%x)\n",
		  _region.vault[i].as,
		  _region.vault[i].address,
		  _region.vault[i].size);
    }

  /*
   * 3)
   */

  if (machine_call(region, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function marks an area of virtual memory as being temporarily
 * unreservable.
 *
 * this functionality has been introduced in order to counter the following
 * scenario.
 *
 * let us recall that the function region_inject() calls set_add(). should
 * set_add() need to expand the array, malloc() would be called which, may
 * it need extra memory, would call map_reserve() and, in turn,
 * region_reserve() and region_space(). region_space() could then return
 * the address of the memory area that the caller was trying to inject in
 * order to secure it.
 *
 * the following function can therefore be used to mark a region in order
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

t_status		region_protect(i_as			as,
				       t_vaddr			address,
				       t_vsize			size)
{
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < REGION_VAULT_SIZE; i++)
    {
      /*
       * a)
       */

      if (_region.vault[i].state == REGION_VAULT_STATE_AVAILABLE)
	break;
    }

  /*
   * 2)
   */

  if (i == REGION_VAULT_SIZE)
    CORE_ESCAPE("unable to protect this zone as the vault is already full");

  /*
   * 3)
   */

  _region.vault[i].state = REGION_VAULT_STATE_USED;
  _region.vault[i].as = as;
  _region.vault[i].address = address;
  _region.vault[i].size = size;

  /*
   * 4)
   */

  if (machine_call(region, protect, as, address, size) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function performs the opposite action of region_protect() i.e
 * it unmarks a memory area.
 *
 * this function is usally called after the region has been successfully
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

t_status		region_unprotect(i_as			as,
					 t_vaddr		address,
					 t_vsize		size)
{
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < REGION_VAULT_SIZE; i++)
    {
      /*
       * a)
       */

      if ((_region.vault[i].state == REGION_VAULT_STATE_USED) &&
	  (_region.vault[i].as == as) &&
	  (_region.vault[i].address == address) &&
	  (_region.vault[i].size == size))
	break;
    }

  /*
   * 2)
   */

  if (i == REGION_VAULT_SIZE)
    CORE_ESCAPE("unable to locate the zone to unprotect");

  /*
   * 3)
   */

  memset(&_region.vault[i], 0x0, sizeof (s_region_zone));

  /*
   * 4)
   */

  if (machine_call(region, unprotect, as, address, size) != STATUS_OK)
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

t_bool			region_protected(i_as			as,
					 t_vaddr		address)
{
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < REGION_VAULT_SIZE; i++)
    {
      /*
       * a)
       */

      if ((_region.vault[i].state == REGION_VAULT_STATE_USED) &&
	  (_region.vault[i].as == as) &&
	  (_region.vault[i].address <= address) &&
	  (address < (_region.vault[i].address + _region.vault[i].size)))
	CORE_TRUE();
    }

  CORE_FALSE();
}

/*
 * this function tries to find free space in the region set via the
 * first fit algorithm.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) if there is no region yet...
 *   a) verify that the whole region space is large enough to satisfy the
 *      request.
 *   b) try every address to make sure it is not protected.
 *   c) if no address has been found, return an error.
 * 3) otherwise, retrieve the very first region object.
 * 4) perhaps there is enough space between the beginning of the virtual
 *    space and the first region's address.
 * 5) go through the regions of the address space.
 *   a) retrieve the region object.
 *   b) try to retrieve the following region object, or escape otherwise.
 *   c) if there is space between the two regions, return this address.
 * 6) retrieve the last region.
 * 7) try to find space between the last region and the end of the virtual
 *    space.
 */

t_status		region_fit_first(i_as			asid,
					 t_vsize		size,
					 t_vaddr*		address)
{
  o_region*		current;
  t_state		state;
  o_region*		head;
  o_region*		tail;
  o_as*			as;
  s_iterator		i;

  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to find space for zero bytes");

  if (address == NULL)
    CORE_ESCAPE("the 'address' argument is null");

  if ((size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  /*
   * 1)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_head(as->regions, &i) != TRUE)
    {
      /*
       * a)
       */

      if ((_region.size - _region.base) < size)
	CORE_ESCAPE("there is not enough memory to satisfy the reservation "
		    "request");

      /*
       * b)
       */

      for (*address = _region.base;
	   *address < (_region.base + _region.size);
	   *address = *address + ___kaneton$pagesz)
	{
	  /*
	   * i)
	   */

	  if (region_protected(as->id, *address) == FALSE)
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

  if (set_object(as->regions, i, (void**)&head) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space's very first region");

  /*
   * 4)
   */

  for (*address = _region.base;
       (*address + size) <= head->address;
       *address = *address + ___kaneton$pagesz)
    {
      if (region_protected(as->id, *address) == FALSE)
	CORE_LEAVE();
    }

  /*
   * 5)
   */

  set_foreach(SET_OPTION_FORWARD, as->regions, &i, state)
    {
      o_region*		next;
      s_iterator	j;

      /*
       * a)
       */

      if (set_object(as->regions, i, (void**)&current) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      /*
       * b)
       */

      if (set_next(as->regions, i, &j) != TRUE)
	break;

      if (set_object(as->regions, j, (void**)&next) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the next region object");

      /*
       * c)
       */

      for (*address = current->address + current->size;
	   (*address + size) <= next->address;
	   *address = *address + ___kaneton$pagesz)
	{
	  if (region_protected(as->id, *address) == FALSE)
	    CORE_LEAVE();
	}
    }

  /*
   * 6)
   */

  if (set_tail(as->regions, &i) != TRUE)
    CORE_ESCAPE("unable to locate the address space's last region");

  if (set_object(as->regions, i, (void**)&tail) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 7)
   */

  for (*address = tail->address + tail->size;
       (*address + size) <= (_region.base + _region.size);
       *address = *address + ___kaneton$pagesz)
    {
      if (region_protected(as->id, *address) == FALSE)
	CORE_LEAVE();
    }

  CORE_ESCAPE("unable to locate enough space between the existing regions "
	      "to satisfy the reservation request");
}

/*
 * this function tries to find available space in the address space.
 *
 * note that this function is a wrapper over the several fit algorithms
 * provided.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) forward the call to the right algorithm.
 */

t_status		region_space(i_as			asid,
				     t_vsize			size,
				     t_vaddr*			address)
{
  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to find space for zero bytes");

  if (address == NULL)
    CORE_ESCAPE("the 'address' argument is null");

  if ((size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  /*
   * 1)
   */

  switch (REGION_FIT)
    {
    case FIT_FIRST:
      {
	if (region_fit_first(asid, size, address) != STATUS_OK)
	  CORE_ESCAPE("unable to locate space through the first-fit "
		      "algorithm");

	break;
      }
    default:
      CORE_ESCAPE("unknown region's fit algorithm '%u'",
		  REGION_FIT);
    }

  CORE_LEAVE();
}

/*
 * this function injects a region object into the address space.
 *
 * note that the object must already be ready for insertion in the set
 * of regions. therefore if the set has not be configured to clone the
 * objects to add, the object should have been pre-allocated.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) fill the remaining region attributes, especially the identifier.
 * 3) protect the zone so that it cannot get reserved until it has been
 *    properly added to the set.
 * 4) add the object to the address space's set of regions.
 * 5) unprotect the zone.
 * 6) call the machine.
 */

t_status		region_inject(i_as			as,
				      o_region*			object,
				      i_region*			id)
{
  o_as*			o;

  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  if (object->options & REGION_OPTION_INVALID)
    CORE_ESCAPE("the object's options are invalid");

  if (object->size == 0)
    CORE_ESCAPE("the object's size is zero");

  if ((object->size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  /*
   * 1)
   */

  if (as_get(as, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  object->id = REGION_IDENTIFIER(object);
  object->options |= REGION_OPTION_FORCE;

  *id = object->id;

  /*
   * 3)
   */

  if (region_protect(o->id, object->address, object->size) != STATUS_OK)
    CORE_ESCAPE("unable to protect the zone");

  /*
   * 4)
   */

  if (set_add(o->regions, object) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the address space's set of "
		"regions");

  /*
   * 5)
   */

  if (region_unprotect(o->id, object->address, object->size) != STATUS_OK)
    CORE_ESCAPE("unable to unprotect the zone");

  /*
   * 6)
   */

  if (machine_call(region, inject, as, object, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function splits a region into two regions according to the
 * splitting size.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the region object and its associated segment.
 * 2) verify that the the splitting size fits the region.
 * 3) build the new future-'right' region.
 * 4) inject the new region in the address space's set of regions.
 * 5) adjust the old future-'left' region size.
 * 6) set the new left and right identifiers.
 * 7) call the machine.
 */

t_status		region_split(i_as			asid,
				     i_region			regid,
				     t_vsize			size,
				     i_region*			left,
				     i_region*			right)
{
  i_region		useless;
  o_region*		reg;
  o_segment*		seg;
  o_region*		new;

  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to split to a size of zero");

  if (left == NULL)
    CORE_ESCAPE("the 'left' argument is null");

  if (right == NULL)
    CORE_ESCAPE("the 'right' argument is null");

  if ((size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  /*
   * 1)
   */

  if (region_get(asid, regid, &reg) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  if (segment_get(reg->segment, &seg) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (reg->size <= size)
    CORE_ESCAPE("the splitted size is larger than the current region's size");

  /*
   * 3)
   */

  if ((new = malloc(sizeof (o_region))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the region object");

  new->segment = reg->segment;
  new->offset = reg->offset + size;
  new->address = reg->address + size;
  new->options = reg->options;
  new->size = reg->size - size;

  new->id = REGION_IDENTIFIER(new);

  /*
   * 4)
   */

  if (region_inject(asid, new, &useless) != STATUS_OK)
    CORE_ESCAPE("unable to inject the object in the address space's "
		"set of regions");

  /*
   * 5)
   */

  reg->size = size;

  /*
   * 6
   */

  *left = reg->id;
  *right = new->id;

  /*
   * 7)
   */

  if (machine_call(region, split,
		   asid,
		   regid,
		   size,
		   left,
		   right) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function resizes a region according to the given size which can be
 * smaller or larger than the current size.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) retrieve the region object to resize.
 * 3) set the future identifier since it will always be the same.
 * 4) resize the region depending on the future size.
 *   A) if the future size is smaller than the current size...
 *     a) call the machine.
 *   B) if the future size is larger than the current size...
 *     a) try to locate the following region.
 *       i) if such a region exist...
 *         #1) retrieve the following region object.
 *         #2) verify that there is enough space between the current region
 *             and the following one to resize i.e resizing will not make
 *             the current region overlap the following one.
 *       ii) if the current region is the last one...
 *         #1) verify that there is enough space between the current region
 *             and the end of the virtual space to accept the resizing.
 *     b) retrieve the segment object mapped by the region.
 *     c) verify that the region's future size will not be larger than the
 *        segment's.
 *     d) protect the expanded part of the future region.
 *     e) call the machine.
 *     f) unprotect the zone.
 *   C) if the future size is identical to the current one...
 *     a) nothing to do in this case.
 * 5) update the region's size with the resized size.
 */

t_status		region_resize(i_as			as,
				      i_region			old,
				      t_vsize			size,
				      i_region*			new)
{

  o_region*		reg = NULL;
  o_region*		next_reg;
  o_segment*		seg;
  o_as*			oas;
  s_iterator		i;
  s_iterator		next;

  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to resize to a size of zero");

  if (new == NULL)
    CORE_ESCAPE("the 'new' argument is null");

  if ((size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  /*
   * 1)
   */

  if (as_get(as, &oas) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_locate(oas->regions, old, &i) != STATUS_OK)
    CORE_ESCAPE("unable to locate the object in the address space's "
		"set of regions");

  if (set_object(oas->regions, i, (void**)&reg) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  *new = old;

  /*
   * 4)
   */

  if (size < reg->size)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      if (machine_call(region, resize,
		       as,
		       old,
		       size,
		       new) != STATUS_OK)
	CORE_ESCAPE("an error occured in the machine");
    }
  else if (size > reg->size)
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (set_next(oas->regions, i, &next) == TRUE)
	{
	  /*
	   * i)
	   */

	  /*
	   * #1)
	   */

	  if (set_object(oas->regions, next, (void**)&next_reg) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the next region object");

	  /*
	   * #2)
	   */

	  if ((reg->address + size) >= next_reg->address)
	    CORE_ESCAPE("unable to resize the region since it would "
			"overlap with the next region");
	}
      else
	{
	  /*
	   * ii)
	   */

	  /*
	   * #1)
	   */

	  if ((reg->address + size) >= (_region.base + _region.size))
	    CORE_ESCAPE("unable to resize the region since it would reach the "
			"end of the available region space");
	}

      /*
       * b)
       */

      if (segment_get(reg->segment, &seg) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the segment object");

      /*
       * c)
       */

      if (size > seg->size)
	CORE_ESCAPE("unable to resize a region to a size larger than "
		    "its segment");

      /*
       * d)
       */

      if (region_protect(as,
			 reg->address + reg->size,
			 size - reg->size) != STATUS_OK)
	CORE_ESCAPE("unable to protect the zone");

      /*
       * e)
       */

      if (machine_call(region, resize,
		       as,
		       old,
		       size,
		       new) != STATUS_OK)
	CORE_ESCAPE("an error occured in the machine");

      /*
       * f)
       */

      if (region_unprotect(as,
			   reg->address + reg->size,
			   size - reg->size) != STATUS_OK)
	CORE_ESCAPE("unable to unprotect the zone");
    }
  else
    {
      /*
       * C)
       */

      /*
       * a)
       */
    }

  /*
   * 5)
   */

  reg->size = size;

  CORE_LEAVE();
}

/*
 * this function merges two adjacent regions into a single one.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve both region objects.
 * 2) verify that the regions are adjacents but also that they have
 *    the same privileges.
 * 3) retrieve the address space object.
 * 4) set the final region size.
 * 5) remove the 'right' region from the address space's set of regions.
 * 6) call the machine.
 * 7) return the new region identifier.
 */

t_status		region_coalesce(i_as			asid,
					i_region		left,
					i_region		right,
					i_region*		region)
{
  o_as*			as;
  o_region*		oleft;
  o_region*		oright;

  /*
   * 0)
   */

  if (left == right)
    CORE_ESCAPE("unable to coalesce the two same regions");

  if (region == NULL)
    CORE_ESCAPE("the 'region' argument is null");

  /*
   * 1)
   */

  if (region_get(asid, left, &oleft) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  if (region_get(asid, right, &oright) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region object");

  /*
   * 2)
   */

  if (oleft->address + oleft->size != oright->address ||
      oleft->segment != oright->segment ||
      oleft->offset + oleft->size != oright->offset)
    CORE_ESCAPE("unable to coalesce non-adjacent regions");

  /*
   * 3)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 4)
   */

  oleft->size += oright->size;

  /*
   * 5)
   */

  if (set_remove(as->regions, right) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the address space's set "
		"of regions");

  /*
   * 6)
   */

  if (machine_call(region, coalesce,
		   asid,
		   left,
		   right,
		   region) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 7)
   */

  *region = left;

  CORE_LEAVE();
}

/*
 * this function reserves a region in order to map the given segment
 * at a precise offset, for a specific size and according to several options.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) retrieve the segment object.
 * 3) verify that the region does not map outside the segment.
 * 4) compute the future region's address depending on the options.
 *   A) if the region has been request to be located at a precise address...
 *     a) verify that the address lies in the virtual space.
 *     b) go through the existing regions and verify that this address is
 *        not already in use.
 *   B) if the region can be reserved anywhere...
 *     a) look for available space in the address space's set of regions.
 * 5) protect the located zone.
 * 6) allocate the object.
 * 7) fill the object's attributes.
 * 8) add the object to the address space's set of regions.
 * 9) unprotect the zone.
 * 10) call the machine.
 * 11) return the region's identifier.
 */

t_status		region_reserve(i_as			asid,
				       i_segment		segid,
				       t_paddr			offset,
				       t_options		options,
				       t_vaddr			address,
				       t_vsize			size,
				       i_region*		region)
{
  o_segment*		segment;
  o_as*			as;
  o_region*		o;
  t_state		state;
  s_iterator		i;
  o_region*		data;

  /*
   * 0)
   */

  if (options & REGION_OPTION_INVALID)
    CORE_ESCAPE("the options seem to be invalid");

  if (size == 0)
    CORE_ESCAPE("unable to reserve a region with a zero size");

  if (region == NULL)
    CORE_ESCAPE("the 'region' argument is null");

  if ((size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  if (options & REGION_OPTION_FORCE)
    {
      if ((address % ___kaneton$pagesz) != 0)
        CORE_ESCAPE("the forced address is not aligned on the machine's "
                    "page size: '%u'",
                    ___kaneton$pagesz);
    }

  /*
   * 1)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (segment_get(segid, &segment) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the segment object");

  /*
   * 3)
   */

  if (segment->size < (offset + size))
    CORE_ESCAPE("out-of-bound operation");

  /*
   * 4)
   */

  if (options & REGION_OPTION_FORCE)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      if ((address < _region.base) ||
	  (address >= (_region.base + _region.size)))
	CORE_ESCAPE("the provided address is either too low or too high");

      /*
       * b)
       */

      set_foreach(SET_OPTION_FORWARD, as->regions, &i, state)
	{
	  if (set_object(as->regions, i, (void**)&data) != STATUS_OK)
	    CORE_ESCAPE("unable to retrieve the region object");

	  if (data->address <= address && data->address + data->size > address)
	    CORE_ESCAPE("unable to reserve the region at this precise "
			"location 0x%x: seems to be already taken",
			address);
	}
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      if (region_space(asid, size, &address) != STATUS_OK)
	CORE_ESCAPE("unable to find available space for the region "
		    "reservation");
    }

  /*
   * 5)
   */

  if (region_protect(asid, address, size) != STATUS_OK)
    CORE_ESCAPE("unable to protect the zone");

  /*
   * 6)
   */

  if ((o = malloc(sizeof (o_region))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the region object");

  /*
   * 7)
   */

  o->address = address;
  o->id = REGION_IDENTIFIER(o);
  o->segment = segid;
  o->size = size;
  o->offset = offset;
  o->options = options;

  /*
   * 8)
   */

  if (set_add(as->regions, o) != STATUS_OK)
    CORE_ESCAPE("unable to add the object to the address space's set of "
		"regions");

  /*
   * 9)
   */

  if (region_unprotect(asid, address, size) != STATUS_OK)
    CORE_ESCAPE("unable to unprotect the zone");

  /*
   * 10)
   */

  if (machine_call(region, reserve,
		   asid, segid, offset, options,
		   address, size, region) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 11)
   */

  *region = o->id;

  CORE_LEAVE();
}

/*
 * this function releases an existing region from the given address space.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the address space object.
 * 3) retrieve the region object.
 * 4) remove the region from the address space's set of regions.
 */

t_status		region_release(i_as			asid,
				       i_region			regid)
{
  o_as*			as;

  /*
   * 1)
   */

  if (machine_call(region, release, asid, regid) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  if (set_remove(as->regions, regid) != STATUS_OK)
    CORE_ESCAPE("unable to remove the object from the address space's "
		"set of regions");

  CORE_LEAVE();
}

/*
 * this function locates the region in which the given address lies.
 *
 * note that this function returns either true or false depending on the
 * success of the look up.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) go through the regions.
 *   a) retrieve the region object.
 *   b) check that the address resides in this region.
 * 3) if no region has been found, return false;
 */

t_bool			region_locate(i_as			as,
				      t_vaddr			address,
				      i_region*			id)
{
  o_as*			o;
  t_state		state;
  s_iterator		i;

  /*
   * 0)
   */

  assert(id != NULL);

  /*
   * 1)
   */

  assert(as_get(as, &o) == STATUS_OK);

  /*
   * 2)
   */

  set_foreach(SET_OPTION_FORWARD, o->regions, &i, state)
    {
      o_region*		r;

      /*
       * a)
       */

      assert(set_object(o->regions, i, (void**)&r) == STATUS_OK);

      /*
       * b)
       */

      if ((r->address <= address) && (address <= (r->address + r->size - 1)))
	{
	  *id = r->id;

	  CORE_TRUE();
	}
    }

  /*
   * 3)
   */

  CORE_FALSE();
}

/*
 * this function removes every region that belongs to the give address space.
 *
 * steps:
 *
 * 1) call the machine.
 * 2) retrieve the address space object.
 * 3) go through the existing regions and release them all.
 */

t_status		region_flush(i_as			asid)
{
  o_as*			as;
  s_iterator		it;
  i_region*		obj;

  /*
   * 1)
   */

  if (machine_call(region, flush, asid) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 3)
   */

  while (set_head(as->regions, &it) == TRUE)
    {
      if (set_object(as->regions, it, (void**)&obj) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the region object");

      if (region_release(asid, *obj) != STATUS_OK)
	CORE_ESCAPE("unable to release the region");
    }

  CORE_LEAVE();
}

/*
 * this function returns true if the region object exists.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) check if the region identifier can be located in the address space's
 *    set of regions.
 */

t_status		region_exist(i_as			asid,
				     i_region			regid)
{
  o_as*			as;

  /*
   * 0)
   */

  assert(as_get(asid, &as) == STATUS_OK);

  /*
   * 1)
   */

  if (set_exist(as->regions, regid) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns a region object from the given address space.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) retrieve the object from the address space's set of regions.
 */

t_status		region_get(i_as				asid,
				   i_region			regid,
				   o_region**			object)
{
  o_as*			as;

  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (as_get(asid, &as) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (set_get(as->regions, regid, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the region from the address space's set "
		"of regions");

  CORE_LEAVE();
}

/*
 * this function initializes the region manager.
 *
 * the _base_ and _size_ arguments represents the virtual space i.e its
 * base address and available size.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) display a message.
 * 2) initialize the region manager's structure.
 * 3) set the base and size attributes. note that the base address is
 *    incremented by the page size. this is done in order to render the
 *    first page unuseable. this has the impact to produce a page fault
 *    every time the first page is referenced. this is a common practice
 *    since most pointers are manually initialised by programmers to NULL.
 * 4) calls the machine.
 */

t_status		region_initialize(t_vaddr		base,
					  t_vsize		size)
{
  /*
   * 0)
   */

  if (size == 0)
    CORE_ESCAPE("unable to initialize the region manager with a size of zero");

  if ((base % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the base is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  if ((size % ___kaneton$pagesz) != 0)
    CORE_ESCAPE("the size is not aligned on the machine's page size: '%u'",
		___kaneton$pagesz);

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the region manager\n");

  /*
   * 2)
   */

  memset(&_region, 0x0, sizeof (m_region));

  /*
   * 3)
   */

  _region.base = base + ___kaneton$pagesz;
  _region.size = size - ___kaneton$pagesz;

  /*
   * 4)
   */

  if (machine_call(region, initialize, base, size) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the region manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 */

t_status		region_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the region manager\n");

  /*
   * 2)
   */

  if (machine_call(region, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

#endif
