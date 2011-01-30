/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/environment.c
 *
 * created       julien quintard   [thu jan 13 23:13:50 2011]
 * updated       julien quintard   [sun jan 30 12:25:16 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for initializing the environments of a
 * task, especially regarding its address space.
 *
 * note that the kernel is treated separately from the servers i.e drivers,
 * services and guests.
 *
 * [XXX:improvement] in the server initialization, needless to map the kernel
 *                   code and stack. instead the handler shells should be
 *                   mapped i.e the .handler section.
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

extern m_kernel*	_kernel;

/*
 * the thread manager.
 */

extern m_thread*	_thread;

/*
 * the segment manager.
 */

extern m_segment*	_segment;

/*
 * the event manager.
 */

extern m_event*		_event;

/*
 * the architecture manager.
 */

extern am		_architecture;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initializes the kernel's environment.
 *
 * steps:
 *
 * 1) retrieve the address space objec.
 * 2) set the kernel address space's page directory by importing the
 *    page directory set up by the boot loader.
 * 3) generate the PDBR - Page Directory Base Register, also known as
 *    the CR3, based on the kernel page directory's physical address and
 *    some flags.
 * 4) set the page directory virtual address as being an identity mapping
 *    of the physical address. this is how the boot loader set things up.
 * 5) set the current page directory as being the kernel's one by updating
 *    the microprocessor CR3 register.
 * 6) update the kernel page directory---which is assumed to have been mapped
 *    by the boot loader through the identity mapping technique---in order
 *    to set up the mirroring entry. this entry wastes the last 4MB
 *    of memory and are used for accessing the kernel page directory and
 *    tables without mapping anything, hence preventing infinite loops.
 *    note that the entry references the page directory itself, making the
 *    page directory act as a page table whenever accessed through the
 *    mirror page directory entry.
 * 7) the last 4MB of virtual memory are not accessible since the mirroring
 *    page directory entry and the referenced page table's entries---the
 *    page directory itself---are used for the mirroring mechanism. these
 *    4MB are wasted and must therefore not be reservable or the kernel may
 *    end up overwritten the mirroring entries. a region covering the last
 *    4MB of virtual memory is therefore injected.
 *    note that the region injected references an ID_UNUSED-identified
 *    segment in order to avoid having to reserve a 4MB segment. this is
 *    possible because region_inject() does not check if the referenced
 *    segment identifer is valid.
 * 8) this step consists in cleaning the page directory set up by the boot
 *    loader, now used by the kernel, by unmapping any page which is not
 *    related to the fundamental regions provided by the boot loader.
 *    go through all the pre-reserved regions provided by the bootloader plus
 *    one. this additional iteration is required in order to clean the
 *    mapped pages from the last region to the end of the virtual address
 *    space.
 *   a) compute the page directory and table end indexes for the given
 *      region. note that for the extra iteration, the end indexes are
 *      set to their maximum so that every page table entry of every page
 *      directory entry following the last region is cleaned.
 *   b) go through the involved page directory entries.
 *     i) if the page directory entry does not reference a page table or
 *        is used as the mirroring entry, leave it. otherwise...
 *       #1) retrieve the page table referenced by the page directory entry.
 *           note that the boot loader relied on the identity mapping technique
 *           for its paging set up. identity mapping is therefore used to
 *           retrieve the page table virtual address.
 *       #2) go through the page table's involved entries.
 *         #a) if the page table entry is used, delete the reference as
 *             this mapping must not be very important since not related to
 *             the pre-reserved regions provided by the boot loader.
 *   c) if the treated region is not the extra one, compute the next
 *      page directory and table start indexes as starting right after
 *      the end of the region i.e address + size.
 * 9) the page directory imported by the kernel has been set up by the
 *    boot loader. besides, the boot loader has passed to the kernel a list
 *    of pre-reserved system segments and regions. the segments are or will
 *    be injected in the segment manager and likewise for the regions, in the
 *    region manager. however, the page directory provided by the boot loader
 *    makes use of several page tables which have not been passed to the
 *    kernel as pre-reserved segments. the physical memory pages containing
 *    these page tables must be protected so they never get reserved and
 *    overwritten by a task or the kernel itself. this step therefore goes
 *    through the page directory and inject a segment in the segment manager
 *    for every unprotected page table.
 *    go through the regions as these are the only elements being currently
 *    mapped.
 *   a) compute the start and end indexes of the region's memory area.
 *   b) go through the involved page directory entries.
 *     i) if there is a referenced page table...
 *       #1) if there is no segment associated with the page table's physical
 *           address...
 *         #a) create a segment object which describes the page table's
 *             memory area.
 *         #b) inject the segment, hence preventing this system memory
 *             area from being reserved by anyone.
 * 10) flush the whole TLB, resetting all the address translations.
 * 11) register the kernel PDBR as being the PDBR on which to switch whenever
 *     an interrupt occurs.
 */

t_error			architecture_environment_kernel(i_as	id)
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
  i_region		useless;
  at_cr3		pdbr;
  o_as*			as;
  at_pd			pd;
  at_pt			pt;
  o_region*		r;
  t_uint32		i;

  /*
   * 1)
   */

  if (as_get(id, &as) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  as->machine.pd = _init->machine.pd;

  /*
   * 3)
   */

  if (architecture_paging_pdbr(as->machine.pd,
			       ARCHITECTURE_REGISTER_CR3_PCE |
			       ARCHITECTURE_REGISTER_CR3_PWB,
			       &pdbr) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the CR3 register's content");

  /*
   * 4)
   */

  pd = (at_pd)as->machine.pd;

  /*
   * 5)
   */

  if (architecture_paging_import(pd,
				 pdbr) != ERROR_OK)
    MACHINE_ESCAPE("unable to import the kernel page directory");

  /*
   * 6)
   */

  if (architecture_pd_insert(pd,
			     ARCHITECTURE_PD_MIRROR,
			     as->machine.pd,
			     ARCHITECTURE_PDE_PRESENT |
			     ARCHITECTURE_PDE_RW |
			     ARCHITECTURE_PDE_SUPERVISOR |
			     ARCHITECTURE_PDE_PWB |
			     ARCHITECTURE_PDE_PCE) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the mirroring directory entry");

  /*
   * 7)
   */

  if ((r = malloc(sizeof (o_region))) == NULL)
    MACHINE_ESCAPE("unable to allocate memory for the region object");

  r->address = ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR, 0);
  r->segment = ID_UNUSED;
  r->offset = 0x0;
  r->size = ARCHITECTURE_PT_SIZE * ___kaneton$pagesz;
  r->options = REGION_OPTION_NONE;

  if (region_inject(as->id, r, &useless) != ERROR_OK)
    MACHINE_ESCAPE("unable to inject the mirroring region");

  /*
   * 8)
   */

  pde.start = 0;
  pte.start = 0;

  for (i = 0; i < (_init->nregions + 1); i++)
    {
      /*
       * a)
       */

      if (i != _init->nregions)
	{
	  pde.end = ARCHITECTURE_PD_INDEX(_init->regions[i].address);
	  pte.end = ARCHITECTURE_PT_INDEX(_init->regions[i].address);
	}
      else
	{
	  pde.end = ARCHITECTURE_PD_SIZE - 1;
	  pte.end = ARCHITECTURE_PT_SIZE;
	}

      /*
       * b)
       */

      for (pde.index = pde.start;
	   pde.index <= pde.end;
	   pde.index++)
	{
	  /*
	   * i)
	   */

	  if ((pde.index != ARCHITECTURE_PD_MIRROR) &&
	      (pd[pde.index] & ARCHITECTURE_PDE_PRESENT))
	    {
	      /*
	       * #1)
	       */

	      pt = (at_pt)ARCHITECTURE_PDE_ADDRESS(pd[pde.index]);

	      /*
	       * #2)
	       */

	      for (pte.index = (pde.index == pde.start ? pte.start : 0);
		   pte.index < (pde.index == pde.end ?
				pte.end : ARCHITECTURE_PT_SIZE);
		   pte.index++)
		{
		  /*
		   * #a)
		   */

		  if (pt[pte.index] & ARCHITECTURE_PTE_PRESENT)
		    {
		      if (architecture_pt_delete(pt, pte.index) != ERROR_OK)
			MACHINE_ESCAPE("unable to delete the page "
				       "table entry");
		    }
		}
	    }
	}

      /*
       * c)
       */

      if (i != _init->nregions)
	{
	  pde.start = ARCHITECTURE_PD_INDEX(_init->regions[i].address +
					    _init->regions[i].size);
	  pte.start = ARCHITECTURE_PT_INDEX(_init->regions[i].address +
					    _init->regions[i].size);
	}
    }

  /*
   * 9)
   */

  for (i = 0; i < _init->nregions; i++)
    {
      /*
       * a)
       */

      pde.start = ARCHITECTURE_PD_INDEX(_init->regions[i].address);
      pde.end = ARCHITECTURE_PD_INDEX(_init->regions[i].address +
				      _init->regions[i].size);

      /*
       * b)
       */

      for (pde.index = pde.start;
	   pde.index <= pde.end;
	   pde.index++)
	{
	  /*
	   * i)
	   */

	  if (pd[pde.index] & ARCHITECTURE_PDE_PRESENT)
	    {
	      i_segment		segment;
	      o_segment*	s;

	      /*
	       * #1)
	       */

	      if (segment_locate(ARCHITECTURE_PDE_ADDRESS(pd[pde.index]),
				 &segment) == ERROR_FALSE)
		{
		  /*
		   * #a)
		   */

		  if ((s = malloc(sizeof (o_segment))) == NULL)
		    MACHINE_ESCAPE("unable to allocate memory");

		  s->address = ARCHITECTURE_PDE_ADDRESS(pd[pde.index]);
		  s->size = ___kaneton$pagesz;
		  s->permissions = PERMISSION_READ | PERMISSION_WRITE;
		  s->options = SEGMENT_OPTION_SYSTEM;

		  /*
		   * #b)
		   */

		  if (segment_inject(as->id, s, &segment) != ERROR_OK)
		    MACHINE_ESCAPE("unable to inject the segment object");
		}
	    }
	}
    }

  /*
   * 10)
   */

  if (architecture_tlb_flush() != ERROR_OK)
    MACHINE_ESCAPE("unable to flush the TLB");

  /*
   * 11)
   */

  _architecture.kernel.pdbr = pdbr;

  MACHINE_LEAVE();
}

/*
 * this function sets up the environment of a server i.e drivers, services
 * and guests.
 *
 * steps:
 *
 * 1) retrieve the address space object.
 * 2) reserve a system segment.
 * 3) use this segment for the given address space's page directory.
 * 4) map the page directory, initialize it and unmap it.
 * 5) locate the segment containing the system's TSS and map it in
 *    the given address space. note that the TSS is mapped at the same
 *    virtual address as in the kernel.
 * 6) locate the segment containing the system's GDT and map it in the
 *    given address space, again at the same virtual address as the kernel's.
 * 7) locate the segment containing the system's IDT and map it in the
 *    given address space, again at the same virtual address as the kernel's.
 * 8) locate the segment containing the kernel code and map it in the given
 *    address space. note that the identity mapping technique is used here.
 * 9) locate the segment containing the kernel stack and map it in the given
 *    address space, note that the identity mapping technique is used here.
 */

t_error			architecture_environment_server(i_as	id)
{
  i_segment	        segment;
  i_region		region;
  o_as*			as;
  at_pd			pd;
  o_region*		r;
  o_segment*		s;

  /*
   * 1)
   */

  if (as_get(id, &as) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (segment_reserve(as->id,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_SYSTEM,
		      &segment) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a segment");

  if (segment_get(segment, &s) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the segment object");

  /*
   * 3)
   */

  as->machine.pd = s->address;

  /*
   * 4)
   */

  if (architecture_pd_map(as->machine.pd, &pd) != ERROR_OK)
    MACHINE_ESCAPE("unable to map the page directory");

  if (architecture_pd_build(pd) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the page directory");

  if (architecture_pd_unmap(pd) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the page directory");

  /*
   * 5)
   */

  if (region_locate(_kernel->as,
		    _thread->machine.tss,
		    &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region in which the TSS lies");

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  if (region_reserve(as->id,
		     r->segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_NONE,
		     _thread->machine.tss,
		     r->size,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the TSS");

  /*
   * 6)
   */

  if (region_locate(_kernel->as,
		    (t_vaddr)_segment->machine.gdt.table,
		    &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region in which the GDT lies");

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  if (region_reserve(as->id,
		     r->segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_NONE,
		     (t_vaddr)_segment->machine.gdt.table,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the GDT");

  /*
   * 7)
   */

  if (region_locate(_kernel->as,
		    (t_vaddr)_event->machine.idt.table,
		    &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region in which the IDT lies");

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  if (region_reserve(as->id,
		     r->segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_NONE,
		     (t_vaddr)_event->machine.idt.table,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the IDT");

  /*
   * 8)
   */

  /* XXX
     if (region_reserve(asid,
     _init->kcode,
     LINKER_SYMBOL(_handler_begin) - _init->kcode,
     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED,
     LINKER_SYMBOL(_handler_begin),
     LINKER_SYMBOL(_handler_end) -
     LINKER_SYMBOL(_handler_begin),
     &reg) != ERROR_OK)

     if (region_reserve(asid,
     _init->kcode,
     LINKER_SYMBOL(_handler_data_begin) - _init->kcode,
     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED,
     LINKER_SYMBOL(_handler_data_begin),
     LINKER_SYMBOL(_handler_data_end) -
     LINKER_SYMBOL(_handler_data_begin),
     &reg) != ERROR_OK)
  */

  if (segment_locate(_init->kcode, &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the segment which contains the "
		   "kernel code");

  if (region_reserve(as->id,
		     segment,
		     0x0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->kcode,
		     (t_vsize)_init->kcodesz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the kernel code");

  /*
   * 9)
   */

  if (segment_locate(_init->kstack,
		     &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the segment which contains the "
		   "kernel stack");

  if (region_reserve(as->id,
		     segment,
		     0x0,
		     REGION_OPTION_FORCE,
		     (t_vaddr)_init->kstack,
		     (t_vsize)_init->kstacksz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the kernel stack");

  MACHINE_LEAVE();
}
