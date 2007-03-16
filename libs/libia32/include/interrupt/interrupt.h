/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/interrupt/interrupt.h
 *
 * created       renaud voltz   [fri feb 17 16:48:22 2006]
 * updated       matthieu bucchianeri   [fri mar 16 21:54:38 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 *
 */

#ifndef IA32_INTERRUPT_INTERRUPT_H
#define IA32_INTERRUPT_INTERRUPT_H       1

/*                                                                  [cut] k2 */

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

#define EXCEPTION_NR    32
#define IRQ_NR          16
#define IPI_NR          8

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

#define EXCEPTION_PREHANDLER_CODE(nr)			       		\
  asm	(".globl prehandler_exception" #nr "		\n"		\
	 "prehandler_exception" #nr ":			\n"		\
	 SAVE_CONTEXT()							\
	 FORCE_RING0_SWITCH()						\
	 "	pushl 40(%esp)				\n"		\
	 "	pushl $" #nr "				\n"		\
	 "	call handler_exception			\n"		\
	 "	addl $8, %esp				\n"		\
	 FORCE_RING0_BACK()						\
	 RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					")

#define EXCEPTION_PREHANDLER_NOCODE(nr)			       		\
  asm	(".globl prehandler_exception" #nr "		\n"		\
	 "prehandler_exception" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 SAVE_CONTEXT()							\
	 FORCE_RING0_SWITCH()						\
	 "	pushl $0				\n"		\
	 "	pushl $" #nr "				\n"		\
	 "	call handler_exception			\n"		\
	 "	addl $8, %esp				\n"		\
	 FORCE_RING0_BACK()						\
	 RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					")

#define IRQ_PREHANDLER(nr)				       		\
  asm	(".globl prehandler_irq" #nr "			\n"		\
	 "prehandler_irq" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 SAVE_CONTEXT()							\
	 FORCE_RING0_SWITCH()						\
	 "	pushl $0				\n"		\
	 "	pushl $" #nr "				\n"		\
	 "	call handler_irq			\n"		\
	 "	addl $8, %esp				\n"		\
	 FORCE_RING0_BACK()						\
	 RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					")

#define IPI_PREHANDLER(nr)				       		\
  asm	(".globl prehandler_ipi" #nr "			\n"		\
	 "prehandler_ipi" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 SAVE_CONTEXT()							\
	 FORCE_RING0_SWITCH()						\
	 "	pushl $0				\n"		\
	 "	pushl $" #nr "				\n"		\
	 "	call handler_ipi			\n"		\
	 "	addl $8, %esp				\n"		\
	 FORCE_RING0_BACK()						\
	 RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					")

/*
 * force a stack switch event if coming from ring0.
 */

#define FORCE_RING0_SWITCH()						\
  "	movl interrupt_stack, %eax			\n"		\
  "	testl %eax, %eax				\n"		\
  "	jz 1f						\n"		\
  "	movl 48(%esp), %eax				\n"		\
  "	andl $3, %eax					\n"		\
  "	jnz 1f						\n"		\
  "	pushl $64					\n"		\
  "	pushl context					\n"		\
  "	movl interrupt_stack, %eax			\n"		\
  "	addl $-64, %eax					\n"		\
  "	pushl %eax					\n"		\
  "	call memcpy					\n"		\
  "	popl %ebx					\n"		\
  "	movl %ebx, context				\n"		\
  "	movw 6(%ebx), %ax				\n"		\
  "	movw %ax, 60(%ebx)				\n"		\
  "	popl %eax					\n"		\
  "	addl $56, %eax					\n"		\
  "	movl %eax, 56(%ebx)				\n"		\
  "	addl $4, %esp					\n"		\
  "	movl %ebx, %esp					\n"		\
  "1:							\n"

/*
 * force a stack switch if coming to ring 0.
 */

#define FORCE_RING0_BACK()						\
  "	movl interrupt_stack, %eax			\n"		\
  "	testl %eax, %eax				\n"		\
  "	jz 1f						\n"		\
  "	movl 48(%esp), %eax				\n"		\
  "	andl $3, %eax					\n"		\
  "	jnz 1f						\n"		\
  "	movl 56(%esp), %eax				\n"		\
  "	addl $-56, %eax					\n"		\
  "	push $56					\n"		\
  "	push context					\n"		\
  "	push %eax					\n"		\
  "	call memcpy					\n"		\
  "	popl context					\n"		\
  "	addl $8, %esp					\n"		\
  "	movl context, %esp				\n"		\
  "1:							\n"

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_ia32_interrupt_prehandler)(void);
typedef void			(*t_ia32_interrupt_handler)(t_id, t_uint32);

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

void    prehandler_ipi0(void);
void    prehandler_ipi1(void);
void    prehandler_ipi2(void);
void    prehandler_ipi3(void);
void    prehandler_ipi4(void);
void    prehandler_ipi5(void);
void    prehandler_ipi6(void);
void    prehandler_ipi7(void);

/*                                                                 [cut] /k2 */

#endif
