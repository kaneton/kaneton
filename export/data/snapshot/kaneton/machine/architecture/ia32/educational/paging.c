/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/architecture/ia32/educational/paging.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:05 2005]
 * updated       julien quintard   [tue apr 12 07:34:31 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file provides basic paging functionalities.
 *
 * note that the ia32/educational implementation makes use of the mirroring
 * technique. this technique is used to prevent the kernel from looping
 * infinitely whenever trying to map a page table for instance. indeed,
 * whenever a page table must be mapped, a virtual memory area is picked.
 * this memory area must be mapped by creating a mapping in the page
 * directory/table hierarchical structure. in order to do so, the kernel page
 * directory and the kernel page table referencing the given area must
 * also be mapped. should not this be the case, the referencing page table,
 * for example, should be mapped by creating a mapping etc. there is an
 * obvious infinite loop occuring here where every page table which has
 * to be mapped implies another page table to reference it, hence to be
 * mapped as well.
 *
 * in order to prevent this infinite loop, the kernel relies on the
 * mirroring technique. this mechanism consists in setting up a kernel
 * page directory entry as acting as a special loopback. for example, the
 * last page directory entry, i.e pd[1023], will not reference another page
 * table as it is the case for the other page directory entries but the
 * page directory itself. this way, the page directory, whenever the
 * mirroring page directory entry is used, is considered by the microprocessor
 * as page table and its entries as page tables entries.
 *
 * the following schema depicts this organization:
 *
 * pd @ 0x011e2000
 *   pde 0 references 0x011e3000
 *     pt @ 0x011e3000
 *       pte 0 not present
 *       pte 1 references 0x00001000
 *       pte 2 references 0x00002000
 *       pte 3 references 0x00003000
 *       pte 4 references 0x00004000
 *       ...
 *   pde 2 not present
 *   pde 3 not present
 *   pde 4 references 0x011e4000
 *     pt @ 0x011e4000
 *       pte 0 references 0x01000000
 *       pte 1 references 0x01001000
 *       pte 2 references 0x01002000
 *   ...
 *   pde 1023 references 0x011e2000 (the page directory itself)
 *     pt @ 0x011e2000 (the page directory acts as a page table)
 *       pte 0 references 0x011e3000 (the first page table: pde 0)
 *       pte 4 references 0x011e4000 (the second page table: pde 1)
 *       pte 1023 references 0x011e2000 (the page directory: pde 1023)
 *
 * this technique---assuming the kernel page directory is mapped, but this is
 * quite obvious---enables the kernel to modify any page directory/table
 * without mapping any intermediate page table. indeed, the kernel only needs
 * to reference the page table it wishes to modify through its mirrored
 * page directory entry.
 *
 * this technique implies that the last page directory entry is reserved
 * for the mirroring mechanism. therefore, the last 4MB of virtual memory
 * must be locked for that purpose meaning that 4MB of memory are wasted.
 *
 * finally, the reader should have understood that the mirroring mechanism
 * only applies to the kernel tables. indeed, other address spaces'
 * page directories and tables can be mapped normally since, should
 * a kernel table need to be mapped to access another non-kernel table, it
 * will be through the mirroring mechanism.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function generates the CR3 register's content so that to be
 * overwritten, hence referencing another page directory structure.
 *
 * note that the CR3 register is also referred to as the PDBR - Page Directory
 * Base Register as it contains the address of the page directory in use.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) generate the CR3 register's content.
 */

t_status			architecture_paging_pdbr(t_paddr	pd,
						 t_flags	flags,
						 at_cr3*	pdbr)
{
  /*
   * 0)
   */

  if (pdbr == NULL)
    MACHINE_ESCAPE("the 'pdbr' argument is null");

  /*
   * 1)
   */

  *pdbr = (pd & 0xfffff000) | flags;

  MACHINE_LEAVE();
}

/*
 * this function maps a portion of the given segment to the given virtual
 * address.
 */

t_status			architecture_paging_map(i_as		id,
						i_segment	segment,
						t_paddr		offset,
						t_options	options,
						t_vaddr		address,
						t_vsize		size)
{
  /* FIXME[code to complete] */

  MACHINE_LEAVE();
}

/*
 * this function unmaps the mappings associated with the given address and
 * size.
 */

t_status			architecture_paging_unmap(i_as		id,
						  t_vaddr	address,
						  t_vsize	size)
{
  /* FIXME[code to complete] */

  MACHINE_LEAVE();
}

/*
 * this function reads data from a given segment by temporarily mapping
 * the necessary pages.
 *
 * note that this function supports non-aligned addresses and sizes.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object.
 * 2) compute the aligned segment offset along with the non-aligned shift.
 * 3) compute the last aligned page boundary.
 * 4) reserve a region for the involved pages.
 * 5) retrieve the region object.
 * 6) copy data from the mapped region into the given buffer.
 * 7) release the region.
 */

t_status			architecture_paging_read(i_segment	id,
						 t_paddr	offset,
						 void*		buffer,
						 t_psize	size)
{
  t_paddr		shift;
  i_region		region;
  t_paddr		end;
  o_segment*		o;
  o_region*		r;

  /*
   * 0)
   */

  if (buffer == NULL)
    MACHINE_ESCAPE("the 'buffer' argument is null");

  /*
   * 1)
   */

  if (segment_get(id, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (offset % ___kaneton$pagesz)
    {
      shift = offset - (offset & ~(___kaneton$pagesz - 1));
      offset = offset & ~(___kaneton$pagesz - 1);
    }
  else
    {
      shift = 0;
    }

  /*
   * 3)
   */

  end = offset + shift + size;

  if (end % ___kaneton$pagesz)
    end = (end & ~(___kaneton$pagesz - 1)) + ___kaneton$pagesz;

  /*
   * 4)
   */

  if (region_reserve(_kernel.as,
		     id,
		     offset,
		     REGION_OPTION_NONE,
		     0x0,
		     end - offset,
		     &region) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve a region");

  /*
   * 5)
   */

  if (region_get(_kernel.as, region, &r) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 6)
   */

  memcpy(buffer, (void*)r->address + shift, size);

  /*
   * 7)
   */

  if (region_release(_kernel.as, region) != STATUS_OK)
    MACHINE_ESCAPE("unable to release the region");

  MACHINE_LEAVE();
}

/*
 * this function writes data to a given segment by temporarily mapping
 * the necessary pages.
 *
 * note that this function supports non-aligned addresses and sizes.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the segment object.
 * 2) compute the aligned segment offset along with the non-aligned shift.
 * 3) compute the last aligned page boundary.
 * 4) reserve a region for the involved pages.
 * 5) retrieve the region object.
 * 6) copy data from the given buffer to the mapped region.
 * 7) release the region.
 */

t_status			architecture_paging_write(i_segment	id,
						  t_paddr	offset,
						  const void*	buffer,
						  t_psize	size)
{
  t_paddr		shift;
  i_region		region;
  t_paddr		end;
  o_segment*		o;
  o_region*		r;

  /*
   * 0)
   */

  if (buffer == NULL)
    MACHINE_ESCAPE("the 'buffer' argument is null");

  /*
   * 1)
   */

  if (segment_get(id, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (offset % ___kaneton$pagesz)
    {
      shift = offset - (offset & ~(___kaneton$pagesz - 1));
      offset = offset & ~(___kaneton$pagesz - 1);
    }
  else
    {
      shift = 0;
    }

  /*
   * 3)
   */

  end = offset + shift + size;

  if (end % ___kaneton$pagesz)
    end = (end & ~(___kaneton$pagesz - 1)) + ___kaneton$pagesz;

  /*
   * 4)
   */

  if (region_reserve(_kernel.as,
		     id,
		     offset,
		     REGION_OPTION_NONE,
		     0x0,
		     end - offset,
		     &region) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve a region");

  /*
   * 5)
   */

  if (region_get(_kernel.as, region, &r) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 6)
   */

  memcpy((void*)r->address + shift, buffer, size);

  /*
   * 7)
   */

  if (region_release(_kernel.as, region) != STATUS_OK)
    MACHINE_ESCAPE("unable to release the region");

  MACHINE_LEAVE();
}

/*
 * this function copies data from a segment to another by temporarily
 * mapping the necessary pages.
 *
 * note that this function supports non-aligned addresses and sizes.
 *
 * steps:
 *
 * 1) retrieve the segment objects.
 * 2) compute the source aligned offset and non-aligned shift.
 * 3) compute the source last aligned page boundary.
 * 4) map the source pages through a region reservation.
 * 5) retrieve the reserved region.
 * 6) compute the destination aligned offset and non-aligned shift.
 * 7) compute the destination last aligned page boundary.
 * 8) map the destination pages through a region reservation.
 * 9) retrieve the reserved region.
 * 10) copy from the source mapped region to the destination mapped region.
 * 11) release the reserved regions.
 */

t_status			architecture_paging_copy(i_region	dst,
						 t_paddr	to,
						 i_region	src,
						 t_paddr	from,
						 t_psize	size)
{
  struct
  {
    struct
    {
      o_segment*	object;
    }			segment;
    struct
    {
      i_region		id;
      o_region*		object;
    }			region;
    t_paddr		shift;
  }			source;
  struct
  {
    struct
    {
      o_segment*	object;
    }			segment;
    struct
    {
      i_region		id;
      o_region*		object;
    }			region;
    t_paddr		shift;
  }			destination;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(dst, &destination.segment.object) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the segment object");

  if (segment_get(src, &source.segment.object) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the segment object");

  /*
   * 2)
   */

  if (from % ___kaneton$pagesz)
    {
      source.shift = from - (from & ~(___kaneton$pagesz - 1));
      from = from & ~(___kaneton$pagesz - 1);
    }
  else
    {
      source.shift = 0;
    }

  /*
   * 3)
   */

  end = from + source.shift + size;

  if (end % ___kaneton$pagesz)
    end = (end & ~(___kaneton$pagesz - 1)) + ___kaneton$pagesz;

  /*
   * 4)
   */

  if (region_reserve(_kernel.as,
		     src,
		     from,
		     REGION_OPTION_NONE,
		     0x0,
		     end - from,
		     &source.region.id) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve a region");

  /*
   * 5)
   */

  if (region_get(_kernel.as,
		 source.region.id,
		 &source.region.object) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 6)
   */

  if (to % ___kaneton$pagesz)
    {
      destination.shift = to - (to & ~(___kaneton$pagesz - 1));
      to = to & ~(___kaneton$pagesz - 1);
    }
  else
    {
      destination.shift = 0;
    }

  /*
   * 7)
   */

  end = to + destination.shift + size;

  if (end % ___kaneton$pagesz)
    end = (end & ~(___kaneton$pagesz - 1)) + ___kaneton$pagesz;

  /*
   * 8)
   */

  if (region_reserve(_kernel.as,
		     dst,
		     to,
		     REGION_OPTION_NONE,
		     0x0,
		     end - to,
		     &destination.region.id) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve a region");

  /*
   * 9)
   */

  if (region_get(_kernel.as,
		 destination.region.id,
		 &destination.region.object) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 10)
   */

  memcpy((void*)destination.region.object->address + destination.shift,
	 (void*)source.region.object->address + source.shift,
	 size);

  /*
   * 11)
   */

  if (region_release(_kernel.as, source.region.id) != STATUS_OK)
    MACHINE_ESCAPE("unable to release the region");

  if (region_release(_kernel.as, destination.region.id) != STATUS_OK)
    MACHINE_ESCAPE("unable to release the region");

  MACHINE_LEAVE();
}
