/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../architecture/ia32/educational/mapping.c
 *
 * created       matthieu bucchianeri   [mon dec 24 19:26:06 2007]
 * updated       julien quintard   [wed dec  1 23:50:19 2010]
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

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function directly maps a chunk of memory.
 *
 * steps:
 *
 * 1) check  if  the needed  page  table  is  present in  the  kernel,
 *    otherwise,  create  it.
 * 2) via the mirroring entry,  add the page-table entry corresponding
 *    to the virtual address.
 * 3) inject the manually mapped region into the kernel address space.
 * 4) invalidate tlb entry.
 */

t_error			ia32_map_chunk(t_vaddr		v,
				       t_paddr		p,
				       void*		alloc)
{
  i_region		useless;
  t_ia32_table		pt;
  t_ia32_page		pg;
  i_segment		seg;
  o_region*		reg = alloc;

  assert(!(v % PAGESZ));
  assert(!(p % PAGESZ));

  if (reg == NULL)
    return (ERROR_KO);

  /*
   * 1)
   */

  if (ia32_pd_get_table(IA32_PAGE_DIRECTORY_CURRENT,
			IA32_PAGE_DIRECTORY_ENTRY_INDEX(v), &pt) != ERROR_OK)
    {
      if (segment_reserve(_kernel->as,
			  PAGESZ,
			  PERMISSION_READ | PERMISSION_WRITE,
			  &seg) != ERROR_OK)
	return (ERROR_KO);

      if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
	return (ERROR_KO);

      if (ia32_pt_build((t_paddr)seg, &pt) != ERROR_OK)
	return (ERROR_KO);

      pt.rw = IA32_PAGE_TABLE_WRITABLE;
      pt.present = 1;
      pt.user = IA32_PAGE_TABLE_PRIVILEGED;
      pt.cached = IA32_PAGE_TABLE_CACHED; // XXX CACHED
      pt.writeback = IA32_PAGE_TABLE_WRITEBACK;

      if (ia32_pd_add_table(IA32_PAGE_DIRECTORY_CURRENT,
			    IA32_PAGE_DIRECTORY_ENTRY_INDEX(v),
			    pt) != ERROR_OK)
	return (ERROR_KO);

      ia32_tlb_invalidate(
	IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
			   IA32_PAGE_DIRECTORY_ENTRY_INDEX(v)));

      memset((void*)IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
				       IA32_PAGE_DIRECTORY_ENTRY_INDEX(v)), 0,
	     PAGESZ);
    }

  /*
   * 2)
   */

  pt.vaddr = IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
				IA32_PAGE_DIRECTORY_ENTRY_INDEX(v));

  pg.rw = IA32_PAGE_WRITABLE;
  pg.present = 1;
  pg.user = IA32_PAGE_PRIVILEGED;
  pg.cached = IA32_PAGE_CACHED; // CACHED
  pg.writeback = IA32_PAGE_WRITEBACK;
  pg.addr = p;

  if (ia32_pt_add_page(&pt, IA32_PAGE_TABLE_ENTRY_INDEX(v), pg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  reg->segment = p;
  reg->address = v;
  reg->offset = 0;
  reg->size = PAGESZ;
  reg->options = REGION_OPTION_NONE;

  if (region_inject(_kernel->as, reg, &useless) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 4)
   */

  ia32_tlb_invalidate(v);

  return (ERROR_OK);
}

/*
 * this function unmaps a page previously mapped with the map_chunk
 * function.
 *
 * steps:
 *
 * 1) get the page table corresponding to the virtual address.
 * 2) remove, via the mirroring entry, the page-table entry.
 * 3) manually remove the kernel region associated.
 * 4) invalidate translation caches.
 */

t_error			ia32_unmap_chunk(t_vaddr	v)
{

  t_ia32_table		pt;
  o_as*			as;

  assert(!(v % PAGESZ));

  /*
   * 1)
   */

  if (ia32_pd_get_table(IA32_PAGE_DIRECTORY_CURRENT,
			IA32_PAGE_DIRECTORY_ENTRY_INDEX(v), &pt) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  pt.vaddr = IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
				IA32_PAGE_DIRECTORY_ENTRY_INDEX(v));

  if (ia32_pt_delete_page(&pt, IA32_PAGE_TABLE_ENTRY_INDEX(v)) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  if (as_get(_kernel->as, &as) != ERROR_OK)
    return (ERROR_KO);

  if (set_remove(as->regions, v) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 4)
   */

  ia32_tlb_invalidate(v);

  return (ERROR_OK);
}

/*
 * this function is an helper for mapping a page directory.
 */

t_error			ia32_map_pd(t_ia32_directory*	pd)
{
  /*							     [block::map_pd] */

  t_vaddr		chunk;
  void*			tmp;

  tmp = malloc(sizeof(o_region));

  if (region_space(_kernel->as, PAGESZ, &chunk) != ERROR_OK)
    return (ERROR_KO);

  if (ia32_map_chunk(chunk, (t_paddr)*pd, tmp) != ERROR_OK)
    return (ERROR_KO);

  *pd = (t_ia32_directory)(t_uint32)chunk;

  /*							  [endblock::map_pd] */

  return (ERROR_OK);
}

/*
 * this function is an helper for mapping a page table.
 */

t_error			ia32_map_pt(t_ia32_table*	pt)
{
  /*							     [block::map_pt] */

  t_vaddr		chunk;
  void*			tmp;

  tmp = malloc(sizeof(o_region));

  if (region_space(_kernel->as, PAGESZ, &chunk) != ERROR_OK)
    return (ERROR_KO);

  if (ia32_map_chunk(chunk, (t_paddr)pt->paddr, tmp) != ERROR_OK)
    return (ERROR_KO);

  pt->vaddr = chunk;

  /*							  [endblock::map_pt] */

  return (ERROR_OK);
}

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
  t_ia32_directory	pd;
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

  assert(!(size % PAGESZ));
  assert(!(offset % PAGESZ));
  assert(!(address % PAGESZ));

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  if (segment_get(segid, &oseg) != ERROR_OK)
    return (ERROR_KO);

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

  pd = o->machine.pd;

  if (asid != _kernel->as)
    {
      if (ia32_map_pd(&pd) != ERROR_OK)
	return (ERROR_KO);
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

      if (ia32_pd_get_table(&pd, pde, &pt) != ERROR_OK)
	{
	  if (segment_reserve(asid,
			      PAGESZ,
			      PERMISSION_READ | PERMISSION_WRITE,
			      &ptseg) != ERROR_OK)
	    return (ERROR_KO);

	  if (segment_type(ptseg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
	    return (ERROR_KO);

	  if (ia32_pt_build((t_paddr)ptseg, &pt) != ERROR_OK)
	    return (ERROR_KO);

	  pt.rw = IA32_PAGE_TABLE_WRITABLE;
	  pt.present = 1;
	  pt.user = IA32_PAGE_TABLE_USER;
	  pt.cached = IA32_PAGE_TABLE_CACHED; // XXX CACHED
	  pt.writeback = IA32_PAGE_TABLE_WRITEBACK;

	  if (ia32_pd_add_table(&pd, pde, pt) != ERROR_OK)
	    return (ERROR_KO);

	  clear_pt = 1;
	}
      else
	clear_pt = 0;

      /*
       * b)
       */

      if (ia32_map_pt(&pt) != ERROR_OK)
	return (ERROR_KO);

      if (clear_pt)
	memset((void*)pt.vaddr, 0, PAGESZ);

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PAGE_TABLE_MAX_ENTRIES);
	   pte++)
	{
	  if (((t_ia32_pte*)pt.vaddr)[pte] & IA32_PAGE_TABLE_ENTRY_FLAG_USED)
	    printf("warning: adding already-in-use page\n");

	  /*
	   * d)
	   */

	  pg.addr = paddr;
	  paddr += PAGESZ;
	  size -= PAGESZ;

	  if (ia32_pt_add_page(&pt, pte, pg) != ERROR_OK)
	    return (ERROR_KO);

	  /*
	   * e)
	   */

	  if (asid == _kernel->as)
	    ia32_tlb_invalidate((t_vaddr)IA32_ENTRY_ADDRESS(pde, pte));
	}

      /*
       * f)
       */

      if (ia32_unmap_chunk((t_vaddr)pt.vaddr) != ERROR_OK)
	return (ERROR_KO);
    }

  /*
   * 6)
   */

  if (asid != _kernel->as)
    if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_OK)
      return (ERROR_KO);

  /*						      [endblock::map_region] */

  return (ERROR_OK);
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
  t_ia32_directory	pd;
  t_ia32_table		pt;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;

  assert(!(address % PAGESZ));
  assert(!(size % PAGESZ));

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  pd = o->machine.pd;

  if (asid != _kernel->as)
    {
      if (ia32_map_pd(&pd) != ERROR_OK)
	return (ERROR_KO);
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

      if (ia32_pd_get_table(&pd, pde, &pt) != ERROR_OK)
	return (ERROR_KO);

      /*
       * b)
       */

      if (ia32_map_pt(&pt) != ERROR_OK)
	return (ERROR_KO);

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PAGE_TABLE_MAX_ENTRIES);
	   pte++)
	{
	  if (ia32_pt_delete_page(&pt, pte) != ERROR_OK)
	    return (ERROR_KO);

	  if (asid == _kernel->as)
	    ia32_tlb_invalidate(IA32_ENTRY_ADDRESS(pde, pte));
	}

      /*
       * d)
       */

      if (ia32_unmap_chunk((t_vaddr)pt.vaddr) != ERROR_OK)
	return (ERROR_KO);

      /*
       * e)
       */

      if (pde != pde_start && pde != pde_end)
	{
	  if (ia32_pd_delete_table(&pd, pde) != ERROR_OK)
	    return (ERROR_KO);

	  if (asid == _kernel->as)
	    ia32_tlb_invalidate(IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
						   pde));

	  if (segment_release((i_segment)pt.paddr) != ERROR_OK)
	    return (ERROR_KO);
	}
    }

  /*
   * 5)
   */

  if (asid != _kernel->as)
    if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_OK)
      return (ERROR_KO);

  /*						    [endblock::unmap_region] */

  return (ERROR_OK);
}
