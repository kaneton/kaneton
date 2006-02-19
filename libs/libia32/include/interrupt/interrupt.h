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
 * save cpu register on the stack
 */

#define SAVE_REG							\
  asm volatile("pushl %eax\n\t"						\
	       "pushl %ebx\n\t"						\
	       "pushl %ecx\n\t"						\
	       "pushl %edx\n\t"						\
	       "pushl %edi\n\t"						\
	       "pushl %esi\n\t"						\
	       "pushl %ebp\n\t"						\
	       "pushl %ds\n\t"						\
	       "pushl %es\n\t"						\
	       "pushl %fs\n\t");

/*
 * restore cpu register from the stack
 */

#define RESTORE_REG							\
  asm volatile("popl %fs\n\t"						\
	       "popl %es\n\t"						\
	       "popl %ds\n\t"						\
	       "popl %ebp\n\t"						\
	       "popl %esi\n\t"						\
	       "popl %edi\n\t"						\
	       "popl %edx\n\t"						\
	       "popl %ecx\n\t"						\
	       "popl %ebx\n\t"						\
	       "popl %eax\n\t");

/*
 * load segment registers
 */

#define LOAD_SEG_REG							\
  "movl $0x10,%edx\n"							\
  "mov %dx,%ds\n"							\
  "mov %dx,%es\n"							\
  "mov %dx,%fs\n"

/*
 * pre-handler for exceptions
 */

#define EXCEPTION_PREHANDLER(_nr_)					\
  void	handler_exception##_nr_(void)					\
    {									\
      SAVE_REG								\
      exception_wrapper(_nr_);						\
      RESTORE_REG      							\
    }

/*
 * pre-handler for irq's
 */

#define IRQ_PREHANDLER(_nr_)						\
  void	handler_irq##_nr_(void)						\
    {									\
      SAVE_REG								\
      irq_wrapper(_nr_);						\
      RESTORE_REG							\
    }

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_interrupt_handler)(void);

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * ../interrupt/exception.c
 */

void	handler_exception0(void);
void	handler_exception1(void);
void	handler_exception2(void);
void	handler_exception3(void);
void	handler_exception4(void);
void	handler_exception5(void);
void	handler_exception6(void);
void	handler_exception7(void);
void	handler_exception8(void);
void	handler_exception9(void);
void	handler_exception10(void);
void	handler_exception11(void);
void	handler_exception12(void);
void	handler_exception13(void);
void	handler_exception14(void);
void	handler_exception15(void);
void	handler_exception16(void);
void	handler_exception17(void);
void	handler_exception18(void);
void	handler_exception19(void);
void	handler_exception20(void);
void	handler_exception21(void);
void	handler_exception22(void);
void	handler_exception23(void);
void	handler_exception24(void);
void	handler_exception25(void);
void	handler_exception26(void);
void	handler_exception27(void);
void	handler_exception28(void);
void	handler_exception29(void);
void	handler_exception30(void);
void	handler_exception31(void);

/*
 * ../interrupt/irq.c
 */

void    handler_irq0(void);
void    handler_irq1(void);
void    handler_irq2(void);
void    handler_irq3(void);
void    handler_irq4(void);
void    handler_irq5(void);
void    handler_irq6(void);
void    handler_irq7(void);
void    handler_irq8(void);
void    handler_irq9(void);
void    handler_irq10(void);
void    handler_irq11(void);
void    handler_irq12(void);
void    handler_irq13(void);
void    handler_irq14(void);
void    handler_irq15(void);

/*
 * eop
 */

/*                                                                  [cut] /k3 */

#endif
