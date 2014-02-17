/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/architecture/ia32/educational/pd.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:20 2005]
 * updated       julien quintard   [mon feb  7 10:55:07 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for managing PDs - Page Directories.
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
 * the address space manager which contains the kernel PD.
 */

extern m_as		_as;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the given mapped page directory.
 *
 * steps:
 *
 * 1) retrieve the given mapped page directory's physical address.
 * 2) display a general message.
 * 3) go through the present page directory entries.
 *   a) build the entry's flags string.
 *   b) display the page directory entry.
 *   c) map the referenced page table.
 *   d) dump the page table.
 *   e) unmap the page table.
 */

t_status		architecture_pd_dump(at_pd		pd)
{
  t_paddr		paddr;
  at_pdei		i;

  /*
   * 1)
   */

  if (as_physical(_kernel.as, (t_vaddr)pd, &paddr) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the page directory's physical address");

  /*
   * 2)
   */

  module_call(console, message,
	      '#', "[page directory] paddr(0x%08x) vaddr(0x%08x)\n",
	      paddr, (t_vaddr)pd);

  /*
   * 3)
   */

  for (i = 0; i < ARCHITECTURE_PD_SIZE; i++)
    {
      if (pd[i] & ARCHITECTURE_PDE_PRESENT)
	{
	  at_pt		pt;
	  char		flags[7];

	  /*
	   * a)
	   */

	  if (pd[i] & ARCHITECTURE_PDE_PRESENT)
	    flags[0] = 'p';
	  else
	    flags[0] = '.';

	  if (pd[i] & ARCHITECTURE_PDE_RW)
	    flags[1] = 'w';
	  else
	    flags[1] = 'r';

	  if (pd[i] & ARCHITECTURE_PDE_USER)
	    flags[2] = 'u';
	  else
	    flags[2] = 's';

	  if (pd[i] & ARCHITECTURE_PDE_PWT)
	    flags[3] = 't';
	  else
	    flags[3] = 'b';

	  if (pd[i] & ARCHITECTURE_PDE_PCD)
	    flags[4] = '.';
	  else
	    flags[4] = 'c';

	  if (pd[i] & ARCHITECTURE_PDE_ACCESSED)
	    flags[5] = 'a';
	  else
	    flags[5] = '.';

	  flags[6] = '\0';

	  /*
	   * b)
	   */

	  module_call(console, message,
		      '#',
		      "  [page directory entry %4u] address(0x%08x) flags(%s) "
		      "range(0x%08x - 0x%08x)\n",
		      i,
		      ARCHITECTURE_PDE_ADDRESS(pd[i]),
		      flags,
		      i * ARCHITECTURE_PD_SIZE * ___kaneton$pagesz,
		      (i + 1) * ARCHITECTURE_PD_SIZE * ___kaneton$pagesz - 1);

	  /*
	   * c)
	   */

	  if (architecture_pt_map(ARCHITECTURE_PDE_ADDRESS(pd[i]),
				  &pt) != STATUS_OK)
	    MACHINE_ESCAPE("unable to map the page table");

	  /*
	   * d)
	   */

	  if (architecture_pt_dump(pt,
				   2 * MODULE_CONSOLE_MARGIN_SHIFT) !=
	      STATUS_OK)
	    MACHINE_ESCAPE("unable to dump the %uth page directory entry");

	  /*
	   * e)
	   */

	  if (architecture_pt_unmap(pt) != STATUS_OK)
	    MACHINE_ESCAPE("unable to unmap the page table");
	}
    }

  MACHINE_LEAVE();
}

/*
 * this function builds a mapped page directory.
 *
 * steps:
 *
 * 1) initialize the page directory's entries making them all non-present.
 */

t_status		architecture_pd_build(at_pd		pd)
{
  /*
   * 1)
   */

  memset(pd, 0x0, ___kaneton$pagesz);

  MACHINE_LEAVE();
}

/*
 * this function inserts a page table reference in the given page directory.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) construct the page directory entry.
 */

t_status		architecture_pd_insert(at_pd		pd,
					       at_pdei		index,
					       t_paddr		address,
					       t_flags		flags)
{
  /*
   * 0)
   */

  if (index >= ARCHITECTURE_PT_SIZE)
    MACHINE_ESCAPE("out-of-bound page table entry index");

  if (ARCHITECTURE_PAGING_BASE(address) != address)
    MACHINE_ESCAPE("the given address is not aligned");

  /*
   * 1)
   */

  pd[index] =
    ARCHITECTURE_PAGING_BASE(address) |
    flags |
    ARCHITECTURE_PDE_PRESENT;

  MACHINE_LEAVE();
}

/*
 * this function deletes a page directory entry by resetting its memory.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) reset the page directory entry.
 */

t_status		architecture_pd_delete(at_pd		pd,
					       at_pdei		index)
{
  /*
   * 0)
   */

  if (index >= ARCHITECTURE_PT_SIZE)
    MACHINE_ESCAPE("out-of-bound page table entry index");

  if (!(pd[index] & ARCHITECTURE_PDE_PRESENT))
    MACHINE_ESCAPE("this page directory entry does not seem to be used");

  /*
   * 1)
   */

  pd[index] = 0x0;

  MACHINE_LEAVE();
}

/*
 * this function maps a page directory in the kernel's address space, taking
 * care to register the page used to map it in the region manager.
 *
 * note that the mirroring technique is used to prevent this function
 * from looping infinitely i.e mapping a page table requires it to be mapped
 * which may require the page table referencing the page to be mapped to
 * be mapped as well and so on and so forth.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) reserve a page in the virtual space for the page directory to be mapped.
 * 2) compute the address of the kernel's page table responsible for
 *    referencing the reserved page. note that the mirroring technique
 *    is used for locating the page table. therefore, if this page table
 *    is present, the kernel can access it without mapping it.
 * 3) if this page table required for referencing the page---through
 *    which the page directory is about to be mapped---is not present,
 *    create it.
 *   a) reserve a segment for the page table to be created.
 *   b) retrieve the reserved segment object.
 *   c) update the page directory, hence referencing the new page table.
 *   d) build the page table. note that the mirrored address is used from
 *      now on. this is why the page table does not have to be mapped.
 * 4) update the page table, referencing the page through which the given
 *    page directory is being mapped.
 * 5) locate the segment in which the given page directory lies.
 * 6) allocate and fill a region object.
 * 7) inject the region in the kernel's address space in order to secure
 *    it.
 * 8) return the address of the mapped page directory.
 */

t_status		architecture_pd_map(t_paddr		paddr,
					    at_pd*		table)
{
  i_segment		segment;
  i_region		region;
  t_vaddr		vaddr;
  at_pt			pt;
  o_region*		o;

  /*
   * 0)
   */

  if (paddr % ___kaneton$pagesz)
    MACHINE_ESCAPE("the physical address is not aligned");

  if (table == NULL)
    MACHINE_ESCAPE("the 'table' argument is null");

  /*
   * 1)
   */

  if (region_space(_kernel.as,
		   ___kaneton$pagesz,
		   &vaddr) != STATUS_OK)
    MACHINE_ESCAPE("unable to find space within the kernel's address space");

  /*
   * 2)
   */

  pt = (at_pt)ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
					  ARCHITECTURE_PD_INDEX(vaddr));

  /*
   * 3)
   */

  if (!(_as.machine.pd[ARCHITECTURE_PD_INDEX(vaddr)] &
	ARCHITECTURE_PDE_PRESENT))
    {
      o_segment*	o;

      /*
       * a)
       */

      if (segment_reserve(_kernel.as,
			  ___kaneton$pagesz,
			  PERMISSION_READ | PERMISSION_WRITE,
			  SEGMENT_OPTION_SYSTEM,
			  &segment) != STATUS_OK)
	MACHINE_ESCAPE("unable to reserve a segment");

      /*
       * b)
       */

      if (segment_get(segment, &o) != STATUS_OK)
	MACHINE_ESCAPE("unable to retrieve the segment object");

      /*
       * c)
       */

      if (architecture_pd_insert(_as.machine.pd,
				 ARCHITECTURE_PD_INDEX(vaddr),
				 o->address,
				 ARCHITECTURE_PDE_PRESENT |
				 ARCHITECTURE_PDE_RW |
				 ARCHITECTURE_PDE_SUPERVISOR |
				 ARCHITECTURE_PDE_PWB |
				 ARCHITECTURE_PDE_PCE) != STATUS_OK)
	MACHINE_ESCAPE("unable to insert an entry in the page directory");

      /*
       * d)
       */

      if (architecture_pt_build(pt) != STATUS_OK)
	MACHINE_ESCAPE("unable to build the page table");
    }

  /*
   * 4)
   */

  if (architecture_pt_insert(pt,
			     ARCHITECTURE_PT_INDEX(vaddr),
			     paddr,
			     ARCHITECTURE_PTE_PRESENT |
			     ARCHITECTURE_PTE_RW |
			     ARCHITECTURE_PTE_SUPERVISOR |
			     ARCHITECTURE_PTE_PWB |
			     ARCHITECTURE_PTE_PCE) != STATUS_OK)
    MACHINE_ESCAPE("unable to add the page to the page table");

  /*
   * 5)
   */

  if (segment_locate(paddr, &segment) == FALSE)
    MACHINE_ESCAPE("unable to locate the segment to map");

  /*
   * 6)
   */

  if ((o = malloc(sizeof (o_region))) == NULL)
    MACHINE_ESCAPE("unable to allocate memory for the region object");

  o->segment = segment;
  o->address = vaddr;
  o->offset = 0x0;
  o->size = ___kaneton$pagesz;
  o->options = REGION_OPTION_NONE;

  /*
   * 7)
   */

  if (region_inject(_kernel.as, o, &region) != STATUS_OK)
    MACHINE_ESCAPE("unable to inject the region associated with the mapped "
		   "page");

  /*
   * 8)
   */

  *table = (at_pd)vaddr;

  MACHINE_LEAVE();
}

/*
 * this function unmaps the given page directory.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) if there is no kernel page directory entry referecing the given
 *    address, return an error as the given page directory does not seem
 *    to be mapped.
 * 2) compute the mirroring address of the page table referencing the
 *    given address.
 * 3) delete the page table entry referencing the given address.
 * 4) retrieve the kernel address space object.
 * 5) locate the region corresponding to the given page directory to unmap.
 * 6) remove this region from the kernel address space, making it
 *    available.
 * 7) invalidate the virtual address as no longer used.
 */

t_status		architecture_pd_unmap(at_pd		table)
{
  i_region		region;
  t_vaddr		vaddr;
  at_pt			pt;
  o_as*			o;

  /*
   * 0)
   */

  vaddr = (t_vaddr)table;

  if (vaddr % ___kaneton$pagesz)
    MACHINE_ESCAPE("the page directory address is not aligned");

  /*
   * 1)
   */

  if (!(_as.machine.pd[ARCHITECTURE_PD_INDEX(vaddr)] &
	ARCHITECTURE_PDE_PRESENT))
    MACHINE_ESCAPE("the page table referencing the page directory does "
		   "not seem to be used");

  /*
   * 2)
   */

  pt = (at_pt)ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
					  ARCHITECTURE_PD_INDEX(vaddr));

  /*
   * 3)
   */

  if (architecture_pt_delete(pt,
			     ARCHITECTURE_PT_INDEX(vaddr)) != STATUS_OK)
    MACHINE_ESCAPE("unable to delete the page table entry referencing "
		   "the mapped page directory");

  /*
   * 4)
   */

  if (as_get(_kernel.as, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the kernel address space object");

  /*
   * 5)
   */

  if (region_locate(o->id, vaddr, &region) == FALSE)
    MACHINE_ESCAPE("unable to locate the region associated with the "
		   "page directory");

  /*
   * 6)
   */

  if (set_remove(o->regions, region) != STATUS_OK)
    MACHINE_ESCAPE("unable to remove the region from the address space");

  /*
   * 7)
   */

  if (architecture_tlb_invalidate(vaddr) != STATUS_OK)
    MACHINE_ESCAPE("unable to invalidate the page directory address");

  MACHINE_LEAVE();
}
