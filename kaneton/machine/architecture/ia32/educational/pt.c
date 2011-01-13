/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/architecture/ia32/educational/pt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:48 2005]
 * updated       julien quintard   [thu jan 13 12:16:14 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for manipulating PTs - Page Tables.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

// XXX
extern m_kernel*	_kernel;

// XXX
extern at_pd		_architecture_pd;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps a page table.
 *
 * steps:
 *
 * 1) XXX
 */

t_error			architecture_pt_dump(at_pt		pt,
					     mt_margin		margin)
{
  t_paddr		paddr;
  at_ptei		i;

  /*
   * 1)
   */

  if (as_physical(_kernel->as, (t_vaddr)pt, &paddr) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the page tabel's physical address");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "[page table] paddr(0x%08x) vaddr(0x%08x)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      paddr, (t_vaddr)pt);

  /*
   * 3)
   */

  for (i = 0; i < ARCHITECTURE_PT_SIZE; i++)
    {
      if (pt[i] & ARCHITECTURE_PTE_USED)
	{
	  char		flags[8];

	  /*
	   * a)
	   */

	  if (pt[i] & ARCHITECTURE_PTE_PRESENT)
	    flags[0] = 'p';
	  else
	    flags[0] = '.';

	  if (pt[i] & ARCHITECTURE_PTE_RW)
	    flags[1] = 'w';
	  else
	    flags[1] = 'r';

	  if (pt[i] & ARCHITECTURE_PTE_USER)
	    flags[2] = 'u';
	  else
	    flags[2] = 's';

	  if (pt[i] & ARCHITECTURE_PTE_PWT)
	    flags[3] = 't';
	  else
	    flags[3] = 'b';

	  if (pt[i] & ARCHITECTURE_PTE_PCD)
	    flags[4] = '.';
	  else
	    flags[4] = 'c';

	  if (pt[i] & ARCHITECTURE_PTE_ACCESSED)
	    flags[5] = 'a';
	  else
	    flags[5] = '.';

	  if (pt[i] & ARCHITECTURE_PTE_DIRTY)
	    flags[6] = 'd';
	  else
	    flags[6] = '.';

	  flags[7] = '\0';

	  /*
	   * b)
	   */

	  module_call(console, message,
		      '#',
		      MODULE_CONSOLE_MARGIN_FORMAT
		      "  [page table entry %4u] "
		      "address(0x%08x) flags(%s)\n",
		      MODULE_CONSOLE_MARGIN_VALUE(margin),
		      i,
		      ARCHITECTURE_PTE_ADDRESS(pt[i]),
		      flags);
	}
    }

  MACHINE_LEAVE();
}

/*
 * this function builds a page table in the page given by the
 * virtual address.
 *
 * steps:
 *
 * 1) initialize the page table's entries making them all non-present.
 */

t_error			architecture_pt_build(at_pt		pt)
{
  /*
   * 1)
   */

  memset(pt, 0x0, ___kaneton$pagesz);

  MACHINE_LEAVE();
}

/*
 * this function inserts a mapping relation in the given page table so
 * that the 'address' page gets maps through the page table entry 'index'.
 *
 * 0) verify the arguments.
 * 1) update the page table entry.
 */

t_error			architecture_pt_insert(at_pt		pt,
					       at_ptei		index,
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

  pt[index] =
    ARCHITECTURE_PAGING_BASE(address) |
    flags |
    ARCHITECTURE_PTE_USED;

  MACHINE_LEAVE();
}

/*
 * this function deletes the page table entry by resetting its memory.
 *
 * steps:
 *
 * XXX
 *
 * [XXX:improvement] should the page table gets empty, remove the
 *                   page directory entry referencing it!
 */

t_error			architecture_pt_delete(at_pt		pt,
					       at_ptei		index)
{
  /*
   * 0)
   */

  if (index >= ARCHITECTURE_PT_SIZE)
    MACHINE_ESCAPE("out-of-bound page table entry index");

  /*
   * 1)
   */

  if (!(pt[index] & ARCHITECTURE_PTE_USED))
    MACHINE_ESCAPE("this page table entry does not seem to be used");

  /*
   * 2)
   */

  pt[index] = 0x0;

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			architecture_pt_map(t_paddr		paddr,
					    at_pt*		table)
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

  /*
   * 1)
   */

  if (region_space(_kernel->as,
		   ___kaneton$pagesz,
		   &vaddr) != ERROR_OK)
    MACHINE_ESCAPE("unable to find space within the kernel's address space");

  /*
   * 2)
   */

  pt = (at_pt)ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
					  ARCHITECTURE_PD_INDEX(vaddr));

  /*
   * 2)
   */

  if (!(_architecture_pd[ARCHITECTURE_PD_INDEX(vaddr)] &
	ARCHITECTURE_PDE_USED))
    {
      o_segment*	o;

      /*
       * A)
       */

      /*
       * a)
       */

      if (segment_reserve(_kernel->as,
			  ___kaneton$pagesz,
			  PERMISSION_READ | PERMISSION_WRITE,
			  &segment) != ERROR_OK)
	MACHINE_ESCAPE("unable to reserve a segment");

      /*
       * b)
       */

      if (segment_type(segment, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
	MACHINE_ESCAPE("unable to change the segment's type to system");

      /*
       * c)
       */

      if (segment_get(segment, &o) != ERROR_OK)
	MACHINE_ESCAPE("unable to retrieve the segment object");

      /*
       * d)
       */

      if (architecture_pd_insert(_architecture_pd,
				 ARCHITECTURE_PD_INDEX(vaddr),
				 o->address,
				 ARCHITECTURE_PDE_PRESENT |
				 ARCHITECTURE_PDE_RW |
				 ARCHITECTURE_PDE_SUPERVISOR |
				 ARCHITECTURE_PDE_PWB |
				 ARCHITECTURE_PDE_PCE) != ERROR_OK)
	MACHINE_ESCAPE("unable to insert an entry in the page directory");

      /*
       * e)
       */

      if (architecture_tlb_invalidate((t_vaddr)pt) != ERROR_OK)
	MACHINE_ESCAPE("unable to invalidate the page table's address");
      // XXX message d'erreur/explication?

      /*
       * f)
       */

      memset(pt, 0x0, ___kaneton$pagesz);
    }

  /*
   * 3)
   */

  if (architecture_pt_insert(pt,
			     ARCHITECTURE_PT_INDEX(vaddr),
			     paddr,
			     ARCHITECTURE_PTE_PRESENT |
			     ARCHITECTURE_PTE_RW |
			     ARCHITECTURE_PTE_SUPERVISOR |
			     ARCHITECTURE_PTE_PWB |
			     ARCHITECTURE_PTE_PCE) != ERROR_OK)
    MACHINE_ESCAPE("unable to add the page to the page table");

  /*
   * 4)
   */

  if (segment_locate(paddr, &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the segment to map");

  /*
   * 5)
   */

  if ((o = malloc(sizeof(o_region))) == NULL)
    MACHINE_ESCAPE("unable to allocate memory for the region object");

  /*
   * 6)
   */

  o->segment = segment;
  o->address = vaddr;
  o->offset = 0x0;
  o->size = ___kaneton$pagesz;
  o->options = REGION_OPTION_NONE;

  /*
   * 7)
   */

  if (region_inject(_kernel->as, o, &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to inject the region associated with the mapped "
		   "page");

  /*
   * 8)
   */

  if (architecture_tlb_invalidate(vaddr) != ERROR_OK)
    MACHINE_ESCAPE("unable to invalidate the page address");

  /*
   * 9)
   */

  *table = (at_pt)vaddr;

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			architecture_pt_unmap(at_pt		table)
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

  if (!(_architecture_pd[ARCHITECTURE_PD_INDEX(vaddr)] &
	ARCHITECTURE_PDE_USED))
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
			     ARCHITECTURE_PT_INDEX(vaddr)) != ERROR_OK)
    MACHINE_ESCAPE("unable to delete the page table entry referencing "
		   "the mapped page directory");

  /*
   * 4)
   */

  if (as_get(_kernel->as, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the kernel address space object");

  /*
   * 5)
   */

  if (region_locate(o->id, vaddr, &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region associated with the "
		   "page directory");

  /*
   * 6)
   */

  if (set_remove(o->regions, region) != ERROR_OK)
    MACHINE_ESCAPE("unable to remove the region from the address space");

  /*
   * 7)
   */

  if (architecture_tlb_invalidate(vaddr) != ERROR_OK)
    MACHINE_ESCAPE("unable to invalidate the page directory address");

  MACHINE_LEAVE();
}
