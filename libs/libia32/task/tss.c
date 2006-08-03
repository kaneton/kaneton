/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/tss.c
 *
 * created       renaud voltz   [mon apr 10 01:01:34 2006]
 * updated       matthieu bucchianeri   [mon jul 31 19:56:13 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * an event handler is always executed in kernel land.
 * also when an event occurs in user land, the ia32 architecture automaticaly
 * performs a task switch, replacing the cpl3-stack by the cpl0-stack of the
 * thread. information needed to perform this task switch are contained into
 * the current TSS whose descriptor is pointed by the TR register.
 * thus, whatever the design chosen for the context switch implementation, ia32
 * architecture always needs at least one TSS.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * update the current tss.
 */

t_error			tss_load(t_ia32_tss*			tss,
				 t_uint16			ss,
				 t_uint32			esp,
				 t_uint32			io)
{
  tss->ss = ss;
  tss->esp = esp;
  tss->io = io;

  return ERROR_NONE;
}

/*
 * init the tss with the kernel thread.
 *
 * steps:
 *
 * 1)fill the tss descriptor and add it into the gdt.
 * 2)get the segment selector for this descriptor.
 * 3)load the task register with this segment selector.
 */

t_error			tss_init(t_ia32_tss*			tss)
{
  t_uint16		segment;
  t_uint16		selector;
  t_ia32_segment	descriptor;

  /*
   * 1)
   */

  descriptor.base = (t_uint32)tss;
  descriptor.limit = sizeof(t_ia32_tss);
  descriptor.privilege = PRIV_RING0;
  descriptor.is_system = 1;
  descriptor.type.sys = SEG_TYPE_TSS;

  if (gdt_reserve_segment(NULL, descriptor, &segment) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (gdt_build_selector(segment, descriptor.privilege, &selector)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  LTR(selector);

  interrupt_stack = tss->esp0;

  return ERROR_NONE;
}
