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

extern s_init*		_init;

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
  t_ia32_directory	pd;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  o->machine.pd = _init->machine.pd;

  if (ia32_pd_activate((t_ia32_directory)o->machine.pd,
		       IA32_PAGE_DIRECTORY_CACHED,
		       IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 3)
   */

  // XXX o->machine.pd est une paddr. dans glue/init.h d'ailleurs eviter
  // de filer une structure car le kernel pourrait vouloir utiliser une
  // autre structure. il vaut mieux se contenter d'un adresse physique.
  if (ia32_pt_build(o->machine.pd, &pt) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  pt.present = 1;
  pt.rw = IA32_PAGE_TABLE_WRITABLE;
  pt.user = IA32_PAGE_TABLE_PRIVILEGED;
  pt.cached = IA32_PAGE_TABLE_CACHED;
  pt.writeback = IA32_PAGE_TABLE_WRITEBACK;

  pd = (t_ia32_directory)o->machine.pd;

  if (ia32_pd_add_table(&pd, // XXX pq & ici?
			IA32_PAGE_DIRECTORY_MIRROR,
			pt) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 4)
   */

  // XXX a noter que l'on inject pas de segment ni de region pour le page directory
  // car ils sont dans init segment/regions et seront injectes dans task_initialize()

  if ((preg = malloc(sizeof(o_region))) == NULL)
    MACHINE_ESCAPE("XXX");

  preg->address = IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR, 0);
  preg->segment = (i_segment)pt.paddr; // XXX !!!WARNING!!! ici on devrait
                                       // pouvoir recupere le vrai segment
                                       // pour extraire l'id ou le generer
                                       // avec SEGMENT_IDENTIFIER() au pire!
  preg->offset = 0;
  preg->size = IA32_PAGE_TABLE_MAX_ENTRIES * ___kaneton$pagesz; // XXX
  preg->options = REGION_OPTION_NONE;

  if (region_inject(asid, preg, &useless) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

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
	      ia32_pd_get_table(&pd, // XXX & ?
				pde,
				&pt) == ERROR_OK)
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
	  if (ia32_pd_get_table(&pd, // XXX & ?
				pde,
				&pt) == ERROR_OK)
	    {
	      i_segment		s;

	      if (segment_locate(pt.paddr, &s) == ERROR_FALSE)
		{
		  if ((pt_seg = malloc(sizeof(o_segment))) == NULL)
		    MACHINE_ESCAPE("XXX");

		  pt_seg->type = SEGMENT_TYPE_SYSTEM;
		  pt_seg->address = pt.paddr;
		  pt_seg->size = ___kaneton$pagesz;
		  pt_seg->permissions = PERMISSION_READ | PERMISSION_WRITE;

		  if (segment_inject(asid, pt_seg, &s) != ERROR_OK)
		    MACHINE_ESCAPE("XXX");
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
		      pd,
		      IA32_PAGE_DIRECTORY_CACHED,
		      IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  MACHINE_LEAVE();
}

/*
 * this function finalize the kernel as creation by marking the system
 * segments.
 */

t_error			ia32_kernel_as_finalize(void)
{
  i_segment		segment;

  if (segment_locate(_init->segments[0].address, &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("XXX");

  if (segment_type(segment,
		   SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (segment_locate(_init->segments[1].address, &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("XXX");

  if (segment_type(segment,
		   SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (segment_locate(_init->segments[11].address, &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("XXX");

  if (segment_type(segment,
		   SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  // XXX tester le nombre de segments plutot!
  if (_init->segments[12].size != 0)
    {
      if (segment_locate(_init->segments[12].address, &segment) == ERROR_FALSE)
	MACHINE_ESCAPE("XXX");

      if (segment_type(segment,
		       SEGMENT_TYPE_SYSTEM) != ERROR_OK)
	MACHINE_ESCAPE("XXX");
    }

  MACHINE_LEAVE();
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
  o_segment*		s;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (segment_reserve(asid,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (segment_get(seg, &s) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 3)
   */

  o->machine.pd = s->address;

  pd = (t_ia32_directory)o->machine.pd;

  if (ia32_map_pd(&pd) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  memset((void*)pd, 0, ___kaneton$pagesz);

  /*
   * 4)
   */

  // XXX normal pd = vaddr
  if (ia32_unmap_chunk((t_vaddr)pd) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 5)
   */

  // XXX c'est normal c'est une address virtuelle. par contre ce serait
  // mieux de changer dans glue pour avoir une vaddr et faire un cast
  // quand on veut l'utiliser, je trouverais ca plus propre. on saurait de
  // quoi on part (d'une vaddr), qu'on pourrait caster sans risquer de faire
  // une connerie alors que la on caste un truc dont on ne sait pas ce que
  // c'est: paddr? vaddr?
  if (region_locate(_kernel->as,
		    _thread->machine.tss,
		    &reg) == ERROR_FALSE)
    MACHINE_ESCAPE("XXX");

  if (region_get(_kernel->as,
		 reg,
		 &preg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  // XXX on mappe le TSS en recuperant son address physique via l'as kernel.
  if (region_reserve(asid,
		     preg->segment,
		     0,
		     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     _thread->machine.tss,
		     preg->size,
		     &reg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  //printf("GDT: 0x%x\n", ia32_gdt.descriptor);

  // XXX cast normal mais regarder si on peut pas l'eviter avec des types
  // differents
  if (segment_locate((t_paddr)ia32_gdt.descriptor, &seg) == ERROR_FALSE)
    MACHINE_ESCAPE("XXX");

  // XXX identity mapping for the GDT: car c'est une adresse physique.
  // XXX le mieux serait de faire un segment locate ici!
  if (region_reserve(asid,
		     seg,
		     0,
		     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)ia32_gdt.descriptor,
		     ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  //printf("IDT: 0x%x\n", ia32_idt.descriptor);

  // XXX cast normal mais regarder si on peut pas l'eviter avec des types
  // differents
  if (segment_locate((t_paddr)ia32_idt.descriptor, &seg) == ERROR_FALSE)
    MACHINE_ESCAPE("XXX");

  // XXX identity mapping for the IDT, car c'est une address physique.
  // le mieux serait de faire un segment locate ici!
  if (region_reserve(asid,
		     seg,
		     0,
		     REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)ia32_idt.descriptor,
		     ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * XXX
   */

  // XXX ce if() est inutile pourqu'on est dans la fonction d'init
  // des as de taches non-kernel :)
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
	 _init->kcode,
	 LINKER_SYMBOL(_handler_begin) - _init->kcode,
	 REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
	 REGION_OPTION_GLOBAL,
	 LINKER_SYMBOL(_handler_begin),
	 LINKER_SYMBOL(_handler_end) -
	 LINKER_SYMBOL(_handler_begin),
	 &reg) != ERROR_OK)
	 MACHINE_ESCAPE("XXX");

	 if (region_reserve(asid,
	 _init->kcode,
	 LINKER_SYMBOL(_handler_data_begin) - _init->kcode,
	 REGION_OPTION_FORCE | REGION_OPTION_PRIVILEGED |
	 REGION_OPTION_GLOBAL,
	 LINKER_SYMBOL(_handler_data_begin),
	 LINKER_SYMBOL(_handler_data_end) -
	 LINKER_SYMBOL(_handler_data_begin),
	 &reg) != ERROR_OK)
	 MACHINE_ESCAPE("XXX");
      */

      i_region	region;

      if (segment_locate(_init->kcode, &seg) == ERROR_FALSE)
	MACHINE_ESCAPE("XXX");

      // XXX identity mapping for the kernel code.
      if (region_reserve(asid,
			 seg,
			 0,
			 REGION_OPTION_FORCE | REGION_OPTION_GLOBAL,
			 // XXX ??? REGION_OPTION_PRIVILEGED,
			 (t_vaddr)_init->kcode,
			 (t_vsize)_init->kcodesz,
			 &region) != ERROR_OK)
	MACHINE_ESCAPE("XXX");

      if (segment_locate(_init->kstack, &seg) == ERROR_FALSE)
	MACHINE_ESCAPE("XXX");

      // XXX identity mapping for the kernel stack.
      if (region_reserve(asid,
			 seg,
			 0,
			 REGION_OPTION_FORCE | REGION_OPTION_GLOBAL,
			 // XXX ??? REGION_OPTION_PRIVILEGED,
			 (t_vaddr)_init->kstack,
			 (t_vsize)_init->kstacksz,
			 &region) != ERROR_OK)
	MACHINE_ESCAPE("XXX");
    }

  MACHINE_LEAVE();
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
    MACHINE_ESCAPE("XXX");

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_kernel;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_KERNEL_DS,
			   seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

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
    MACHINE_ESCAPE("XXX");

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_driver;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_DRIVER_DS,
			   seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

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
    MACHINE_ESCAPE("XXX");

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_service;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_SERVICE_DS,
			   seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

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
    MACHINE_ESCAPE("XXX");

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_privilege_guest;
  seg.is_system = 0;
  seg.type.user = ia32_type_data;
  if (ia32_gdt_add_segment(IA32_GDT_CURRENT, IA32_PMODE_GDT_GUEST_DS,
			   seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

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

  MACHINE_LEAVE();
}
