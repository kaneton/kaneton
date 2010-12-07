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

/*
 * the init structure.
 */

extern t_init*		_init;

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * the thread manager.
 */

extern m_thread*	_thread;

/*
 * the IDT structure.
 */

extern t_ia32_idt	ia32_idt;

/*
 * the GDT structure.
 */

extern t_ia32_gdt	ia32_gdt;

/*
 * the layout's .handler and .handler_data addresses.
 */

extern t_vaddr		_handler_begin;
extern t_vaddr		_handler_end;
extern t_vaddr		_handler_data_begin;
extern t_vaddr		_handler_data_end;

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

t_error			ia32_kernel_as_initialize(i_as		asid)
{
  o_as*			o;
  i_segment	        seg;
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

  if (as_get(asid, &o) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  memcpy(&o->machine.pd, &_init->machine.pd, sizeof(t_ia32_directory));

  if (ia32_pd_activate(o->machine.pd,
		       IA32_PAGE_DIRECTORY_CACHED, // XXX CACHED
		       IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  if (ia32_pt_build((t_paddr)o->machine.pd, &pt) != ERROR_OK)

    return (ERROR_KO);

  pt.present = 1;
  pt.rw = IA32_PAGE_TABLE_WRITABLE;
  pt.user = IA32_PAGE_TABLE_PRIVILEGED;
  pt.cached = IA32_PAGE_TABLE_CACHED; // XXX CACHED
  pt.writeback = IA32_PAGE_TABLE_WRITEBACK;

  if (ia32_pd_add_table(&o->machine.pd,
			IA32_PAGE_DIRECTORY_MIRROR,
			pt) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 4)
   */

  // XXX a noter que l'on inject pas de segment ni de region pour le page directory
  // car ils sont dans init segment/regions et seront injectes dans task_initialize()

  if ((preg = malloc(sizeof(o_region))) == NULL)
    return (ERROR_KO);

  preg->address = IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR, 0);
  preg->segment = (i_segment)pt.paddr;
  preg->offset = 0;
  preg->size = IA32_PAGE_TABLE_MAX_ENTRIES * PAGESZ; // XXX
  preg->options = REGION_OPTION_NONE;

  if (region_inject(asid, preg, &useless) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 5)
   */

  pde_start = 0;
  pte_start = 0;

  for (i = 0; i <= _init->nregions; i++)
    {
      if (i != _init->nregions)
	{
	  pde_end = IA32_PAGE_DIRECTORY_ENTRY_INDEX(_init->regions[i].address);
	  pte_end = IA32_PAGE_TABLE_ENTRY_INDEX(_init->regions[i].address);
	}
      else
	{
	  pde_end = IA32_PAGE_DIRECTORY_MAX_ENTRIES - 1;
	  pte_end = IA32_PAGE_TABLE_MAX_ENTRIES;
	}

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  if (pde != IA32_PAGE_DIRECTORY_MIRROR &&
	      ia32_pd_get_table(&o->machine.pd, pde, &pt) == ERROR_OK)
	    {
	      pt.vaddr = pt.paddr;

	      for (pte = (pde == pde_start ? pte_start : 0);
		   pte < (pde == pde_end ? pte_end :
			  IA32_PAGE_TABLE_MAX_ENTRIES);
		   pte++)
		{
		  ia32_pt_delete_page(&pt, pte);
		}
	    }
	}

      if (i != _init->nregions)
	{
	  pde_start =
	    IA32_PAGE_DIRECTORY_ENTRY_INDEX(_init->regions[i].address +
					    _init->regions[i].size);
	  pte_start =
	    IA32_PAGE_TABLE_ENTRY_INDEX(_init->regions[i].address +
					_init->regions[i].size);
	}
    }

  /*
   * 6)
   */

  for (i = 0; i < _init->nregions; i++)
    {
      pde_start = IA32_PAGE_DIRECTORY_ENTRY_INDEX(_init->regions[i].address);
      pde_end = IA32_PAGE_DIRECTORY_ENTRY_INDEX(_init->regions[i].address +
						_init->regions[i].size);

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  if (ia32_pd_get_table(&o->machine.pd, pde, &pt) == ERROR_OK)
	    {
	      seg = pt.paddr;

	      if (segment_exist(seg) == ERROR_FALSE)
		{
		  if ((pt_seg = malloc(sizeof(o_segment))) == NULL)
		    return (ERROR_KO);

		  pt_seg->type = SEGMENT_TYPE_SYSTEM;
		  pt_seg->address = (t_paddr)seg;
		  pt_seg->size = PAGESZ;
		  pt_seg->permissions = PERMISSION_READ | PERMISSION_WRITE;

		  if (segment_inject(asid, pt_seg, &seg) != ERROR_OK)
		    return (ERROR_KO);
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

  if (ia32_pd_get_cr3((t_uint32*)&ia32_interrupt_pdbr,
		      o->machine.pd,
		      IA32_PAGE_DIRECTORY_CACHED, // XXX CACHED
		      IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}

/*
 * this function finalize the kernel as creation by marking the system
 * segments.
 */

t_error			ia32_kernel_as_finalize(void)
{
  if (segment_type((i_segment)_init->segments[0].address,
		   SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    return (ERROR_KO);

  if (segment_type((i_segment)_init->segments[1].address,
		   SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    return (ERROR_KO);

  if (segment_type((i_segment)_init->segments[11].address,
		   SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    return (ERROR_KO);

  if (_init->segments[12].size != 0)
    {
      if (segment_type((i_segment)_init->segments[12].address,
		       SEGMENT_TYPE_SYSTEM) != ERROR_OK)
	return (ERROR_KO);
    }

  return (ERROR_OK);
}

/*
 * this function initializes an address space for a normal---i.e
 * non-kernel---task.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) reserve a segment for the directory.
 * 3) build a new page directory for the as.
 * 4) release the kernel-side region mapping the directory.
 * 5) map the mandatory regions (gdt, idt, kernel handlers, tss)
 */

t_error			ia32_task_as_initialize(i_as		asid)
{
  o_as*			o;
  i_segment	        seg;
  i_region		reg;
  o_region*		preg;
  t_ia32_directory	pd;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  if (segment_reserve(asid,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    return (ERROR_KO);

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  pd = o->machine.pd = (t_ia32_directory)(t_uint32)seg;

  if (ia32_map_pd(&pd) != ERROR_OK)
    return (ERROR_KO);

  memset((void*)pd, 0, PAGESZ);

  /*
   * 4)
   */

  if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 5)
   */

  if (region_get(_kernel->as,
		 (i_region)(t_vaddr)_thread->machine.tss,
		 &preg) != ERROR_OK)
    return (ERROR_KO);

  if (region_reserve(asid,
		     preg->id,
		     0,
		     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)_thread->machine.tss,
		     preg->size,
		     &reg) != ERROR_OK)
    return (ERROR_KO);

  if (region_reserve(asid,
		     (i_segment)(t_uint32)ia32_gdt.descriptor,
		     0,
		     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)ia32_gdt.descriptor,
		     PAGESZ,
		     &reg) != ERROR_OK)
    return (ERROR_KO);

  if (region_reserve(asid,
		     (i_segment)(t_uint32)ia32_idt.descriptor,
		     0,
		     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)ia32_idt.descriptor,
		     PAGESZ,
		     &reg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * XXX
   */

  if (asid != _kernel->as)
    {
      // XXX on map le kernel code/data car lors d'une interruption,
      // XXX on arrive dans une page qui contient l'ISR.
      // XXX dans la version educationa, on switch dans l'AS kernel
      // XXX a chaque fois pour eviter de propager les modifs de page
      // XXX directory/table des que le kernel alloue dynamiquement des
      // XXX donnees ou d'utiliser du mapping de porc a la linux. cela dit,
      // XXX l'ISR doit connaitre l'AS du kernel. il faut donc mappe les
      // XXX donnees du kernel ainsi que son code (pour le code de l'ISR).

      /* XXX
	 if (region_reserve(asid,
	 (i_segment)_init->kcode,
	 LINKER_SYMBOL(_handler_begin) - _init->kcode,
	 REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
	 REGION_OPTION_GLOBAL,
	 LINKER_SYMBOL(_handler_begin),
	 LINKER_SYMBOL(_handler_end) -
	 LINKER_SYMBOL(_handler_begin),
	 &reg) != ERROR_OK)
	 return (ERROR_KO);

	 if (region_reserve(asid,
	 (i_segment)_init->kcode,
	 LINKER_SYMBOL(_handler_data_begin) - _init->kcode,
	 REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
	 REGION_OPTION_GLOBAL,
	 LINKER_SYMBOL(_handler_data_begin),
	 LINKER_SYMBOL(_handler_data_end) -
	 LINKER_SYMBOL(_handler_data_begin),
	 &reg) != ERROR_OK)
	 return (ERROR_KO);
      */

      i_region	region;

      if (region_reserve(asid,
			 (i_segment)_init->kcode,
			 0,
			 REGION_OPTION_FORCE | REGION_OPTION_GLOBAL,
			 _init->kcode,
			 _init->kcodesz,
			 &region) != ERROR_OK)
	return (ERROR_KO);

      if (region_reserve(asid,
			 (i_segment)_init->kstack,
			 0,
			 REGION_OPTION_FORCE | REGION_OPTION_GLOBAL,
			 (t_vaddr)_init->kstack,
			 _init->kstacksz,
			 &region) != ERROR_OK)
	return (ERROR_KO);
    }

  return (ERROR_OK);
}

/*
 * this function just initializes the machine-dependent segment manager.
 *
 * steps:
 *
 * 1) insert code and data segments for kernel task.
 * 2) insert code and data segments for drivers.
 * 3) insert code and data segments for services.
 * 4) insert code and data segments for userland guests.
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
  seg.privilege = ia32_privilege_kernel;
  seg.is_system = 0;
  seg.type.user = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_KERNEL_CS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_kernel;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_KERNEL_DS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_driver;
  seg.is_system = 0;
  seg.type.user = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_DRIVER_CS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_driver;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_DRIVER_DS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_service;
  seg.is_system = 0;
  seg.type.user = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_SERVICE_CS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_service;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_SERVICE_DS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 4)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_guest;
  seg.is_system = 0;
  seg.type.user = ia32_type_code;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_GUEST_CS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_guest;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_GUEST_DS,
			   seg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 5)
   */

  ia32_gdt_build_selector(IA32_PMODE_GDT_KERNEL_CS,
			  ia32_privilege_kernel,
			  &kcs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_KERNEL_DS,
			  ia32_privilege_kernel,
			  &kds);
  ia32_pmode_set_segment_registers(kcs, kds);

  /*
   * 6)
   */

  ia32_interrupt_ds = kds;

  return (ERROR_OK);
}
