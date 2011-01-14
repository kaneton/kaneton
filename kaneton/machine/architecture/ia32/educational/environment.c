/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/environment.c
 *
 * created       julien quintard   [thu jan 13 23:13:50 2011]
 * updated       julien quintard   [fri jan 14 13:40:04 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 *
 * [XXX:improvement] in the server initialization, needless to map the kernel
 *                   code and stack. instead the handler shells should be
 *                   mapped i.e the .handler section.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- externs ---------------------------------------------------------
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
 * the system's GDT.
 */

extern as_gdt_descriptor	_architecture_gdt; // XXX

/*
 * the system's IDT.
 */

extern as_idt_descriptor	_architecture_idt; // XXX

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initializes the kernel's environment.
 *
 * steps:
 *
 * XXX
 */

t_error			architecture_environment_kernel(i_as	id)
{
  struct
  {
    at_pdei		start;
    at_pdei		end;
    at_pdei		index;
  }			pde;
  struct
  {
    at_ptei		start;
    at_ptei		end;
    at_ptei		index;
  }			pte;
  i_region		useless;
  o_as*			as;
  at_pd			pd;
  at_pt			pt;
  o_region*		r;
  t_uint32		i;

  /*
   * 1)
   */

  if (as_get(id, &as) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  as->machine.pd = _init->machine.pd;

  /*
   * 3)
   */

  pd = (at_pd)as->machine.pd; // XXX identity mapping to come!

  /*
   * 4)
   */

  if (architecture_paging_cr3(pd,
			      ARCHITECTURE_REGISTER_CR3_PCE |
			      ARCHITECTURE_REGISTER_CR3_PWB,
			      &ia32_interrupt_pdbr) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the CR3 register's content");

  /*
   * 5)
   */

  if (architecture_paging_import(pd,
				 ia32_interrupt_pdbr) != ERROR_OK)
    MACHINE_ESCAPE("unable to import the kernel page directory");

  /*
   * 6)
   */

  // XXX mirroring entry
  if (architecture_pd_insert(pd,
			     ARCHITECTURE_PD_MIRROR,
			     as->machine.pd,
			     ARCHITECTURE_PDE_PRESENT |
			     ARCHITECTURE_PDE_RW |
			     ARCHITECTURE_PDE_SUPERVISOR |
			     ARCHITECTURE_PDE_PWB |
			     ARCHITECTURE_PDE_PCE) != ERROR_OK)
    MACHINE_ESCAPE("unable to insert the mirroring directory entry");

  /*
   * 7)
   */

  // XXX a noter que l'on inject pas de segment ni de region pour le
  // page directory car ils sont dans init segment/regions et seront
  // injectes dans task_initialize()

  if ((r = malloc(sizeof(o_region))) == NULL)
    MACHINE_ESCAPE("unable to allocate memory for the region object");

  r->address = ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR, 0);
  r->segment = (i_segment)as->machine.pd; // XXX !!!WARNING!!! ici on devrait
                                       // pouvoir recupere le vrai segment
                                       // pour extraire l'id ou le generer
                                       // avec SEGMENT_IDENTIFIER() au pire!
  r->offset = 0x0;
  // XXX on reserve toutes les entrees du mirroring i.e pour acceder a
  // toutes les page tables kernel
  r->size = ARCHITECTURE_PT_SIZE * ___kaneton$pagesz;
  r->options = REGION_OPTION_NONE;

  /*
   * 8)
   */

  if (region_inject(as->id, r, &useless) != ERROR_OK)
    MACHINE_ESCAPE("unable to inject the mirroring region");

  /*
   * 9)
   */

  pde.start = 0;
  pte.start = 0;

  for (i = 0; i <= _init->nregions; i++)
    {
      /*
       * a)
       */

      if (i != _init->nregions)
	{
	  pde.end = ARCHITECTURE_PD_INDEX(_init->regions[i].address);
	  pte.end = ARCHITECTURE_PT_INDEX(_init->regions[i].address);
	}
      else
	{
	  pde.end = ARCHITECTURE_PD_SIZE - 1;
	  pte.end = ARCHITECTURE_PT_SIZE;
	}

      /*
       * b)
       */

      for (pde.index = pde.start;
	   pde.index <= pde.end; // XXX <
	   pde.index++)
	{
	  /*
	   * i)
	   */

	  if ((pde.index != ARCHITECTURE_PD_MIRROR) && // XXX why puisque pde.end a ete calcule pour l'eviter?
	      (pd[pde.index] & ARCHITECTURE_PDE_USED))
	    {
	      /*
	       * #1)
	       */

	      pt = (at_pt)ARCHITECTURE_PDE_ADDRESS(pd[pde.index]); // XXX identity mapping

	      /*
	       * #2)
	       */

	      for (pte.index = (pde.index == pde.start ? pte.start : 0);
		   pte.index < (pde.index == pde.end ?
				pte.end : ARCHITECTURE_PT_SIZE);
		   pte.index++)
		{
		  /*
		   * #a)
		   */

		  if (pt[pte.index] & ARCHITECTURE_PTE_USED)
		    {
		      if (architecture_pt_delete(pt, pte.index) != ERROR_OK)
			MACHINE_ESCAPE("unable to delete the page "
				       "table entry");
		    }
		}
	    }
	}

      /*
       * c)
       */

      if (i != _init->nregions) // est-ce necessaire puisqu'ensuite on quitte la boucle?
	{
	  pde.start = ARCHITECTURE_PD_INDEX(_init->regions[i].address +
					    _init->regions[i].size);
	  pte.start = ARCHITECTURE_PT_INDEX(_init->regions[i].address +
					    _init->regions[i].size);
	}
    }

  /*
   * 10)
   */

  for (i = 0; i < _init->nregions; i++)
    {
      /*
       * a)
       */

      pde.start = ARCHITECTURE_PD_INDEX(_init->regions[i].address);
      pde.end = ARCHITECTURE_PD_INDEX(_init->regions[i].address +
				      _init->regions[i].size);

      /*
       * b)
       */

      for (pde.index = pde.start;
	   pde.index <= pde.end;
	   pde.index++) // XXX <
	{
	  /*
	   * i)
	   */

	  if (pd[pde.index] & ARCHITECTURE_PDE_USED)
	    {
	      i_segment		segment;
	      o_segment*	s;

	      /*
	       * #1)
	       */

	      if (segment_locate(ARCHITECTURE_PDE_ADDRESS(pd[pde.index]),
				 &segment) == ERROR_FALSE)
		{
		  /*
		   * #a)
		   */

		  if ((s = malloc(sizeof(o_segment))) == NULL)
		    MACHINE_ESCAPE("unable to allocate memory");

		  s->type = SEGMENT_TYPE_SYSTEM;
		  s->address = ARCHITECTURE_PDE_ADDRESS(pd[pde.index]);
		  s->size = ___kaneton$pagesz;
		  s->permissions = PERMISSION_READ | PERMISSION_WRITE;

		  /*
		   * #b)
		   */

		  if (segment_inject(as->id, s, &segment) != ERROR_OK)
		    MACHINE_ESCAPE("unable to inject the segment object");
		}
	    }
	}
    }

  /*
   * 11)
   */

  if (architecture_tlb_flush() != ERROR_OK)
    MACHINE_ESCAPE("unable to flush the TLB");

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			architecture_environment_server(i_as	id)
{
  i_segment	        segment;
  i_region		region;
  o_as*			as;
  at_pd			pd;
  o_region*		r;
  o_segment*		s;

  /*
   * 1)
   */

  if (as_get(id, &as) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (segment_reserve(as->id,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &segment) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a segment");

  /*
   * 3)
   */

  if (segment_type(segment, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("unable to change the segment's type");

  /*
   * 4)
   */

  if (segment_get(segment, &s) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the segment object");

  /*
   * 5)
   */

  as->machine.pd = s->address;

  /*
   * 6)
   */

  if (architecture_pd_map(as->machine.pd, &pd) != ERROR_OK)
    MACHINE_ESCAPE("unable to map the page directory");

  /*
   * 7)
   */

  if (architecture_pd_build(pd) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the page directory");

  /*
   * 8)
   */

  if (architecture_pd_unmap(pd) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the page directory");

  /*
   * 9)
   */

  if (region_locate(_kernel->as,
		    _thread->machine.tss,
		    &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region in which the TSS lies");

  /*
   * 10)
   */

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 11)
   */

  // XXX force the mapping so that the TSS gets mapped at the same
  // location in every as.
  if (region_reserve(as->id,
		     r->segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     _thread->machine.tss,
		     r->size,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the TSS");

  /*
   * 12)
   */

  if (region_locate(_kernel->as,
		    (t_vaddr)_architecture_gdt.table,
		    &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region in which the GDT lies");

  /*
   * 13)
   */

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 14)
   */

  // XXX meme chose on force
  if (region_reserve(as->id,
		     r->segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)_architecture_gdt.table,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the GDT");

  /*
   * 15)
   */

  if (region_locate(_kernel->as,
		    (t_vaddr)_architecture_idt.table,
		    &region) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the region in which the IDT lies");

  /*
   * 16)
   */

  if (region_get(_kernel->as, region, &r) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 17)
   */

  // XXX meme chose on force.
  if (region_reserve(as->id,
		     r->segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_PRIVILEGED |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)_architecture_idt.table,
		     ___kaneton$pagesz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the IDT");

  /*
   * 18)
   */

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

  if (segment_locate(_init->kcode, &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the segment which contains the "
		   "kernel code");

  /*
   * 19)
   */

  // XXX identity mapping for the kernel code.
  if (region_reserve(as->id,
		     segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_GLOBAL,
		     (t_vaddr)_init->kcode,
		     (t_vsize)_init->kcodesz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the kernel code");

  /*
   * 20)
   */

  if (segment_locate(_init->kstack,
		     &segment) == ERROR_FALSE)
    MACHINE_ESCAPE("unable to locate the segment which contains the "
		   "kernel stack");

  /*
   * 21)
   */

  // XXX identity mapping for the kernel stack.
  if (region_reserve(as->id,
		     segment,
		     0x0,
		     REGION_OPTION_FORCE |
		     REGION_OPTION_GLOBAL |
		     REGION_OPTION_PRIVILEGED, // XXX ?
		     (t_vaddr)_init->kstack,
		     (t_vsize)_init->kstacksz,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the region mapping the kernel stack");

  MACHINE_LEAVE();
}
