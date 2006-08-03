/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/interrupt/interrupt.h
 *
 * created       renaud voltz   [fri feb 17 16:48:22 2006]
 * updated       matthieu bucchianeri   [thu aug  3 16:32:18 2006]
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

#define HANDLER_DATA				\
  __attribute__ ((section("handler_data")))

/*
 * switch to the kernel, loading segment selectors and PDBR.
 */

#define SWITCH_KERNEL()							\
  asm volatile("pushl %eax\n\t"						\
	       "movl %cr3, %eax\n\t"					\
	       "pushl %eax");						\
  asm volatile("movl %%eax, %%cr3"					\
	       :							\
	       : "a" (interrupt_pdbr));					\
  asm volatile("pushl %ds\n\t"						\
	       "pushl %es\n\t"						\
	       "pushl %fs\n\t"						\
	       "pushl %gs");						\
  asm volatile("movw %%ax, %%ds\n\t"					\
	       "movw %%ax, %%es\n\t"					\
	       "movw %%ax, %%fs\n\t"					\
	       "movw %%ax, %%gs"					\
	       :							\
	       : "a" (interrupt_ds))


/*
 * switch back to the process.
 */

#define SWITCH_BACK()							\
  asm volatile("popl %gs\n\t"						\
	       "popl %fs\n\t"						\
	       "popl %es\n\t"						\
	       "popl %ds\n\t"						\
	       "popl %eax\n\t"						\
	       "movl %eax, %cr3\n\t"					\
	       "popl %eax");

/*
 * adjust stack to clear error code.
 */

#define CLEAR_ERROR_CODE()						\
  asm volatile("addl $4,%esp\n\t")

/*
 * get the error code in nested handler.
 */

#define GET_ERROR_CODE(_code_)						\
  asm volatile("movl (%%ebp), %%eax\n\t"				\
	       "movl 4(%%eax), %0"					\
	       : "=r" (_code_)						\
	       :							\
	       : "%eax")

/*
 * pre-handler for exceptions. minimal  context save and address space
 * switch.
 */

#define EXCEPTION_PREHANDLER(_nr_, _error_code_)	       		\
  HANDLER void	prehandler_exception##_nr_(void)			\
  {									\
    SWITCH_KERNEL();							\
    handler_exception(IDT_EXCEPTION_BASE + _nr_, _error_code_);		\
    SWITCH_BACK();							\
    LEAVE();								\
    if (_error_code_)							\
      CLEAR_ERROR_CODE();						\
    IRET();								\
  }

/*
 * pre-handler  for  irqs.  minimal  context  save  and address  space
 * switch.
 */

#define IRQ_PREHANDLER(_nr_)                                            \
  HANDLER void  prehandler_irq##_nr_(void)				\
  {									\
    SWITCH_KERNEL();							\
    handler_irq(IDT_IRQ_BASE + _nr_);					\
    SWITCH_BACK();							\
    LEAVE();								\
    IRET();								\
  }

/*
 * force a stack switch event if coming from ring0.
 */

#define RING0_SWITCH()							\
  if (!(context->cs & 0x3) && interrupt_stack)				\
    {									\
      memcpy((void*)(interrupt_stack - sizeof(t_ia32_context)),		\
	     context,							\
	     sizeof(t_ia32_context));					\
      if (cpucaps & IA32_CAPS_SSE)					\
	memcpy((void*)(interrupt_stack - sizeof(t_ia32_context) - 528),	\
	       (t_uint8*)context - 528,					\
	       528);							\
      else								\
	memcpy((void*)(interrupt_stack - sizeof(t_ia32_context) - 108),	\
	       (t_uint8*)context - 108,					\
	       108);							\
      code = (t_uint32)context;						\
      context = (void*)(interrupt_stack - sizeof(t_ia32_context));	\
      context->ebp_handler = (t_uint32)context +			\
	sizeof(t_ia32_context) - 24;					\
      context->ss = context->ds;					\
      context->esp = code + sizeof(t_ia32_context) - 8;			\
      asm volatile("movl %0, %%ebp"					\
		   :							\
		   : "r" ((t_uint8*)context + 44));			\
      asm volatile("movl %0, %%esp"					\
		   :							\
		   : "r" (context));					\
      if (cpucaps & IA32_CAPS_SSE)					\
	asm volatile("subl $512, %esp\n\t"				\
		     "andw $0xFFF0, %sp\n\t"				\
		     "subl $4, %esp");					\
      else								\
	asm volatile("subl $108, %esp");				\
    }

/*
 * force a stack switch if coming to ring 0.
 */

#define RING0_BACK()							\
  if (!(context->cs & 0x3) && interrupt_stack)				\
    {									\
      memcpy((void*)(context->esp - sizeof(t_ia32_context) + 8),	\
	     (t_uint8*)context,						\
	     sizeof(t_ia32_context) - 8);				\
      if (cpucaps & IA32_CAPS_SSE)					\
	memcpy((void*)(context->esp - sizeof(t_ia32_context) + 8 - 528), \
	       (t_uint8*)context - 528,					\
	       528);							\
      else								\
	memcpy((void*)(context->esp - sizeof(t_ia32_context) + 8 - 108), \
	       (t_uint8*)context - 108,					\
	       108);							\
      context = (void*)(context->esp - sizeof(t_ia32_context) + 8);	\
      context->ebp_handler = (t_uint32)context +			\
	sizeof(t_ia32_context) - 24;					\
      asm volatile("movl %0, %%ebp"					\
		   :							\
		   : "r" ((t_uint8*)context + 44));			\
      asm volatile("movl %0, %%esp"					\
		   :							\
		   : "r" (context));					\
      if (cpucaps & IA32_CAPS_SSE)					\
	asm volatile("subl $512, %esp\n\t"				\
		     "andw $0xFFF0, %sp\n\t"				\
		     "subl $4, %esp");					\
      else								\
	asm volatile("subl $108, %esp");				\
    }

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_ia32_interrupt_prehandler)(void);
typedef void			(*t_ia32_interrupt_handler)(t_uint32);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * on-interrupt data segment to load.
 */

extern volatile t_uint16	interrupt_ds;

/*
 * on-interrupt PDBR to load.
 */

extern volatile t_uint32	interrupt_pdbr;

extern volatile t_uint32	interrupt_stack;

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
