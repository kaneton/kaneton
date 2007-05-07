/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [sun may  6 18:07:25 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for as  manager on  ia32 with
 * paging architecture.
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

extern m_as*		as;
extern i_task		ktask;
extern t_init*		init;
extern i_as		kasid;
extern t_ia32_idt           idt;
extern m_thread*	thread;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager dispatch.
 */

d_as			as_dispatch =
  {
    ia32_as_show,
    ia32_as_give,
    NULL,
    NULL,
    NULL,
    ia32_as_reserve,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- externs ---------------------------------------------------------
 */

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
 * this function give an address space to a task.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) get the destination task.
 * 3) for each thread, update the context's PDBR value.
 */

t_error			ia32_as_give(i_task			tskid,
				     i_as			asid)
{
  o_as*			o;
  o_task*		otask;
  o_thread*		oth;
  t_iterator		it;
  t_state		state;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(tskid, &otask) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, otask->threads, &it, state)
    {
      i_thread*		th;

      if (set_object(otask->threads, it, (void**)&th) != ERROR_NONE)
	{
	  cons_msg('!', "as: cannot find the object "
		   "corresponding to its identifier\n");

	  AS_LEAVE(as, ERROR_UNKNOWN);
	}

      if (thread_get(*th, &oth) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (pd_get_cr3(&oth->machdep.context.cr3, o->machdep.pd,
		     PD_CACHED, PD_WRITEBACK) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function displays architecture dependent data.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) display machdep data.
 */

t_error			ia32_as_show(i_as			asid)
{
  o_as*			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  page directory base: 0x%x\n", o->machdep.pd);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) get the as object.
 *   kernel task:
 *  a) get the page directory from the init structure.
 *  b) add the mirroring entry.
 *  c) manually reserve a region for the mirror entry.
 *  d) clean the page directory.
 *  e) reinject the page tables in the kernel as.
 *  f) invalidate MMU caches.
 *  g) setup the PDBR to use on interrupts.
 *   normal task:
 *  a) reserve a segment for the directory.
 *  b) reserve a region for the directory in the kernel address space.
 *  c) build a new page directory for the as.
 *  d) release the kernel-side region mapping the directory.
 *  e) map the mandatory regions (GDT, IDT, kernel stack & handlers, tss)
 *  f) set the new value of PDBR to all threads.
 */

t_error			ia32_as_reserve(i_task			tskid,
					i_as*			asid)
{
  o_as*			o;
  i_segment	        seg;
  i_region		reg;
  o_segment*		oseg;
  o_segment*		pt_seg;
  o_region		oreg;
  o_region*		preg;
  t_paddr		base;
  t_ia32_table		pt;
  t_uint32		i;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;
  o_task*		otask;
  o_thread*		oth;
  t_iterator		it;
  t_state		state;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (tskid == ktask)
    {
      kasid = *asid;

      /*
       * a)
       */

      memcpy(&o->machdep.pd, &init->machdep.pd, sizeof(t_ia32_directory));

      if (pd_activate(o->machdep.pd, PD_CACHED, PD_WRITEBACK) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * b)
       */

      pt.present = 1;
      pt.rw = PT_WRITABLE;
      pt.user = PT_PRIVILEGED;
      pt.cached = PT_NOTCACHED;
      pt.writeback = PT_WRITEBACK;
      pt.entries = (t_paddr)o->machdep.pd;

      if (pd_add_table(&o->machdep.pd, PD_MIRROR, pt) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      if ((preg = malloc(sizeof(o_region))) == NULL)
	AS_LEAVE(as, ERROR_UNKNOWN);

      preg->address = ENTRY_ADDR(PD_MIRROR, 0);
      preg->segid = (i_segment)pt.entries;
      preg->offset = 0;
      preg->size = PT_MAX_ENTRIES * PAGESZ;
      preg->opts = REGION_OPT_NONE;

      if (region_inject(*asid, preg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * d)
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
       * e)
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
			AS_LEAVE(as, ERROR_UNKNOWN);

		      pt_seg->address = (t_paddr)pt.entries;
		      pt_seg->size = PAGESZ;
		      pt_seg->perms = PERM_READ | PERM_WRITE;

		      if (segment_inject(*asid, pt_seg, &seg) != ERROR_NONE)
			AS_LEAVE(as, ERROR_UNKNOWN);
		    }
		}
	    }
	}

      /*
       * f)
       */

      tlb_flush();

      /*
       * g)
       */

      if (pd_get_cr3((t_ia32_directory*)&interrupt_pdbr, o->machdep.pd,
		     PD_CACHED, PD_WRITEBACK) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }
  else
    {
      /*
       * a)
       */

      if (segment_reserve(*asid, PAGESZ,
			  PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * b)
       */

      if (region_reserve(kasid, seg, 0, REGION_OPT_PRIVILEGED,
			 0, PAGESZ, &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      base = reg;

      if (pd_build(base, &o->machdep.pd, 1) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      o->machdep.pd = (t_ia32_directory)(t_uint32)seg;

      /*
       * d)
       */

      if (region_release(kasid, reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * e)
       */

      // A VIRER ---------------

      // on map aussi de quoi faire des printf. A VIRER
      if (region_reserve(*asid,
			 (i_segment)0x1000,
			 0,
			 REGION_OPT_FORCE | REGION_OPT_GLOBAL,
			 0x1000,
			 INIT_ISA_SIZE - 0x1000,
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
#if 0
      if (region_reserve(*asid,
			 (i_segment)init->kcode,
			 0,
			 REGION_OPT_FORCE | REGION_OPT_GLOBAL,
			 init->kcode,
			 init->kcodesz,
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
#endif
      // -----------------------

      if (region_reserve(*asid,
			 (i_segment)init->kcode,
			 LINKER_SYMBOL(_handler_begin) - init->kcode,
			 REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
			 REGION_OPT_GLOBAL,
			 LINKER_SYMBOL(_handler_begin),
			 LINKER_SYMBOL(_handler_end) -
			 LINKER_SYMBOL(_handler_begin),
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (region_reserve(*asid,
			 (i_segment)init->kcode,
			 LINKER_SYMBOL(_handler_data_begin) - init->kcode,
			 REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
			 REGION_OPT_GLOBAL,
			 LINKER_SYMBOL(_handler_data_begin),
			 LINKER_SYMBOL(_handler_data_end) -
			 LINKER_SYMBOL(_handler_data_begin),
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (region_get(kasid, (i_region)(t_uint32)thread->machdep.tss,
		     &preg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (region_reserve(*asid,
			 preg->segid,
			 0,
			 REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
			 REGION_OPT_GLOBAL,
			 (t_vaddr)thread->machdep.tss,
			 preg->size,
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (region_reserve(*asid,
			 (i_segment)(t_uint32)init->machdep.gdt.descriptor,
			 0,
			 REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
			 REGION_OPT_GLOBAL,
			 (t_vaddr)init->machdep.gdt.descriptor,
			 PAGESZ,
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (region_reserve(*asid,
			 (i_segment)(t_uint32)idt.descriptor,
			 0,
			 REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
			 REGION_OPT_GLOBAL,
			 (t_vaddr)idt.descriptor,
			 PAGESZ,
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      base = thread->machdep.tss->esp0 - 2 * PAGESZ + 16;

      if (region_get(kasid, (i_region)base,
		     &preg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (region_reserve(*asid,
			 preg->segid,
			 0,
			 REGION_OPT_FORCE | REGION_OPT_PRIVILEGED |
			 REGION_OPT_GLOBAL,
			 base,
			 2 * PAGESZ,
			 &reg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * f)
       */

      if (task_get(tskid, &otask) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, otask->threads, &it, state)
	{
	  i_thread*		th;

	  if (set_object(otask->threads, it, (void**)&th) != ERROR_NONE)
	    {
	      cons_msg('!', "as: cannot find the object "
		       "corresponding to its identifier\n");

	      AS_LEAVE(as, ERROR_UNKNOWN);
	    }

	  if (thread_get(*th, &oth) != ERROR_NONE)
	      AS_LEAVE(as, ERROR_UNKNOWN);

	  if (pd_get_cr3(&oth->machdep.context.cr3, o->machdep.pd,
			 PD_CACHED, PD_WRITEBACK) != ERROR_NONE)
	    AS_LEAVE(as, ERROR_UNKNOWN);
	}
    }

  AS_LEAVE(as, ERROR_NONE);
}
