/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../architecture/ia32/educational/mapping.c
 *
 * created       matthieu bucchianeri   [mon dec 24 19:26:06 2007]
 * updated       julien quintard   [thu jan 13 10:34:37 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * contiguous mapping on ia32.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * kernel manager.
 */

extern m_kernel*	_kernel;

// XXX
extern at_pd		_architecture_pd;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * map a region.
 *						   [block::map_region::comment]
 *
 * steps:
 *
 * 1) get the as object.
 * 2) get the segment object (for permissions and physical base address).
 * 3) fill the t_page structure.
 * 4) map the as page-directory into the kernel.
 * 5) loop throught the virtual memory to map.
 *  a) get the needed page-table. if not present, the create it.
 *  b) temporarily map the page-table in the kernel as.
 *  c) loop throught the page-table.
 *  d) fill corresponding entries.
 *  e) invalidate translation lookaside buffers.
 *  f) unmap the temporarily mapped table.
 * 6) unmap the page-directory.
 *						[endblock::map_region::comment]
 */

t_error			ia32_map_region(i_as		asid,
					i_segment	segid,
					t_paddr		offset,
					t_options	options,
					t_vaddr		address,
					t_vsize		size)
{
  /*							 [block::map_region] */

  o_as*			o;
  o_segment*		oseg;
  t_vaddr		vaddr;
  t_paddr		paddr;
  at_pd			pd;
  t_ia32_table		pt;
  t_ia32_page		pg;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;
  i_segment		ptseg;
  t_uint32		clear_pt;

  assert(!(size % ___kaneton$pagesz));
  assert(!(offset % ___kaneton$pagesz));
  assert(!(address % ___kaneton$pagesz));

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (segment_get(segid, &oseg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 3)
   */

  pg.rw = (oseg->permissions & PERMISSION_WRITE) ?
    IA32_PAGE_WRITABLE : IA32_PAGE_READONLY;
  pg.present = 1;
  pg.user = (options & REGION_OPTION_PRIVILEGED) ?
    IA32_PAGE_PRIVILEGED : IA32_PAGE_USER;
  pg.global = (options & REGION_OPTION_GLOBAL) ?
    IA32_PAGE_GLOBAL : IA32_PAGE_NONGLOBAL;
  pg.cached = IA32_PAGE_CACHED; // XXX CACHED
  pg.writeback = IA32_PAGE_WRITEBACK;

  /*
   * 4)
   */

  if (asid != _kernel->as)
    {
      /* XXX[old]
      if (ia32_map_pd(&pd) != ERROR_OK)
	MACHINE_ESCAPE("XXX");
      */
      if (architecture_pd_map(o->machine.pd, &pd) != ERROR_OK) // XXX
	MACHINE_ESCAPE("unable to map the page directory");
    }
  else
    {
      // XXX identity mapping
      pd = (at_pd)o->machine.pd;
    }

  /*
   * 5)
   */

  paddr = oseg->address + offset;
  vaddr = address;

  pde_start = IA32_PAGE_DIRECTORY_ENTRY_INDEX(vaddr);
  pte_start = IA32_PAGE_TABLE_ENTRY_INDEX(vaddr);
  pde_end = IA32_PAGE_DIRECTORY_ENTRY_INDEX(vaddr + size);
  pte_end = IA32_PAGE_TABLE_ENTRY_INDEX(vaddr + size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      if (!size)
	break;

      /*
       * a)
       */

      /* XXX[old]
      if (ia32_pd_get_table(&pd, pde, &pt) != ERROR_OK)
      */
      if (!(pd[pde] & ARCHITECTURE_PDE_USED))
	{
	  o_segment*	s;

	  if (segment_reserve(asid,
			      ___kaneton$pagesz,
			      PERMISSION_READ | PERMISSION_WRITE,
			      &ptseg) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");

	  if (segment_type(ptseg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");

	  if (segment_get(ptseg, &s) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");

	  pt.paddr = s->address; // XXX pour le map_pt() a venir!

	  if (architecture_pd_insert(pd,
				     pde,
				     s->address,
				     ARCHITECTURE_PDE_PRESENT |
				     ARCHITECTURE_PDE_RW |
				     ARCHITECTURE_PDE_USER |
				     ARCHITECTURE_PDE_PWB |
				     ARCHITECTURE_PDE_PCE) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");

	  clear_pt = 1;
	}
      else
	{
	  clear_pt = 0;

	  pt.paddr = IA32_BASE(pd[pde]);
	}

      /*
       * b)
       */

      if (architecture_pt_map(pt.paddr, (at_pt*)&pt.vaddr) != ERROR_OK) // XXX
	MACHINE_ESCAPE("XXX");

      if (clear_pt)
	memset((void*)pt.vaddr, 0, ___kaneton$pagesz);

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PAGE_TABLE_MAX_ENTRIES);
	   pte++)
	{
	  if (((t_ia32_pte*)pt.vaddr)[pte] & IA32_PAGE_TABLE_ENTRY_FLAG_USED)
	    module_call(console, print,
			"warning: adding already-in-use page\n");

	  /*
	   * d)
	   */

	  pg.addr = paddr;

	  if (architecture_pt_insert((at_pt)pt.vaddr,
				     pte,
				     paddr,
				     ARCHITECTURE_PTE_PRESENT |
				     (oseg->permissions & PERMISSION_WRITE ?
				       ARCHITECTURE_PTE_RW :
				       ARCHITECTURE_PTE_RO) |
				     (options & REGION_OPTION_PRIVILEGED ?
				       ARCHITECTURE_PTE_SUPERVISOR :
				       ARCHITECTURE_PTE_USER) |
				     ARCHITECTURE_PTE_PWB |
				     ARCHITECTURE_PTE_PCE |
				     (options & REGION_OPTION_GLOBAL ?
				       ARCHITECTURE_PTE_GLOBAL :
				       ARCHITECTURE_PTE_NONE)) != ERROR_OK)
	    MACHINE_ESCAPE("unable to add the page to the page table");

	  paddr += ___kaneton$pagesz;
	  size -= ___kaneton$pagesz;

	  /*
	   * e)
	   */

	  if (asid == _kernel->as)
	    architecture_tlb_invalidate(IA32_ENTRY_ADDRESS(pde, pte));
	}

      /*
       * f)
       */

      if (architecture_pt_unmap((at_pt)pt.vaddr) != ERROR_OK) // XXX
	MACHINE_ESCAPE("XXX");
    }

  /*
   * 6)
   */

  if (asid != _kernel->as)
    if (architecture_pd_unmap((at_pd)pd) != ERROR_OK) // XXX
      MACHINE_ESCAPE("XXX");

  /*						      [endblock::map_region] */

  MACHINE_LEAVE();
}

/*
 * this function  releases a region.  we unmap the region  and release
 * page tables.
 *						 [block::unmap_region::comment]
 *
 * steps:
 *
 * 1) get the kernel as object and the as proper object.
 * 2) map the as page-directory temporarily.
 * 3) loop throught the page-directory and page-table entries.
 *  a) get the page-table.
 *  b) map it temporarily.
 *  c) delete the entries and flush the tlb entries.
 *  d) unmap the page-table.
 *  e) if the resulting page-table is empty, release it.
 * 4) unmap the as page-directory.
 *					      [endblock::unmap_region::comment]
 */

t_error			ia32_unmap_region(i_as		asid,
					  t_vaddr	address,
					  t_vsize	size)
{
  /*						       [block::unmap_region] */

  o_as*			o;
  at_pd			pd;
  t_ia32_table		pt;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;

  assert(!(address % ___kaneton$pagesz));
  assert(!(size % ___kaneton$pagesz));

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 3)
   */

  if (asid != _kernel->as)
    {
      /* XXX[old]
      if (ia32_map_pd(&pd) != ERROR_OK)
	MACHINE_ESCAPE("XXX");
      */
      if (architecture_pd_map(o->machine.pd, &pd) != ERROR_OK) // XXX
	MACHINE_ESCAPE("unable to map the page directory");
    }
  else
    {
      // XXX identity mapping.
      pd = (at_pd)o->machine.pd;
    }

  /*
   * 4)
   */

  pde_start = IA32_PAGE_DIRECTORY_ENTRY_INDEX(address);
  pte_start = IA32_PAGE_TABLE_ENTRY_INDEX(address);
  pde_end = IA32_PAGE_DIRECTORY_ENTRY_INDEX(address + size);
  pte_end = IA32_PAGE_TABLE_ENTRY_INDEX(address + size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      /*
       * a)
       */

      /* XXX[old]
      if (ia32_pd_get_table(&pd, pde, &pt) != ERROR_OK)
	MACHINE_ESCAPE("XXX");
      */
      if (!(pd[pde] & ARCHITECTURE_PDE_USED))
	MACHINE_ESCAPE("XXX");

      /*
       * b)
       */

      if (architecture_pt_map(IA32_BASE(pd[pde]),
			      (at_pt*)&pt.vaddr) != ERROR_OK) // XXX
	MACHINE_ESCAPE("XXX");

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PAGE_TABLE_MAX_ENTRIES);
	   pte++)
	{
	  if (architecture_pt_delete((at_pt)pt.vaddr, pte) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");

	  if (asid == _kernel->as)
	    architecture_tlb_invalidate(IA32_ENTRY_ADDRESS(pde, pte));
	}

      /*
       * d)
       */

      if (architecture_pt_unmap((at_pt)pt.vaddr) != ERROR_OK) // XXX
	MACHINE_ESCAPE("XXX");

      /*
       * e)
       */

      if (pde != pde_start && pde != pde_end)
	{
	  i_segment	s;

	  if (architecture_pd_delete((at_pd)pd, pde) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");

	  if (asid == _kernel->as)
	    architecture_tlb_invalidate(
              IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
				 pde));

	  if (segment_locate(pt.paddr, &s) == ERROR_FALSE)
	    MACHINE_ESCAPE("XXX");

	  if (segment_release(s) != ERROR_OK)
	    MACHINE_ESCAPE("XXX");
	}
    }

  /*
   * 5)
   */

  if (asid != _kernel->as)
    if (architecture_pd_unmap(pd) != ERROR_OK) // XXX
      MACHINE_ESCAPE("XXX");

  /*						    [endblock::unmap_region] */

  MACHINE_LEAVE();
}
