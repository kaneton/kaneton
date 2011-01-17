/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/educational/tss.c
 *
 * created       renaud voltz   [mon apr 10 01:01:34 2006]
 * updated       julien quintard   [mon jan 17 13:31:11 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for managing the TSS - Task State Segment.
 *
 * in order to understand why the ia32 architecture makes use of TSSs,
 * one must first notice that event handlers---being for exceptions, IRQs,
 * IPIs or syscalls---are always executed in kernel land.
 *
 * therefore, whenever an event occurs, for instance while a server is
 * running, the ia32 architecture automatically performs a task switch,
 * hence replacing the current CPL3 stack with the thread's CPL0 stack,
 * commonly referred to as the thread's 'pile'.
 *
 * in order to perform this automatic stack switching, the CPU must know
 * the location of the thread's CPL0 stack i.e pile among other things.
 *
 * a TSS is therefore used by the system in order to store the currently
 * executing thread's informations, including the location of its pile.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function updates the given TSS according to the given parameters.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) fill the TSS' attributes.
 */

t_error			architecture_tss_update(as_tss*		tss,
						t_uint16	ss0,
						t_uint32	esp0,
						t_uint32	io)
{
  /*
   * 0)
   */

  if (tss == NULL)
    MACHINE_ESCAPE("the 'tss' argument is null");

  /*
   * 1)
   */

  tss->ss0 = ss0;
  tss->esp0 = esp0;
  tss->io = io;
  tss->io_end = 0xFF;

  MACHINE_LEAVE();
}

/*
 * this function makes the given TSS the system's current one.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) add the TSS to the GDT. note that the GDT system entry for the TSS
 *    is supposed to store a linear address but since the GDT function
 *    only accepts physical addresses, a cast is performed.
 * 2) build the TSS segment selector.
 * 3) load the selector in the TR register.
 */

t_error			architecture_tss_activate(as_tss*	tss)
{
  t_uint16		index;
  t_uint16		selector;

  /*
   * 0)
   */

  if (tss == NULL)
    MACHINE_ESCAPE("the 'tss' argument is null");

  /*
   * 1)
   */

  if (architecture_gdt_reserve((t_paddr)tss,
			       sizeof(as_tss),
			       ARCHITECTURE_GDTE_DPL_SET(
			         ARCHITECTURE_PRIVILEGE_RING0) |
			       ARCHITECTURE_GDTE_TSS,
			       &index) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a GDT entry");

  /*
   * 2)
   */

  if (architecture_gdt_selector(index,
				ARCHITECTURE_PRIVILEGE_RING0,
				&selector) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the TSS segment selector");

  /*
   * 3)
   */

  ARCHITECTURE_LTR(selector);

  MACHINE_LEAVE();
}
