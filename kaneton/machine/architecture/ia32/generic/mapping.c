/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...hine/architecture/ia32/generic/mapping.c
 *
 * created       matthieu bucchianeri   [mon dec 24 19:26:06 2007]
 * updated       matthieu bucchianeri   [wed jan  9 11:24:48 2008]
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
 * kernel address space identifier.
 */

extern i_as		kasid;

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

  ASSERT(!(v % PAGESZ));
  ASSERT(!(p % PAGESZ));

  if (reg == NULL)
    return (ERROR_UNKNOWN);

  /*
   * 1)
   */

  if (ia32_pd_get_table(IA32_PD_CURRENT, IA32_PDE_ENTRY(v), &pt) != ERROR_NONE)
    {
      if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			  &seg) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (ia32_pt_build((t_paddr)seg, &pt) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      pt.rw = IA32_PT_WRITABLE;
      pt.present = 1;
      pt.user = IA32_PT_PRIVILEGED;
      pt.cached = IA32_PT_CACHED;
      pt.writeback = IA32_PT_WRITEBACK;

      if (ia32_pd_add_table(IA32_PD_CURRENT, IA32_PDE_ENTRY(v),
			    pt) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      ia32_tlb_invalidate(IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v)));

      memset((void*)IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v)), 0,
	     PAGESZ);
    }

  /*
   * 2)
   */

  pt.vaddr = IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v));

  pg.rw = IA32_PG_WRITABLE;
  pg.present = 1;
  pg.user = IA32_PG_PRIVILEGED;
  pg.cached = IA32_PG_CACHED;
  pg.writeback = IA32_PG_WRITEBACK;
  pg.addr = p;

  if (ia32_pt_add_page(&pt, IA32_PTE_ENTRY(v), pg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  reg->segid = p;
  reg->address = v;
  reg->offset = 0;
  reg->size = PAGESZ;
  reg->opts = REGION_OPT_NONE;

  if (region_inject(kasid, reg, &useless) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  ia32_tlb_invalidate(v);

  return (ERROR_NONE);
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

  ASSERT(!(v % PAGESZ));

  /*
   * 1)
   */

  if (ia32_pd_get_table(IA32_PD_CURRENT, IA32_PDE_ENTRY(v), &pt) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  pt.vaddr = IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v));

  if (ia32_pt_delete_page(&pt, IA32_PTE_ENTRY(v)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(kasid, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (set_remove(as->regions, v) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  ia32_tlb_invalidate(v);

  return (ERROR_NONE);
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

  if (region_space(kasid, PAGESZ, &chunk) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (ia32_map_chunk(chunk, (t_paddr)*pd, tmp) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  *pd = (t_ia32_directory)(t_uint32)chunk;

  /*							  [endblock::map_pd] */

  return (ERROR_NONE);
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

  if (region_space(kasid, PAGESZ, &chunk) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (ia32_map_chunk(chunk, (t_paddr)pt->paddr, tmp) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  pt->vaddr = chunk;

  /*							  [endblock::map_pt] */

  return (ERROR_NONE);
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
					t_opts		opts,
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

  ASSERT(!(size % PAGESZ));
  ASSERT(!(offset % PAGESZ));
  ASSERT(!(address % PAGESZ));

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_get(segid, &oseg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  pg.rw = (oseg->perms & PERM_WRITE) ? IA32_PG_WRITABLE : IA32_PG_READONLY;
  pg.present = 1;
  pg.user = (opts & REGION_OPT_PRIVILEGED) ? IA32_PG_PRIVILEGED : IA32_PG_USER;
  pg.global = (opts & REGION_OPT_GLOBAL) ? IA32_PG_GLOBAL : IA32_PG_NONGLOBAL;
  pg.cached = IA32_PG_CACHED;
  pg.writeback = IA32_PG_WRITEBACK;

  /*
   * 4)
   */

  pd = o->machdep.pd;

  if (asid != kasid)
    {
      if (ia32_map_pd(&pd) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  paddr = oseg->address + offset;
  vaddr = address;

  pde_start = IA32_PDE_ENTRY(vaddr);
  pte_start = IA32_PTE_ENTRY(vaddr);
  pde_end = IA32_PDE_ENTRY(vaddr + size);
  pte_end = IA32_PTE_ENTRY(vaddr + size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      if (!size)
	break;

      /*
       * a)
       */

      if (ia32_pd_get_table(&pd, pde, &pt) != ERROR_NONE)
	{
	  if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE,
			      &ptseg) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (segment_type(ptseg, SEGMENT_TYPE_SYSTEM) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (ia32_pt_build((t_paddr)ptseg, &pt) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  pt.rw = IA32_PT_WRITABLE;
	  pt.present = 1;
	  pt.user = IA32_PT_USER;
	  pt.cached = IA32_PT_CACHED;
	  pt.writeback = IA32_PT_WRITEBACK;

	  if (ia32_pd_add_table(&pd, pde, pt) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  clear_pt = 1;
	}
      else
	clear_pt = 0;

      /*
       * b)
       */

      if (ia32_map_pt(&pt) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (clear_pt)
	memset((void*)pt.vaddr, 0, PAGESZ);

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PT_MAX_ENTRIES);
	   pte++)
	{
	  /*
	   * d)
	   */

	  pg.addr = paddr;
	  paddr += PAGESZ;
	  size -= PAGESZ;

	  if (ia32_pt_add_page(&pt, pte, pg) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  /*
	   * e)
	   */

	  if (asid == kasid)
	    ia32_tlb_invalidate((t_vaddr)IA32_ENTRY_ADDR(pde, pte));
	}

      /*
       * f)
       */

      if (ia32_unmap_chunk((t_vaddr)pt.vaddr) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  if (asid != kasid)
    if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
      return (ERROR_UNKNOWN);

  /*						      [endblock::map_region] */

  return (ERROR_NONE);
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

  ASSERT(!(address % PAGESZ));
  ASSERT(!(size % PAGESZ));

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  pd = o->machdep.pd;

  if (asid != kasid)
    {
      if (ia32_map_pd(&pd) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  pde_start = IA32_PDE_ENTRY(address);
  pte_start = IA32_PTE_ENTRY(address);
  pde_end = IA32_PDE_ENTRY(address + size);
  pte_end = IA32_PTE_ENTRY(address + size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      /*
       * a)
       */

      if (ia32_pd_get_table(&pd, pde, &pt) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /*
       * b)
       */

      if (ia32_map_pt(&pt) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PT_MAX_ENTRIES);
	   pte++)
	{
	  if (ia32_pt_delete_page(&pt, pte) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (asid == kasid)
	    ia32_tlb_invalidate(IA32_ENTRY_ADDR(pde, pte));
	}

      /*
       * d)
       */

      if (ia32_unmap_chunk((t_vaddr)pt.vaddr) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /*
       * e)
       */

      if (pde != pde_start && pde != pde_end)
	{
	  if (ia32_pd_delete_table(&pd, pde) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  if (asid == kasid)
	    ia32_tlb_invalidate(IA32_ENTRY_ADDR(IA32_PD_MIRROR, pde));

	  if (segment_release((i_segment)pt.paddr) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}
    }

  /*
   * 5)
   */

  if (asid != kasid)
    if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
      return (ERROR_UNKNOWN);

  /*						    [endblock::unmap_region] */

  return (ERROR_NONE);
}
