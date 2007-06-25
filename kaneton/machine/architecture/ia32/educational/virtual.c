/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/virtual.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [fri jun 15 09:31:08 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern t_init*		init;
extern i_as		kasid;
extern volatile t_uint32		ia32_interrupt_pdbr;
extern volatile t_uint16		ia32_interrupt_ds;
extern m_thread*	thread;
extern t_ia32_idt	ia32_idt;
extern t_ia32_gdt	ia32_gdt;

/*
 * .handler and .handler_data addresses from ld.
 */

extern t_vaddr		_handler_begin;
extern t_vaddr		_handler_end;
extern t_vaddr		_handler_data_begin;
extern t_vaddr		_handler_data_end;

// XXX move elsewhere
#define LINKER_SYMBOL(_symbol_)						\
  ((t_vaddr)&(_symbol_))

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initializes the kernel address space.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) get the page directory from the init structure.
 * 3) add the mirroring entry.
 * 4) manually reserve a region for the mirror entry.
 * 5) clean the page directory.
 * 6) reinject the page tables in the kernel as.
 * 7) invalidate mmu caches.
 * 8) setup the pdbr to use on interrupts.
 */

t_error			ia32_kernel_as_init(i_as	asid)
{
  o_as*			o;
  i_segment	        seg;
  o_segment*		oseg;
  o_segment*		pt_seg;
  o_region*		preg;
  t_ia32_table		pt;
  t_uint32		i;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;
  i_region		useless;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  kasid = asid;

  /*
   * 2)
   */

  memcpy(&o->machdep.pd, &init->machdep.pd, sizeof(t_ia32_directory));

  if (ia32_pd_activate(o->machdep.pd,
		       IA32_PD_CACHED,
		       IA32_PD_WRITEBACK) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  pt.present = 1;
  pt.rw = IA32_PT_WRITABLE;
  pt.user = IA32_PT_PRIVILEGED;
  pt.cached = IA32_PT_NOTCACHED;
  pt.writeback = IA32_PT_WRITEBACK;
  pt.entries = (t_paddr)o->machdep.pd;

  if (ia32_pd_add_table(&o->machdep.pd, IA32_PD_MIRROR, pt) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((preg = malloc(sizeof(o_region))) == NULL)
    return (ERROR_UNKNOWN);

  preg->address = IA32_ENTRY_ADDR(IA32_PD_MIRROR, 0);
  preg->segid = (i_segment)pt.entries;
  preg->offset = 0;
  preg->size = IA32_PT_MAX_ENTRIES * PAGESZ;
  preg->opts = REGION_OPT_NONE;

  if (region_inject(asid, preg, &useless) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 5)
   */

  pde_start = 0;
  pte_start = 0;

  for (i = 0; i <= init->nregions; i++)
    {
      if (i != init->nregions)
	{
	  pde_end = IA32_PDE_ENTRY(init->regions[i].address);
	  pte_end = IA32_PTE_ENTRY(init->regions[i].address);
	}
      else
	{
	  pde_end = IA32_PD_MAX_ENTRIES - 1;
	  pte_end = IA32_PT_MAX_ENTRIES;
	}

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  if (pde != IA32_PD_MIRROR &&
	      ia32_pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
	    {
	      for (pte = (pde == pde_start ? pte_start : 0);
		   pte < (pde == pde_end ? pte_end : IA32_PT_MAX_ENTRIES);
		   pte++)
		{
		  ia32_pt_delete_page(&pt, pte);
		}
	    }
	}

      if (i != init->nregions)
	{
	  pde_start = IA32_PDE_ENTRY(init->regions[i].address +
				     init->regions[i].size);
	  pte_start = IA32_PTE_ENTRY(init->regions[i].address +
				     init->regions[i].size);
	}
    }

  /*
   * 6)
   */

  for (i = 0; i < init->nregions; i++)
    {
      pde_start = IA32_PDE_ENTRY(init->regions[i].address);
      pde_end = IA32_PDE_ENTRY(init->regions[i].address +
			       init->regions[i].size);

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  if (ia32_pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
	    {
	      seg = pt.entries;

	      if (segment_get(seg, &oseg) != ERROR_NONE)
		{
		  if ((pt_seg = malloc(sizeof(o_segment))) == NULL)
		    return (ERROR_UNKNOWN);

		  pt_seg->address = (t_paddr)pt.entries;
		  pt_seg->size = PAGESZ;
		  pt_seg->perms = PERM_READ | PERM_WRITE;

		  if (segment_inject(asid, pt_seg, &seg) != ERROR_NONE)
		    return (ERROR_UNKNOWN);
		}
	    }
	}
    }

  /*
   * 7)
   */

  ia32_tlb_flush();

  /*
   * 8)
   */

  if (ia32_pd_get_cr3(&ia32_interrupt_pdbr,
		      o->machdep.pd,
		      IA32_PD_CACHED,
		      IA32_PD_WRITEBACK) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function initializes an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) reserve a segment for the directory.
 * 3) reserve a region for the directory in the kernel address space.
 * 4) build a new page directory for the as.
 * 5) release the kernel-side region mapping the directory.
 * 6) map the mandatory regions (gdt, idt, kernel stack & handlers, tss)
 */

t_error			ia32_task_as_init(i_as		asid)
{
  o_as*			o;
  i_segment	        seg;
  i_region		reg;
  o_region*		preg;
  t_paddr		base;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (region_reserve(kasid, seg, 0, REGION_OPT_PRIVILEGED,
		     0, PAGESZ, &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  base = reg;

  if (ia32_pd_build(base, &o->machdep.pd, 1) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  o->machdep.pd = (t_ia32_directory)(t_uint32)seg;

  /*
   * 5)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 6)
   */

#if 0 // XXX
  if (region_reserve(asid,
		     (i_segment)0x1000,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_GLOBAL,
		     0x1000,
		     INIT_ISA_SIZE - 0x1000,
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(asid,
		     (i_segment)init->kcode,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_GLOBAL,
		     init->kcode,
		     init->kcodesz,
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);
#else
  if (region_reserve(asid,
		     (i_segment)init->kcode,
		     LINKER_SYMBOL(_handler_begin) - init->kcode,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     LINKER_SYMBOL(_handler_begin),
		     LINKER_SYMBOL(_handler_end) -
		     LINKER_SYMBOL(_handler_begin),
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(asid,
		     (i_segment)init->kcode,
		     LINKER_SYMBOL(_handler_data_begin) - init->kcode,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     LINKER_SYMBOL(_handler_data_begin),
		     LINKER_SYMBOL(_handler_data_end) -
		     LINKER_SYMBOL(_handler_data_begin),
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);
#endif

  if (region_get(kasid, (i_region)(t_uint32)thread->machdep.tss,
		 &preg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(asid,
		     preg->segid,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)thread->machdep.tss,
		     preg->size,
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(asid,
		     (i_segment)(t_uint32)ia32_gdt.descriptor,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)ia32_gdt.descriptor,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(asid,
		     (i_segment)(t_uint32)ia32_idt.descriptor,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)ia32_idt.descriptor,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  base = thread->machdep.tss->esp0 - 2 * PAGESZ + 16;

  if (region_get(kasid, (i_region)base,
		 &preg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (region_reserve(asid,
		     preg->segid,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     base,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function just initializes the machine-dependent segment manager.
 *
 * steps:
 *
 * 1) insert code and data segments for kernel task.
 * 2) insert code and data segments for drivers.
 * 3) insert code and data segments for services.
 * 4) insert code and data segments for userland programs.
 * 5) update segment selector registers.
 * 6) setup the segment selector for interrupts.
 */

t_error			ia32_segmentation_init(void)
{
  t_ia32_segment	seg;
  t_uint16		kcs;
  t_uint16		kds;

  /*
   * 1)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_CORE_CS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_CORE_DS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_DRIVER_CS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_DRIVER_DS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_SERVICE_CS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_SERVICE_DS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_PROGRAM_CS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_PROGRAM_DS, seg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 5)
   */

  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_CS, ia32_prvl_supervisor, &kcs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_DS, ia32_prvl_supervisor, &kds);
  ia32_pmode_set_segment_registers(kcs, kds);

  /*
   * 6)
   */

  ia32_interrupt_ds = kds;

  return (ERROR_NONE);
}

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
  t_paddr		poffset;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs % PAGESZ)
  {
    poffset = offs & ~(PAGESZ - 1);
    offs -= poffset;
  }
  else
  {
    poffset = offs;
    offs = 0;
  }

  end = poffset + offs + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, segid, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy(buff, (void*)(t_vaddr)reg + offs, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
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
  t_paddr		poffset;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs % PAGESZ)
  {
    poffset = offs & ~(PAGESZ - 1);
    offs -= poffset;
  }
  else
  {
    poffset = offs;
    offs = 0;
  }

  end = poffset + offs + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, segid, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)reg + offs, buff, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
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
  t_paddr		poffset;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(dst, &segd) != ERROR_NONE ||
      segment_get(src, &segs) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offss % PAGESZ)
  {
    poffset = offss & ~(PAGESZ - 1);
    offss -= poffset;
  }
  else
  {
    poffset = offss;
    offss = 0;
  }

  end = poffset + offss + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, src, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &regs) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (offsd % PAGESZ)
  {
    poffset = offsd & ~(PAGESZ - 1);
    offsd -= poffset;
  }
  else
  {
    poffset = offsd;
    offsd = 0;
  }

  end = poffset + offsd + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, dst, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &regd) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)regd + offsd, (void*)(t_vaddr)regs + offss, sz);

  /*
   * 4)
   */

  if (region_release(kasid, regs) != ERROR_NONE ||
      region_release(kasid, regd) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

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

  if (reg == NULL)
    return (ERROR_UNKNOWN);

  /*
   * 1)
   */

  if (ia32_pd_get_table(NULL, IA32_PDE_ENTRY(v), &pt) != ERROR_NONE)
    {
      if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			  &seg) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      pt.rw = IA32_PT_WRITABLE;
      pt.present = 1;
      pt.user = IA32_PT_PRIVILEGED;
      pt.cached = IA32_PT_CACHED;
      pt.writeback = IA32_PT_WRITEBACK;
      pt.entries = seg;

      if (ia32_pd_add_table(NULL, IA32_PDE_ENTRY(v), pt) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      ia32_tlb_invalidate(IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v)));

      memset((void*)IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v)), 0,
	     PAGESZ);
    }

  /*
   * 2)
   */

  pt.entries = IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v));

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

  /*
   * 1)
   */

  if (ia32_pd_get_table(NULL, IA32_PDE_ENTRY(v), &pt) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  pt.entries = IA32_ENTRY_ADDR(IA32_PD_MIRROR, IA32_PDE_ENTRY(v));

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
 * map a region.
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
 */

t_error			ia32_map_region(i_as		asid,
					i_segment	segid,
					t_paddr		offset,
					t_opts		opts,
					t_vaddr		address,
					t_vsize		size)
{
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
  t_vaddr		chunk;
  i_segment		ptseg;
  t_uint32		clear_pt;
  void*			tmp;

  ASSERT(!(size % PAGESZ));

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

  if (asid == kasid)
    {
      pd = o->machdep.pd;
    }
  else
    {
      tmp = malloc(sizeof(o_region));

      if (region_space(kasid, PAGESZ, &chunk) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      pd = (t_ia32_directory)(t_uint32)chunk;

      if (ia32_map_chunk((t_vaddr)pd, (t_paddr)o->machdep.pd,
			 tmp) != ERROR_NONE)
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
	  pt.rw = IA32_PT_WRITABLE;
	  pt.present = 1;
	  pt.user = IA32_PT_USER;
	  pt.cached = IA32_PT_CACHED;
	  pt.writeback = IA32_PT_WRITEBACK;

	  if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE,
			      &ptseg) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  pt.entries = ptseg;

	  if (ia32_pd_add_table(&pd, pde, pt) != ERROR_NONE)
	    return (ERROR_UNKNOWN);

	  clear_pt = 1;
	}
      else
	clear_pt = 0;

      /*
       * b)
       */

      tmp = malloc(sizeof(o_region));

      if (region_space(kasid, PAGESZ, &chunk) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (ia32_map_chunk((t_vaddr)chunk, (t_paddr)pt.entries,
			 tmp) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      pt.entries = chunk;

      if (clear_pt)
	memset((void*)chunk, 0, PAGESZ);

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

      if (ia32_unmap_chunk((t_vaddr)pt.entries) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 6)
   */

  if (asid != kasid)
    if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
      return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function  releases a region.  we unmap the region  and release
 * page tables.
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
 */

t_error			ia32_unmap_region(i_as		asid,
					  t_vaddr	address,
					  t_vsize	size)
{
  o_as*			o;
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
  void*			tmp;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (asid == kasid)
    {
      pd = o->machdep.pd;
    }
  else
    {
      tmp = malloc(sizeof(o_region));

      if (region_space(kasid, PAGESZ, &chunk) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      pd = (t_ia32_directory)(t_uint32)chunk;

      if (ia32_map_chunk((t_vaddr)pd, (t_paddr)o->machdep.pd,
			 tmp) != ERROR_NONE)
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

      table_address = pt.entries;

      /*
       * b)
       */

      tmp = malloc(sizeof(o_region));

      if (region_space(kasid, PAGESZ, &chunk) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (ia32_map_chunk((t_vaddr)chunk, (t_paddr)pt.entries,
			 tmp) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      pt.entries = chunk;

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

      if (ia32_unmap_chunk((t_vaddr)pt.entries) != ERROR_NONE)
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

	  if (segment_release((i_segment)table_address) != ERROR_NONE)
	    return (ERROR_UNKNOWN);
	}
    }

  /*
   * 5)
   */

  if (asid != kasid)
    if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
      return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}
