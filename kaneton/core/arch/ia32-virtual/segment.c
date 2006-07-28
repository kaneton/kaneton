/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [fri jul 28 17:41:09 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  file implements dependent  code for  segment manager  on ia32
 * with paging architecture.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_segment*	segment;
extern i_as		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager dispatch.
 */

d_segment		segment_dispatch =
  {

/*                                                                  [cut] k2 */

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_segment_read,
    ia32_segment_write,
    ia32_segment_copy,
    NULL,
    NULL,
    NULL,
    ia32_segment_perms,
    NULL,
    NULL,
    ia32_segment_init,
    ia32_segment_clean

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function reads directly from a segment to a buffer.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) map the segment portion with a region.
 * 3) copy from segment to the buffer.
 * 4) unmap the region.
 */

t_error			ia32_segment_read(i_region		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz)
{
  o_segment*		o;
  i_region		reg;
  t_vsize		size;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs % PAGESZ)
    {
      sz += offs - (offs / PAGESZ * PAGESZ);
      offs = offs / PAGESZ * PAGESZ;
    }

  if (sz % PAGESZ)
    size = sz + PAGESZ - (sz % PAGESZ);
  else
    size = sz;

  if (region_reserve(kasid, segid, offs, REGION_OPT_PRIVILEGED,
		     0, size, &reg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy(buff, (void*)(t_vaddr)reg, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function write directly to a segment from a buffer.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) map the segment portion with a region.
 * 3) copy from the buffer to the segment.
 * 4) unmap the region.
 */

t_error			ia32_segment_write(i_region		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz)
{
  o_segment*		o;
  i_region		reg;
  t_vsize		size;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs % PAGESZ)
    {
      sz += offs - (offs / PAGESZ * PAGESZ);
      offs = offs / PAGESZ * PAGESZ;
    }

  if (sz % PAGESZ)
    size = sz + PAGESZ - (sz % PAGESZ);
  else
    size = sz;

  if (region_reserve(kasid, segid, offs, REGION_OPT_PRIVILEGED,
		     0, size, &reg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)reg, buff, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 * steps:
 *
 * 1) get the segment objects.
 * 2) map temporarily the two segments.
 * 3) do the copy.
 * 4) unmap the segments.
 */

t_error			ia32_segment_copy(i_region		dst,
					  t_paddr		offsd,
					  i_region		src,
					  t_paddr		offss,
					  t_psize		sz)
{
  i_region		regs;
  i_region		regd;
  o_segment*		segs;
  o_segment*		segd;
  t_vsize		size;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(dst, &segd) != ERROR_NONE ||
      segment_get(src, &segs) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (sz % PAGESZ)
    size = sz + PAGESZ - (sz % PAGESZ);
  else
    size = sz;

  if (region_reserve(kasid, src, offss, REGION_OPT_PRIVILEGED,
		     0, size, &regs) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
  if (region_reserve(kasid, dst, offsd, REGION_OPT_PRIVILEGED,
		     0, size, &regd) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)regd, (void*)(t_vaddr)regs, sz);

  /*
   * 4)
   */

  if (region_release(kasid, regs) != ERROR_NONE ||
      region_release(kasid, regd) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function sets the permissions of a segment.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) get the address space and the kernel address space object.
 * 3) search for regions mapping the segment.
 *  a) map the page directory if necessary.
 *  b) loop throught the page directory entries.
 *  c) map the page table temporarily.
 *  d) loop throught the page table entries.
 *  e) change the read/write flag.
 *  f) invalidate the TLB if necessary.
 *  g) unmap the page table.
 * 4) unmap the page directory if needed.
 */

t_error			ia32_segment_perms(i_segment		segid,
					   t_perms		perms)
{
  t_ia32_directory	pd = NULL;;
  t_ia32_table		pt;
  t_ia32_page		pg;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;
  o_segment*		seg;
  o_region*		reg;
  o_as*			as;
  o_as*			kas;
  t_state		state;
  t_iterator		it;
  t_vaddr		vaddr;
  t_vaddr		chunk;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (as_get(kasid, &kas) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (as_get(seg->asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, as->regions, &it, state)
    {
      if (set_object(as->regions, it, (void**)&reg) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      if (reg->segid == segid)
	{

	  /*
	   * a)
	   */

	  if (pd == NULL)
	    {
	      if (seg->asid == kasid)
		{
		  pd = as->machdep.pd;
		}
	      else
		{
		  if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		  pd = (t_ia32_directory)(t_uint32)chunk;

		  if (ia32_region_map_chunk((t_vaddr)pd,
					    (t_paddr)as->machdep.pd) !=
		      ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
		}
	    }

	  /*
	   * b)
	   */

	  vaddr = reg->address;

	  pde_start = PDE_ENTRY(vaddr);
	  pte_start = PTE_ENTRY(vaddr);
	  pde_end = PDE_ENTRY(vaddr + reg->size);
	  pte_end = PTE_ENTRY(vaddr + reg->size);

	  for (pde = pde_start; pde <= pde_end; pde++)
	    {
	      if (pd_get_table(&pd, pde, &pt) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      /*
	       * c)
	       */

	      if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      if (ia32_region_map_chunk((t_vaddr)chunk,
					(t_paddr)pt.entries) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	      pt.entries = chunk;

	      /*
	       * d)
	       */

	      for (pte = (pde == pde_start ? pte_start : 0);
		   pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
		   pte++)
		{
		  /*
		   * e)
		   */

		  if (pt_get_page(&pt, pte, &pg) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		  pg.rw = !!(perms & PERM_WRITE);

		  if (pt_add_page(&pt, pte, pg) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		  /*
		   * f)
		   */

		  if (seg->asid == kasid)
		    tlb_invalidate((t_vaddr)ENTRY_ADDR(pde, pte));
		}

	      /*
	       * g)
	       */

	      if (ia32_region_unmap_chunk((t_vaddr)pt.entries) != ERROR_NONE)
		SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	    }
	}
    }

  /*
   * 4)
   */

  if (pd != NULL)
    {
      if (seg->asid != kasid)
	if (ia32_region_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
	  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
    }

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function just initialises the machine-dependent segment manager.
 *
 * steps:
 *
 * 1) initialise protected-mode.
 * 2) insert code and data segments for kernel task.
 * 3) insert code and data segments for drivers.
 * 4) insert code and data segments for services.
 * 5) insert code and data segments for userland programs.
 * 6) update segment selector registers.
 * 7) setup the segment selector for interrupts.
 */

t_error			ia32_segment_init(void)
{
  t_ia32_segment	seg;
  t_uint16		kcs;
  t_uint16		kds;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (pmode_init() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_CORE_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_CORE_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_DRIVER_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_DRIVER_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_SERVICE_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_SERVICE_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_PROGRAM_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_PROGRAM_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 6)
   */

  gdt_build_selector(PMODE_GDT_CORE_CS, ia32_prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_GDT_CORE_DS, ia32_prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

  /*
   * 7)
   */

  interrupt_ds = kds;

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent segment manager.
 */

t_error			ia32_segment_clean(void)
{
  SEGMENT_ENTER(segment);

  if (pmode_clean() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
