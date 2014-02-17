/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/tss.c
 *
 * created       renaud voltz   [mon apr 10 01:01:34 2006]
 * updated       matthieu bucchianeri   [sun sep 10 12:57:09 2006]
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

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * update the given tss.
 */

t_status		ia32_tss_load(t_ia32_tss*		tss,
				      t_uint16			ss,
				      t_uint32			esp,
				      t_uint32			io)
{
  assert(tss != NULL);

  tss->ss0 = ss;
  tss->esp0 = esp;
  tss->io = io;
  tss->io_end = 0xFF;

  return STATUS_OK;
}

/*
 * set the given tss as current.
 *
 * steps:
 *
 * 1) fill the tss descriptor and add it into the gdt.
 * 2) get the segment selector for this descriptor.
 * 3) load the task register with this segment selector.
 */

t_status		ia32_tss_init(t_ia32_tss*			tss)
{
  t_uint16		segment;
  t_uint16		selector;
  t_ia32_segment	descriptor;

  assert(tss != NULL);

  /*
   * 1)
   */

  descriptor.base = (t_uint32)tss;
  descriptor.limit = sizeof (t_ia32_tss);
  descriptor.privilege = IA32_PRIV_RING0;
  descriptor.is_system = 1;
  descriptor.type.sys = IA32_SEG_TYPE_TSS;

  if (ia32_gdt_reserve_segment(IA32_GDT_CURRENT, descriptor,
			       &segment) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (ia32_gdt_build_selector(segment, descriptor.privilege, &selector)
      != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  LTR(selector);

  return STATUS_OK;
}
