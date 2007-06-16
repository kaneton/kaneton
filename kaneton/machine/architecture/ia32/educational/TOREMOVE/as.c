/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...hitecture/ia32/educational/TOREMOVE/as.c
 *
 * created       julien quintard   [sun jun 10 21:18:54 2007]
 * updated       matthieu bucchianeri   [sat jun 16 18:04:13 2007]
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

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_as*		as;
extern i_task		ktask;
extern t_init*		init;
extern i_as		kasid;
extern t_ia32_idt	idt;
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
  i_region		useless;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (tskid == ktask)
    {
      /// DONE
    }
  else
    {
      // XXX
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
