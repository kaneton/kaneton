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
 * updated       julien quintard   [mon apr 11 13:39:32 2011]
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
 * as a page table and its entries as page tables entries.
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
 * quite obvious---enables the kernel to modify any kernel page table without
 * mapping any intermediate page table. indeed, the kernel only needs
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
 * the init structure.
 */

extern s_init*		_init;

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * the address space manager which contains the kernel PD.
 */

extern m_as		_as;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function enables the paging mechanism by setting the higher
 * bit of CR0. besides, the global pages support is activated by setting
 * the bit 7 of CR4.
 */

t_status		architecture_paging_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "orl $0x80000000, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");

  asm volatile("movl %%cr4, %%eax\n\t"
	       "orl $128, %%eax\n\t"
	       "movl %%eax, %%cr4\n\t"
	       :
	       :
	       : "%eax");

  MACHINE_LEAVE();
}

/*
 * this function sets up the paging by importing the boot loader's page
 * directory.
 *
 * steps:
 *
 * 1) import the boot loader's page directory.
 * 2) enable the paging mechanism.
 */

t_status		architecture_paging_setup(void)
{
  /*
   * 1)
   */

  _as.machine.pd = (at_pd)_init->machine.pd;

  /*
   * 2)
   */

  if (architecture_paging_enable() != STATUS_OK)
    MACHINE_ESCAPE("unable to enable the paging mechanism");

  MACHINE_LEAVE();
}

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

t_status		architecture_paging_pdbr(t_paddr	pd,
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
 * this function imports the given page directory, making it the
 * current one.
 *
 * steps:
 *
 * 1) set the new page directory
 * 2) install the new CR3.
 */

t_status		architecture_paging_import(at_pd	pd,
						   at_cr3	cr3)
{
  /*
   * 1)
   */

  _as.machine.pd = pd;

  /*
   * 2)
   */

  ARCHITECTURE_LCR3(cr3);

  MACHINE_LEAVE();
}

/*
 * this function exports the current CR3 hence page directory.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the current page directory.
 * 2) retrieve the current CR3.
 */

t_status		architecture_paging_export(at_pd*	pd,
						   at_cr3*	cr3)
{
  /*
   * 0)
   */

  if (pd == NULL)
    MACHINE_ESCAPE("the 'pd' argument is null");

  if (cr3 == NULL)
    MACHINE_ESCAPE("the 'cr3' argument is null");

  /*
   * 1)
   */

  *pd = _as.machine.pd;

  /*
   * 2)
   */

  ARCHITECTURE_SCR3(*cr3);

  MACHINE_LEAVE();
}

/*
 * this function maps a portion of the given segment to the given virtual
 * address.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) retrieve the segment object.
 * 3) return an error if the physical address is not aligned on a page size.
 * 4) retrieve the page directory depending on the address space's nature.
 *   A) if the mapping does not occur within the kernel's address space,
 *      map the address space's page directory.
 *   B) otherwise, retrieve the page directory virtual address through
 *      the mirroring mechanism.
 * 5) compute the physical and virtual address.
 * 6) initialize the page directory and table start/end indexes corresponding
 *    to the beginning and end of the virtual region that is about to
 *    be created.
 * 7) go through the page directory entries involved in the mapping.
 *   a) if there is no more pages to map, exit the loop.
 *   b) depending on the existence of the page directory entry.
 *     1#) if the entry does not exist...
 *       i) reserve a segment for the page table.
 *       ii) retrieve the segment object.
 *       iii) update the page directory by referencing the new page table.
 *       iv) map the page table. note that for the kernel address space,
 *          the mirroring mechanism could be used instead of this step.
 *       v) build the page table.
 *     2#) otherwise...
 *       i) map the page table. note that for the kernel address space,
 *          the mirroring mechanism could be used instead of this step.
 *   c) go through the involved page table's entries.
 *     i) if the page table entry is already in use, return an error
 *        since this case should never occur.
 *     ii) update the page table, referencing one of the page to map.
 *     iii) update the variables i.e moving on to the next page of the
 *          region to map and decreasing the size.
 *   d) unmap the page table.
 * 8) if the mapping occurs in a task's address space, i.e not the kernel
 *    address space, unmap the page directory.
 */

t_status		architecture_paging_map(i_as		id,
						i_segment	segment,
						t_paddr		offset,
						t_options	options,
						t_vaddr		address,
						t_vsize		size)
{
  struct
  {
    at_pdei		start;
    at_pdei		end;
    at_pdei		index;
  }			pde;
  struct
  {
    at_ptei		start;
    at_ptei		end;
    at_ptei		index;
  }			pte;
  t_paddr		paddr;
  t_vaddr		vaddr;
  o_as*			as;
  at_pd			pd;
  at_pt			pt;
  o_segment*		o;

  /*
   * 0)
   */

  if (size % ___kaneton$pagesz)
    MACHINE_ESCAPE("the mapping size is not a multiple of the page size");

  if (address % ___kaneton$pagesz)
    MACHINE_ESCAPE("the virtual address is not aligned on the page size");

  /*
   * 1)
   */

  if (as_get(id, &as) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (segment_get(segment, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable t");

  /*
   * 3)
   */

  if ((o->address + offset) % ___kaneton$pagesz)
    MACHINE_ESCAPE("the physical address to map is not aligned on the "
		   "page size");

  /*
   * 4)
   */

  if (as->id != _kernel.as)
    {
      /*
       * A)
       */

      if (architecture_pd_map(as->machine.pd, &pd) != STATUS_OK)
	MACHINE_ESCAPE("unable to map the page directory");
    }
  else
    {
      /*
       * B)
       */

      pd = (at_pd)ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
					      ARCHITECTURE_PT_MIRROR);
    }

  /*
   * 5)
   */

  paddr = o->address + offset;
  vaddr = address;

  /*
   * 6)
   */

  pde.start = ARCHITECTURE_PD_INDEX(vaddr);
  pde.end = ARCHITECTURE_PD_INDEX(vaddr + size);

  pte.start = ARCHITECTURE_PT_INDEX(vaddr);
  pte.end = ARCHITECTURE_PT_INDEX(vaddr + size);

  /*
   * 7)
   */

  for (pde.index = pde.start;
       pde.index <= pde.end;
       pde.index++)
    {
      /*
       * a)
       */

      if (size == 0)
	break;

      /*
       * b)
       */

      if (!(pd[pde.index] & ARCHITECTURE_PDE_PRESENT))
	{
	  i_segment	i;
	  o_segment*	s;

	  /*
	   * 1#)
	   */

	  /*
	   * i)
	   */

	  if (segment_reserve(as->id,
			      ___kaneton$pagesz,
			      PERMISSION_READ | PERMISSION_WRITE,
			      SEGMENT_OPTION_SYSTEM,
			      &i) != STATUS_OK)
	    MACHINE_ESCAPE("unable to reserve a segment");

	  /*
	   * ii)
	   */

	  if (segment_get(i, &s) != STATUS_OK)
	    MACHINE_ESCAPE("unable to retrieve the segment object");

	  /*
	   * iii)
	   */

	  if (architecture_pd_insert(pd,
				     pde.index,
				     s->address,
				     ARCHITECTURE_PDE_PRESENT |
				     ARCHITECTURE_PDE_RW |
				     ARCHITECTURE_PDE_USER |
				     ARCHITECTURE_PDE_PWB |
				     ARCHITECTURE_PDE_PCE) != STATUS_OK)
	    MACHINE_ESCAPE("unable to update the page directory");

	  /*
	   * iv)
	   */

	  if (architecture_pt_map(s->address, &pt) != STATUS_OK)
	    MACHINE_ESCAPE("unable to map the page table");

	  /*
	   * v)
	   */

	  if (architecture_pt_build(pt) != STATUS_OK)
	    MACHINE_ESCAPE("unable to build the page table");
	}
      else
	{
	  /*
	   * 2#)
	   */

	  /*
	   * i)
	   */

	  if (architecture_pt_map(ARCHITECTURE_PDE_ADDRESS(pd[pde.index]),
				  &pt) != STATUS_OK)
	    MACHINE_ESCAPE("unable to map the page table");
	}

      /*
       * c)
       */

      for (pte.index = (pde.index == pde.start ? pte.start : 0);
	   pte.index < (pde.index == pde.end ? pte.end : ARCHITECTURE_PT_SIZE);
	   pte.index++)
	{
	  /*
	   * i)
	   */

	  if (pt[pte.index] & ARCHITECTURE_PTE_PRESENT)
	    MACHINE_ESCAPE("this page table entry is already in use");

	  /*
	   * ii)
	   */

	  if (architecture_pt_insert(pt,
				     pte.index,
				     paddr,
				     ARCHITECTURE_PTE_PRESENT |
				     (o->permissions & PERMISSION_WRITE ?
				       ARCHITECTURE_PTE_RW :
				       ARCHITECTURE_PTE_RO) |
				     (o->options & SEGMENT_OPTION_SYSTEM ?
				       ARCHITECTURE_PTE_SUPERVISOR :
				       ARCHITECTURE_PTE_USER) |
				     ARCHITECTURE_PTE_PWB |
				     ARCHITECTURE_PTE_PCE) != STATUS_OK)
	    MACHINE_ESCAPE("unable to update the page table");

	  /*
	   * iii)
	   */

	  paddr += ___kaneton$pagesz;

	  size -= ___kaneton$pagesz;
	}

      /*
       * d)
       */

      if (architecture_pt_unmap(pt) != STATUS_OK)
	MACHINE_ESCAPE("unable to unmap the page table");
    }

  /*
   * 8)
   */

  if (as->id != _kernel.as)
    {
      if (architecture_pd_unmap(pd) != STATUS_OK)
	MACHINE_ESCAPE("unable to unmap the page directory");
    }

  MACHINE_LEAVE();
}

/*
 * this function unmaps the mappings associated with the given address and
 * size.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the address space object.
 * 2) depending on the address space's nature.
 *   A) if the operation does not occurs in the kernel address space,
 *      map the page directory.
 *   B) otherwise, retrieve the kernel page directory's virtual address
 *      through the mirroring mechanism.
 * 3) compute the page directory and table indexes associated with the
 *    given mapping.
 * 4) go through the invovled page directory entries.
 *   a) if the page directory entry is not used, return an error.
 *   b) map the page table.
 *   c) go through the involved page table entries.
 *     i) delete the page table entry mapping one of the mapping's page.
 *     ii) if the operation occurs within the kernel's address space,
 *         invalidates the virtual address to prevent inconsistent cache
 *         translations.
 *   d) unmap the page table.
 *   e) if all the page table entries have been deleted, delete the whole
 *      page table.
 *     i) delete the page directory entry referencing the page table.
 *     ii) invalidate the page table address if within the kernel
 *         address space.
 *     iii) locate the segment in which the page table lies.
 *     iv) release the segment.
 * 5) finally, unmap the page directory if not the kernel's.
 */

t_status		architecture_paging_unmap(i_as		id,
						  t_vaddr	address,
						  t_vsize	size)
{
  struct
  {
    at_pdei		start;
    at_pdei		end;
    at_pdei		index;
  }			pde;
  struct
  {
    at_ptei		start;
    at_ptei		end;
    at_ptei		index;
  }			pte;
  o_as*			as;
  at_pd			pd;
  at_pt			pt;

  /*
   * 0)
   */

  if (size % ___kaneton$pagesz)
    MACHINE_ESCAPE("the mapping size is not a multiple of the page size");

  if (address % ___kaneton$pagesz)
    MACHINE_ESCAPE("the virtual address is not aligned on the page size");

  /*
   * 1)
   */

  if (as_get(id, &as) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (as->id != _kernel.as)
    {
      /*
       * A)
       */

      if (architecture_pd_map(as->machine.pd, &pd) != STATUS_OK)
	MACHINE_ESCAPE("unable to map the page directory");
    }
  else
    {
      /*
       * B)
       */

      pd = (at_pd)ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
					      ARCHITECTURE_PT_MIRROR);
    }

  /*
   * 3)
   */

  pde.start = ARCHITECTURE_PD_INDEX(address);
  pde.end = ARCHITECTURE_PD_INDEX(address + size);

  pte.start = ARCHITECTURE_PT_INDEX(address);
  pte.end = ARCHITECTURE_PT_INDEX(address + size);

  /*
   * 4)
   */

  for (pde.index = pde.start;
       pde.index <= pde.end;
       pde.index++)
    {
      /*
       * a)
       */

      if (!(pd[pde.index] & ARCHITECTURE_PDE_PRESENT))
	MACHINE_ESCAPE("there is no page table referencing this page");

      /*
       * b)
       */

      if (architecture_pt_map(ARCHITECTURE_PDE_ADDRESS(pd[pde.index]),
			      &pt) != STATUS_OK)
	MACHINE_ESCAPE("unable to map the page table");

      /*
       * c)
       */

      for (pte.index = (pde.index == pde.start ? pte.start : 0);
	   pte.index < (pde.index == pde.end ? pte.end : ARCHITECTURE_PT_SIZE);
	   pte.index++)
	{
	  /*
	   * i)
	   */

	  if (architecture_pt_delete(pt, pte.index) != STATUS_OK)
	    MACHINE_ESCAPE("unable to delete the page table entry");

	  /*
	   * ii)
	   */

	  if (as->id == _kernel.as)
	    {
	      if (architecture_tlb_invalidate(
                    ARCHITECTURE_PAGING_ADDRESS(pde.index,
						pte.index)) != STATUS_OK)
		MACHINE_ESCAPE("unable to invalidate the address");
	    }
	}

      /*
       * d)
       */

      if (architecture_pt_unmap(pt) != STATUS_OK)
	MACHINE_ESCAPE("unable to unmap the page table");

      /*
       * e)
       */

      if (((pde.index != pde.start) && (pde.index != pde.end)) ||
	  ((pte.start == 0) && (pte.end == ARCHITECTURE_PT_SIZE)))
	{
	  i_segment	i;

	  /*
	   * i)
	   */

	  if (architecture_pd_delete(pd, pde.index) != STATUS_OK)
	    MACHINE_ESCAPE("unable to delete the page directory entry");

	  /*
	   * ii)
	   */

	  if (as->id == _kernel.as)
	    {
	      if (architecture_tlb_invalidate(
		    ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
						pde.index)) != STATUS_OK)
		MACHINE_ESCAPE("unable to invalidate the address");
	    }

	  /*
	   * iii)
	   */

	  if (segment_locate(ARCHITECTURE_PDE_ADDRESS(pd[pde.index]),
			     &i) == FALSE)
	    MACHINE_ESCAPE("unable to locate the segment holding the "
			   "page table");

	  /*
	   * iv)
	   */

	  if (segment_release(i) != STATUS_OK)
	    MACHINE_ESCAPE("unable to release the segment");
	}
    }

  /*
   * 5)
   */

  if (as->id != _kernel.as)
    {
      if (architecture_pd_unmap(pd) != STATUS_OK)
	MACHINE_ESCAPE("unable to unmap the page directory");
    }

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

t_status		architecture_paging_read(i_segment	id,
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

t_status		architecture_paging_write(i_segment	id,
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

t_status		architecture_paging_copy(i_region	dst,
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

/*
 * this function tests the virtual address is mapped in current address space
 */

t_status		architecture_paging_test(t_vaddr	vaddr,
                                                 t_boolean*	result)
{
  at_pde* pde;
  at_pte* pte;

  pde = (at_pde*) ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
                                              ARCHITECTURE_PT_MIRROR)
    + ARCHITECTURE_PD_INDEX(vaddr);

  pte = (at_pte*) ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
                                              ARCHITECTURE_PD_INDEX(vaddr))
    + ARCHITECTURE_PT_INDEX(vaddr);


  *result = (*pde & ARCHITECTURE_PDE_PRESENT)
    && (*pte & ARCHITECTURE_PTE_PRESENT);

  MACHINE_LEAVE();
}
