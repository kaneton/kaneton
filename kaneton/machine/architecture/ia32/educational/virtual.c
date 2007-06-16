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
extern t_uint32		interrupt_pdbr;
extern t_uint16		interrupt_ds;
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
    AS_LEAVE(as, ERROR_UNKNOWN);

  kasid = asid;

  /*
   * 2)
   */

  memcpy(&o->machdep.pd, &init->machdep.pd, sizeof(t_ia32_directory));

  if (pd_activate(o->machdep.pd, PD_CACHED, PD_WRITEBACK) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  pt.present = 1;
  pt.rw = PT_WRITABLE;
  pt.user = PT_PRIVILEGED;
  pt.cached = PT_NOTCACHED;
  pt.writeback = PT_WRITEBACK;
  pt.entries = (t_paddr)o->machdep.pd;

  if (pd_add_table(&o->machdep.pd, PD_MIRROR, pt) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  if ((preg = malloc(sizeof(o_region))) == NULL)
    return (ERROR_UNKNOWN);

  preg->address = ENTRY_ADDR(PD_MIRROR, 0);
  preg->segid = (i_segment)pt.entries;
  preg->offset = 0;
  preg->size = PT_MAX_ENTRIES * PAGESZ;
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
	  pde_end = PDE_ENTRY(init->regions[i].address);
	  pte_end = PTE_ENTRY(init->regions[i].address);
	}
      else
	{
	  pde_end = PD_MAX_ENTRIES - 1;
	  pte_end = PT_MAX_ENTRIES;
	}

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  if (pde != PD_MIRROR &&
	      pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
	    {
	      for (pte = (pde == pde_start ? pte_start : 0);
		   pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
		   pte++)
		{
		  pt_delete_page(&pt, pte);
		}
	    }
	}

      if (i != init->nregions)
	{
	  pde_start = PDE_ENTRY(init->regions[i].address +
				init->regions[i].size);
	  pte_start = PTE_ENTRY(init->regions[i].address +
				init->regions[i].size);
	}
    }

  /*
   * 6)
   */

  for (i = 0; i < init->nregions; i++)
    {
      pde_start = PDE_ENTRY(init->regions[i].address);
      pde_end = PDE_ENTRY(init->regions[i].address +
			  init->regions[i].size);

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  if (pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
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

  tlb_flush();

  /*
   * 8)
   */

  if (pd_get_cr3(&interrupt_pdbr, o->machdep.pd, PD_CACHED,
		 PD_WRITEBACK) != ERROR_NONE)
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

  if (pd_build(base, &o->machdep.pd, 1) != ERROR_NONE)
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
 * this function just initialises the machine-dependent segment manager.
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

  interrupt_ds = kds;

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
