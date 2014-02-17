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

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern t_init*		init;
extern i_as		kasid;
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
 * 5) inject the apic page.
 * 6) clean the page directory.
 * 7) reinject the page tables in the kernel as.
 * 8) invalidate mmu caches.
 * 9) setup the pdbr to use on interrupts.
 */

t_status		ia32_kernel_as_init(i_as	asid)
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

  if (as_get(asid, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  kasid = asid;

  /*
   * 2)
   */

  memcpy(&o->machine.pd, &init->machine.pd, sizeof (t_ia32_directory));

  if (ia32_pd_activate(o->machine.pd,
		       IA32_PD_CACHED,
		       IA32_PD_WRITEBACK) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  if (ia32_pt_build((t_paddr)o->machine.pd, &pt) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  pt.present = 1;
  pt.rw = IA32_PT_WRITABLE;
  pt.user = IA32_PT_PRIVILEGED;
  pt.cached = IA32_PT_NOTCACHED;
  pt.writeback = IA32_PT_WRITEBACK;

  if (ia32_pd_add_table(&o->machine.pd, IA32_PD_MIRROR, pt) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 4)
   */

  if ((preg = malloc(sizeof (o_region))) == NULL)
    return (STATUS_UNKNOWN_ERROR);

  preg->address = IA32_ENTRY_ADDR(IA32_PD_MIRROR, 0);
  preg->segid = (i_segment)pt.paddr;
  preg->offset = 0;
  preg->size = IA32_PT_MAX_ENTRIES * PAGESZ;
  preg->opts = REGION_OPT_NONE;

  if (region_inject(asid, preg, &useless) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 4)
   */

  if ((oseg = malloc(sizeof (o_segment))) == NULL)
    return (STATUS_UNKNOWN_ERROR);

  oseg->type = SEGMENT_TYPE_SYSTEM;
  oseg->address = IA32_APIC_REG_BASE;
  oseg->size = PAGESZ;
  oseg->perms = PERM_READ | PERM_WRITE;

  if (segment_inject(asid, oseg, &seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if ((preg = malloc(sizeof (o_region))) == NULL)
    return (STATUS_UNKNOWN_ERROR);

  preg->address = IA32_APIC_REG_BASE;
  preg->segid = seg;
  preg->offset = 0;
  preg->size = PAGESZ;
  preg->opts = REGION_OPT_NONE;

  if (region_inject(asid, preg, &useless) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

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
	      ia32_pd_get_table(&o->machine.pd, pde, &pt) == STATUS_OK)
	    {
	      pt.vaddr = pt.paddr;

	      for (pte = (pde == pde_start ? pte_start : 0);
		   pte < (pde == pde_end ? pte_end : IA32_PT_MAX_ENTRIES);
		   pte++)
		{
		  if (IA32_ENTRY_ADDR(pde, pte) != IA32_APIC_REG_BASE)
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
	  if (ia32_pd_get_table(&o->machine.pd, pde, &pt) == STATUS_OK)
	    {
	      seg = pt.paddr;

	      if (segment_get(seg, &oseg) != STATUS_OK)
		{
		  if ((pt_seg = malloc(sizeof (o_segment))) == NULL)
		    return (STATUS_UNKNOWN_ERROR);

		  pt_seg->type = SEGMENT_TYPE_SYSTEM;
		  pt_seg->address = (t_paddr)seg;
		  pt_seg->size = PAGESZ;
		  pt_seg->perms = PERM_READ | PERM_WRITE;

		  if (segment_inject(asid, pt_seg, &seg) != STATUS_OK)
		    return (STATUS_UNKNOWN_ERROR);
		}
	    }
	}
    }

  /*
   * 7)
   */


  /* XXX flush AP */

  ia32_tlb_flush();

  /*
   * 8)
   */

  if (ia32_pd_get_cr3((t_uint32*)&ia32_interrupt_pdbr,
		      o->machine.pd,
		      IA32_PD_CACHED,
		      IA32_PD_WRITEBACK) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  return (STATUS_OK);
}

/*
 * this function finalize the kernel as creation by marking the system
 * segments.
 */

t_status		ia32_kernel_as_finalize(void)
{
  if (segment_type((i_segment)init->segments[0].address,
		   SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);
  if (segment_type((i_segment)init->segments[1].address,
		   SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);
  if (segment_type((i_segment)init->segments[11].address,
		   SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);
  if (segment_type((i_segment)init->segments[12].address,
		   SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);
  if (segment_type((i_segment)init->segments[13].address,
		   SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  return (STATUS_OK);
}

/*
 * this function initializes an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) reserve a segment for the directory.
 * 3) build a new page directory for the as.
 * 4) release the kernel-side region mapping the directory.
 * 5) map the mandatory regions (gdt, idt, kernel handlers, tss)
 */

t_status		ia32_task_as_init(i_as		asid)
{
  o_as*			o;
  i_segment	        seg;
  i_region		reg;
  o_region*		preg;
  t_paddr		base;
  t_ia32_directory	pd;

  /*
   * 1)
   */

  if (as_get(asid, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (segment_reserve(asid, PAGESZ, PERM_READ | PERM_WRITE,
		      &seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  pd = o->machine.pd = (t_ia32_directory)(t_uint32)seg;

  if (ia32_map_pd(&pd) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  memset((void*)pd, 0, PAGESZ);

  /*
   * 4)
   */

  if (ia32_unmap_chunk((t_vaddr)pd) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 5)
   */

#ifdef IA32_KERNEL_MAPPED
  if (region_reserve(asid,
		     (i_segment)0x1000,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_GLOBAL,
		     0x1000,
		     0x00100000 - 0x1000,
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(asid,
		     (i_segment)init->kcode,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_GLOBAL,
		     init->kcode,
		     init->kcodesz,
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);
#else
  if (region_reserve(asid,
		     (i_segment)init->kcode,
		     LINKER_SYMBOL(_handler_begin) - init->kcode,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     LINKER_SYMBOL(_handler_begin),
		     LINKER_SYMBOL(_handler_end) -
		     LINKER_SYMBOL(_handler_begin),
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(asid,
		     (i_segment)init->kcode,
		     LINKER_SYMBOL(_handler_data_begin) - init->kcode,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     LINKER_SYMBOL(_handler_data_begin),
		     LINKER_SYMBOL(_handler_data_end) -
		     LINKER_SYMBOL(_handler_data_begin),
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);
#endif

  if (region_get(kasid, (i_region)(t_uint32)thread->machine.tss,
		 &preg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(asid,
		     preg->segid,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)thread->machine.tss,
		     preg->size,
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(asid,
		     (i_segment)(t_uint32)ia32_gdt.descriptor,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)ia32_gdt.descriptor,
		     PAGESZ,
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(asid,
		     (i_segment)(t_uint32)ia32_idt.descriptor,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)ia32_idt.descriptor,
		     PAGESZ,
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(asid,
		     (i_segment)IA32_APIC_REG_BASE,
		     0,
		     REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
		     REGION_OPT_GLOBAL,
		     (t_vaddr)IA32_APIC_REG_BASE,
		     PAGESZ,
		     &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  return (STATUS_OK);
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

t_status		ia32_segmentation_init(void)
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
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_CORE_CS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_CORE_DS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_driver;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_DRIVER_CS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_driver;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_DRIVER_DS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_service;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_SERVICE_CS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_service;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_SERVICE_DS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 4)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_PROGRAM_CS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_PROGRAM_DS,
			   seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 5)
   */

  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_CS, ia32_prvl_supervisor, &kcs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_DS, ia32_prvl_supervisor, &kds);
  ia32_pmode_set_segment_registers(kcs, kds);

  /* XXX flush AP */

  /*
   * 6)
   */

  ia32_interrupt_ds = kds;

  return (STATUS_OK);
}
