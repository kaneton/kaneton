/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/include/interrupt/interrupt.h
 *
 * created       renaud voltz   [fri feb 17 16:48:22 2006]
 * updated       renaud voltz   [fri feb 17 16:48:22 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 *
 */

#ifndef IA32_IA32_INTERRUPT_H
#define IA32_IA32_INTERRUPT_H       1

/*                                                                  [cut] k2 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 *
 */

#define SAVE_REG							\
  "pushl %eax\n"							\
  "pushl %ebx\n"							\
  "pushl %ecx\n"							\
  "pushl %edx\n"							\
  "pushl %edi\n"							\
  "pushl %esi\n"							\
  "pushl %ebp\n"							\
  "push %ds\n"								\
  "push %es\n"								\
  "push %fs\n"

/*
 *
 */

#define RESTORE_REG							\
  "pop %fs\n"								\
  "pop %es\n"								\
  "pop %ds\n"								\
  "popl %ebp\n"								\
  "popl %esi\n"								\
  "popl %edi\n"								\
  "popl %edx\n"								\
  "popl %ecx\n"								\
  "popl %ebx\n"								\
  "popl %eax\n"

/*
 *
 */

#define LOAD_SEG_REG							\
  "movl $0x10,%edx\n"							\
  "mov %dx,%ds\n"							\
  "mov %dx,%es\n"							\
  "mov %dx,%fs\n"

#define EXCEPTION_HANDLER(_nr_)						\
  handler_exception#_nr_

#define IRQ_HANDLER(_nr_)						\
  handler_irq_##_nr_


#define EXCEPTION_ENTER(_nr_)						\
  asm volatile(SAVE_REG							\
	       LOAD_SEG_REG);

#define EXCEPTION_LEAVE()						\
  asm volatile(RESTORE_REG);


#define IRQ_ENTER(_nr_)							\
  asm volatile(SAVE_REG							\
	       LOAD_SEG_REG);

#define IRQ_LEAVE()							\
  asm volatile(RESTORE_REG);


/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_interrupt_handler)(void);

typedef t_interrupt_handler	t_exception_handler;

typedef t_interrupt_handler	t_irq_handler;

#endif
