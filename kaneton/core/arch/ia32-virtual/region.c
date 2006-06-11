/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [sun jun 11 17:13:20 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for region  manager on  ia32 with
 * paging architecture.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students will have to develop this entire part of the project. take
 * a look at the dispatch structure declaration to fill it correctly
 * and to write needed functions.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_region*	region;
extern i_as		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager dispatch.
 */

d_region		region_dispatch =
  {

/*                                                                  [cut] k2 */

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_region_reserve,
    ia32_region_release,
    NULL,
    ia32_region_init,
    ia32_region_clean

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

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

t_error			ia32_region_map_chunk(t_vaddr		v,
					      t_paddr		p)
{
  o_region		oreg;
  t_ia32_table		pt;
  t_ia32_page		pg;
  i_segment		seg;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (pd_get_table(NULL, PDE_ENTRY(v), &pt) != ERROR_NONE)
    {
      if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			  &seg) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.rw = 1;
      pt.present = 1;
      pt.user = 0;
      pt.entries = seg;

      if (pd_add_table(NULL, PDE_ENTRY(v), pt) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      tlb_invalidate(ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(v)));

      memset((void*)ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(v)), 0, PAGESZ);
    }

  /*
   * 2)
   */

  pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(v));

  pg.rw = 1;
  pg.present = 1;
  pg.user = 0;
  pg.addr = p;

  if (pt_add_page(&pt, PTE_ENTRY(v), pg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  oreg.segid = p;
  oreg.address = v;
  oreg.offset = 0;
  oreg.size = PAGESZ;

  if (region_inject(kasid, &oreg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  tlb_invalidate(v);

  REGION_LEAVE(region, ERROR_NONE);
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

t_error			ia32_region_unmap_chunk(t_vaddr		v)
{
  t_ia32_table		pt;
  o_as*			as;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (pd_get_table(NULL, PDE_ENTRY(v), &pt) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(v));

  if (pt_delete_page(&pt, PTE_ENTRY(v)) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(kasid, &as) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (set_remove(as->regions, v) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  tlb_invalidate(v);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * reserves a region.
 *
 * steps:
 *
 * 1) get the as object and the kernel page-directory.
 * 2) get the task object (used to check privilege level).
 * 3) get the segment object (for permissions and physical base address).
 * 4) fill the t_page structure.
 * 5) map the as page-directory into the kernel.
 * 6) loop throught the virtual memory to map.
 *  a) get the needed page-table. if not present, the create it.
 *  b) temporarily map the page-table in the kernel as.
 *  c) loop throught the page-table.
 *  d) fill corresponding entries.
 *  e) invalidate translation lookaside buffers.
 *  f) unmap the temporarily mapped table.
 * 7) unmap the page-directory.
 */

t_error			ia32_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid)
{
  o_as*			o;
  o_segment*		oseg;
  o_task*		otsk;
  o_as*			kas;
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
  t_vaddr		chunk;
  i_segment		ptseg;
  t_uint32		clear_pt;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(kasid, &kas) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (as_get(asid, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(o->tskid, &otsk) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (segment_get(segid, &oseg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 4)
   */

  pg.rw = !!(oseg->perms & PERM_WRITE);
  pg.present = 1;
  pg.user = (otsk->class == TASK_CLASS_PROGRAM);

  /*
   * 5)
   */

  if (asid == kasid)
    {
      pd = o->machdep.pd;
    }
  else
    {
      if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pd = (t_ia32_directory)(t_uint32)chunk;

      if (ia32_region_map_chunk((t_vaddr)pd, (t_paddr)o->machdep.pd) !=
	  ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  paddr = oseg->address + offset;
  vaddr = address;

  pde_start = PDE_ENTRY(vaddr);
  pte_start = PTE_ENTRY(vaddr);
  pde_end = PDE_ENTRY(vaddr + size);
  pte_end = PTE_ENTRY(vaddr + size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      /* XXX some cases, empty pt is added */

      /*
       * a)
       */

      if (pd_get_table(&pd, pde, &pt) != ERROR_NONE)
	{
	  pt.rw = 1;
	  pt.present = 1;
	  pt.user = 0;

	  if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE,
			      &ptseg) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  pt.entries = ptseg;

	  if (pd_add_table(&pd, pde, pt) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  clear_pt = 1;
	}
      else
	clear_pt = 0;

      /*
       * b)
       */

      if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (ia32_region_map_chunk((t_vaddr)chunk,
				(t_paddr)pt.entries) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.entries = chunk;

      if (clear_pt)
	memset((void*)chunk, 0, PAGESZ);

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
	   pte++)
	{
	  /*
	   * d)
	   */

	  pg.addr = paddr;
	  paddr += PAGESZ;

	  if (pt_add_page(&pt, pte, pg) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  /*
	   * e)
	   */

	  if (asid == kasid)
	    tlb_invalidate((t_vaddr)ENTRY_ADDR(pde, pte));
	}

      /*
       * f)
       */

      if (ia32_region_unmap_chunk((t_vaddr)pt.entries) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 7)
   */

  if (asid != kasid)
    if (ia32_region_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
      REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function  releases a region.  we unmap the region  and release
 * page tables.
 *
 * steps:
 *
 * 1) get the kernel as object and the as proper object.
 * 2) get the region object.
 * 3) map the as page-directory temporarily.
 * 4) loop throught the page-directory and page-table entries.
 *  a) get the page-table.
 *  b) map it temporarily.
 *  c) delete the entries and flush the tlb entries.
 *  d) unmap the page-table.
 *  e) if the resulting page-table is empty, release it.
 * 5) unmap the as page-directory.
 */

t_error			ia32_region_release(i_as		asid,
					    i_region		regid)
{
  o_as*			o;
  o_as*			kas;
  o_region*		oreg;
  t_ia32_directory	pd;
  t_ia32_table		pt;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;
  t_vaddr		chunk;
  t_paddr		table_address;

  REGION_ENTER(region);

  /*
   * 1)
   */

  if (as_get(kasid, &kas) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (as_get(asid, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (region_get(asid, regid, &oreg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (asid == kasid)
    {
      pd = o->machdep.pd;
    }
  else
    {
      if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pd = (t_ia32_directory)(t_uint32)chunk;

      if (ia32_region_map_chunk((t_vaddr)pd,
				(t_paddr)o->machdep.pd) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  pde_start = PDE_ENTRY(oreg->address);
  pte_start = PTE_ENTRY(oreg->address);
  pde_end = PDE_ENTRY(oreg->address + oreg->size);
  pte_end = PTE_ENTRY(oreg->address + oreg->size);

  for (pde = pde_start; pde <= pde_end; pde++)
    {
      /*
       * a)
       */

      if (pd_get_table(&pd, pde, &pt) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      table_address = pt.entries;

      /*
       * b)
       */

      if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (ia32_region_map_chunk((t_vaddr)chunk,
				(t_paddr)pt.entries) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      pt.entries = chunk;

      /*
       * c)
       */

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
	   pte++)
	{
	  if (pt_delete_page(&pt, pte) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  if (asid == kasid)
	    tlb_invalidate(ENTRY_ADDR(pde, pte));
	}

      /*
       * d)
       */

      if (ia32_region_unmap_chunk((t_vaddr)pt.entries) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      /*
       * e)
       */

      if (pde != pde_start && pde != pde_end)
	{
	  if (pd_delete_table(&pd, pde) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);

	  if (asid == kasid)
	    tlb_invalidate(ENTRY_ADDR(PD_MIRROR, pde));

	  if (segment_release((i_segment)table_address) != ERROR_NONE)
	    REGION_LEAVE(region, ERROR_UNKNOWN);
	}
    }

  /*
   * 5)
   */

  if (asid != kasid)
    if (ia32_region_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
      REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function just initialises the machine-dependent region manager.
 *
 * for the intel 32-bit architecture we just initialise the paging mode.
 */

t_error			ia32_region_init(t_vaddr		start,
					 t_vsize		size)
{
  REGION_ENTER(region);

  if (paging_init() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent region manager.
 */

t_error			ia32_region_clean(void)
{
  REGION_ENTER(region);

  if (paging_clean() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
