/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/task/tss.c
 *
 * created       renaud voltz   [mon apr 10 01:01:34 2006]
 * updated       renaud voltz   [mon apr 10 01:01:34 2006]
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
 * ---------- assignments -----------------------------------------------------
 *
 * XXX THREAD assigments have to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

t_ia32_tss		tss;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX EVENT
 *
 * stes:
 *
 * 1) retrieve the global tss pointer if needed.
 * 2) update tss fields.
 */

/* XXX EVENT rename tss_new */

t_error			tss_load(t_ia32_tss*			tss_new,
				 t_uint16			ss,
				 t_uint32			esp,
				 t_uint32			io)
{
  /*
   * 1)
   */

  if (!tss_new)
    tss_new = &tss;

  /*
   * 2)
   */

  tss_new->ss = ss;
  tss_new->esp = esp;
  tss_new->io = io;

  return ERROR_NONE;
}

/*
 * init the tss with the kernel thread.
 *
 * steps:
 *
 * 1)retrieve the global tss pointer if needed.
 * 2)fill the tss descriptor and add it into the gdt.
 * 3)get the segment selector for this descriptor.
 * 4)load the task register with this segment selector.
 */

t_error			tss_init(t_ia32_tss*			base)
{
  t_uint16		segment;
  t_uint16		selector;
  t_ia32_segment	descriptor;

  /*
   * 1)
   */

  if (!base)
    base = &tss;

  /*
   * 2)
   */

  descriptor.base = (t_uint32)base;
  descriptor.limit = sizeof(t_ia32_tss);
  descriptor.privilege = 0;
  descriptor.is_system = 1;
  descriptor.type.sys = SEG_TYPE_TSS;

  if (gdt_reserve_segment(NULL, descriptor, &segment) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if (gdt_build_selector(segment, descriptor.privilege, &selector)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 4)
   */

  LTR(selector);

  return ERROR_NONE;
}
