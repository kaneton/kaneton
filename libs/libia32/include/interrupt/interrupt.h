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

#ifndef IA32_INTERRUPT_INTERRUPT_H
#define IA32_INTERRUPT_INTERRUPT_H       1

/*                                                                  [cut] k3 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * misc
 */

#define EXCEPTION_NR	32
#define IRQ_NR		16

/*
 * save cpu register on the stack
 */

#define SAVE_REG							\
  "pushl %eax\n\t"							\
  "pushl %ebx\n\t"							\
  "pushl %ecx\n\t"							\
  "pushl %edx\n\t"							\
  "pushl %edi\n\t"							\
  "pushl %esi\n\t"							\
  "pushl %ds\n\t"							\
  "pushl %es\n\t"							\
  "pushl %fs\n\t"

/*
 * restore cpu register from the stack
 */

#define RESTORE_REG							\
  "popl %fs\n\t"							\
  "popl %es\n\t"							\
  "popl %ds\n\t"							\
  "popl %esi\n\t"							\
  "popl %edi\n\t"							\
  "popl %edx\n\t"							\
  "popl %ecx\n\t"							\
  "popl %ebx\n\t"							\
  "popl %eax\n\t"

/*
 * load segment registers
 */

#define LOAD_SEG_REG							\
  "movl $0x10,%edx\n\t"							\
  "mov %dx,%ds\n\t"							\
  "mov %dx,%es\n\t"							\
  "mov %dx,%fs\n\t"

/*
 * simulate a return from function
 */

#define ADJUST_STACK							\
  "movl %ebp,%esp\n\t"							\
  "popl %ebp\n\t"

/*
 * pre-handler for exceptions
 */

#define EXCEPTION_PREHANDLER(_nr_)					\
  void	prehandler_exception##_nr_(void)				\
    {									\
      asm volatile(SAVE_REG						\
		   LOAD_SEG_REG);					\
      interrupt_wrapper(IDT_EXCEPTION_BASE + _nr_);			\
      asm volatile(RESTORE_REG						\
		   ADJUST_STACK						\
		   "iret\n\t");						\
    }

/*
 * pre-handler for irq's
 */

#define IRQ_PREHANDLER(_nr_)						\
  void	prehandler_irq##_nr_(void)					\
    {									\
      asm volatile(SAVE_REG						\
		   LOAD_SEG_REG);					\
      interrupt_wrapper(IDT_IRQ_BASE + _nr_);				\
      asm volatile(RESTORE_REG						\
		   ADJUST_STACK						\
		   "iret\n\t");						\
    }

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_interrupt_pre_hdl)(void);
typedef void			(*t_interrupt_hdl)(t_uint32);

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * ../interrupt/exception.c
 */

void	prehandler_exception0(void);
void	prehandler_exception1(void);
void	prehandler_exception2(void);
void	prehandler_exception3(void);
void	prehandler_exception4(void);
void	prehandler_exception5(void);
void	prehandler_exception6(void);
void	prehandler_exception7(void);
void	prehandler_exception8(void);
void	prehandler_exception9(void);
void	prehandler_exception10(void);
void	prehandler_exception11(void);
void	prehandler_exception12(void);
void	prehandler_exception13(void);
void	prehandler_exception14(void);
void	prehandler_exception15(void);
void	prehandler_exception16(void);
void	prehandler_exception17(void);
void	prehandler_exception18(void);
void	prehandler_exception19(void);
void	prehandler_exception20(void);
void	prehandler_exception21(void);
void	prehandler_exception22(void);
void	prehandler_exception23(void);
void	prehandler_exception24(void);
void	prehandler_exception25(void);
void	prehandler_exception26(void);
void	prehandler_exception27(void);
void	prehandler_exception28(void);
void	prehandler_exception29(void);
void	prehandler_exception30(void);
void	prehandler_exception31(void);

/*
 * ../interrupt/irq.c
 */

void    prehandler_irq0(void);
void    prehandler_irq1(void);
void    prehandler_irq2(void);
void    prehandler_irq3(void);
void    prehandler_irq4(void);
void    prehandler_irq5(void);
void    prehandler_irq6(void);
void    prehandler_irq7(void);
void    prehandler_irq8(void);
void    prehandler_irq9(void);
void    prehandler_irq10(void);
void    prehandler_irq11(void);
void    prehandler_irq12(void);
void    prehandler_irq13(void);
void    prehandler_irq14(void);
void    prehandler_irq15(void);

/*
 * eop
 */

/*                                                                  [cut] /k3 */

#endif
