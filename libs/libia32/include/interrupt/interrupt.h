/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/interrupt/interrupt.h
 *
 * created       renaud voltz   [fri feb 17 16:48:22 2006]
 * updated       matthieu bucchianeri   [wed jul 26 15:46:27 2006]
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
 * ---------- macros ----------------------------------------------------------
 */

/*
 * interrupts number.
 */

#define EXCEPTION_NR	32
#define IRQ_NR		16

/*
 * exception error code.
 */

#define ERROR_CODE	1
#define NO_ERROR_CODE	0

/*
 * specific  to handlers.  this is  used to  place handler  code  in a
 * special ELF section.
 */

#define HANDLER								\
  __attribute__ ((section("handler")))

/*
 * load segment registers
 */

#define LOAD_CORE_SELECTORS(_ds_)					\
  asm volatile("movl $"#_ds_",%edx\n\t"					\
	       "movw %dx,%ds\n\t"					\
	       "movw %dx,%es\n\t"					\
	       "movw %dx,%fs\n\t"					\
	       "movw %dx,%gs\n\t")

/*
 * pre-handler for exceptions
 */

#define EXCEPTION_PREHANDLER(_nr_, _error_code_)	       		\
  HANDLER void	prehandler_exception##_nr_(void)			\
    {									\
      t_uint32	code = 0;						\
									\
      SAVE_CONTEXT();							\
      LOAD_CORE_SELECTORS(0x10);					\
      if (_error_code_)					       		\
        asm volatile("movl 4(%%ebp),%%eax\n\t"				\
		     : "=a" (code)					\
		     :);						\
      asm volatile("subl $0x8, %esp");					\
      exception_wrapper(IDT_EXCEPTION_BASE + _nr_, code);		\
      asm volatile("addl $0x8, %esp");					\
      RESTORE_CONTEXT();						\
      LEAVE();								\
      if (_error_code_)							\
        asm volatile("addl $4,%esp\n\t");				\
      IRET();								\
    }

/*
 * pre-handler for irq's
 */

#define IRQ_PREHANDLER(_nr_)                                            \
  HANDLER void  prehandler_irq##_nr_(void)				\
    {                                                                   \
      __attribute__((unused)) t_uint32  code = 0;                       \
                                                                        \
      SAVE_CONTEXT();							\
      LOAD_CORE_SELECTORS(0x10);					\
      asm volatile("subl $0x8, %esp");                                  \
      irq_wrapper(IDT_IRQ_BASE + _nr_);                                 \
      asm volatile("addl $0x8, %esp");                                  \
      RESTORE_CONTEXT();						\
      LEAVE();								\
      IRET();								\
    }

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_ia32_interrupt_prehandler)(void);
typedef void			(*t_ia32_interrupt_handler)(t_uint32);

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
