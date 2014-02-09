/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/interrupt/interrupt.h
 *
 * created       renaud voltz   [fri feb 17 16:48:22 2006]
 * updated       matthieu bucchianeri   [sun may  6 17:23:08 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 *
 */

#ifndef ARCHITECTURE_INTERRUPT_H
#define ARCHITECTURE_INTERRUPT_H       1


/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * interrupts number.
 */

#define IA32_EXCEPTION_NR	32
#define IA32_IRQ_NR		16
#define IA32_IPI_NR		8
#define IA32_SYSCALL_NR		10

#define IA32_HANDLER_NR		IA32_EXCEPTION_NR + IA32_IRQ_NR +	\
				IA32_IPI_NR + IA32_SYSCALL_NR

/*
 * exception error code.
 */

#define IA32_ERROR_CODE		1
#define IA32_NO_ERROR_CODE	0

/*
 * specific  to handlers.  this is  used to  place handler  code  in a
 * special ELF section.
 */

#define IA32_HANDLER_SECTION						\
  __attribute__ ((section(".handler")))

#define IA32_HANDLER_DATA_SECTION					\
  __attribute__ ((section(".handler_data")))

#define IA32_EXCEPTION_PREHANDLER_CODE(nr)		       		\
  asm	(".section .handler				\n"		\
	 "prehandler_exception" #nr ":			\n"		\
	 IA32_SAVE_CONTEXT()						\
	 "	pushl 40(%esp)				\n"		\
	 "	pushl $" #nr "				\n"		\
	 "	call ia32_handler_exception		\n"		\
	 "	addl $8, %esp				\n"		\
	 IA32_RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					\n"		\
	 ".text						")

#define IA32_EXCEPTION_PREHANDLER_NOCODE(nr)		       		\
  asm	(".section .handler				\n"		\
	 "prehandler_exception" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 IA32_SAVE_CONTEXT()						\
	 "	pushl $0				\n"		\
	 "	pushl $" #nr "				\n"		\
	 "	call ia32_handler_exception		\n"		\
	 "	addl $8, %esp				\n"		\
	 IA32_RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					\n"		\
	 ".text						")

#define IA32_IRQ_PREHANDLER(nr)				       		\
  asm	(".section .handler				\n"		\
	 "prehandler_irq" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 IA32_SAVE_CONTEXT()						\
	 "	pushl $" #nr "				\n"		\
	 "	call ia32_handler_irq			\n"		\
	 "	addl $4, %esp				\n"		\
	 IA32_RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					\n"		\
	 ".text						")

#define IA32_IPI_PREHANDLER(nr)				       		\
  asm	(".section .handler				\n"		\
	 "prehandler_ipi" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 IA32_SAVE_CONTEXT()						\
	 "	pushl $" #nr "				\n"		\
	 "	call ia32_handler_ipi			\n"		\
	 "	addl $4, %esp				\n"		\
	 IA32_RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					\n"		\
	 ".text						")

#define IA32_SYSCALL_PREHANDLER(nr)			       		\
  asm	(".section .handler				\n"		\
	 "prehandler_syscall" #nr ":			\n"		\
	 "	addl $-4, %esp				\n"		\
	 IA32_SAVE_CONTEXT()						\
	 "	pushl $" #nr "				\n"		\
	 "	call ia32_handler_syscall		\n"		\
	 "	addl $4, %esp				\n"		\
	 IA32_RESTORE_CONTEXT()						\
	 "	addl $4, %esp				\n"		\
	 "	iret					\n"		\
	 ".text						")

#define IA32_CALL_HANDLER(_handler_, ...)				\
  ((t_ia32_interrupt_handler)((_handler_).function))(__VA_ARGS__)

#define PREHANDLER_PROTOTYPES()						\
  void	prehandler_exception0(void);					\
  void	prehandler_exception1(void);					\
  void	prehandler_exception2(void);					\
  void	prehandler_exception3(void);					\
  void	prehandler_exception4(void);					\
  void	prehandler_exception5(void);					\
  void	prehandler_exception6(void);					\
  void	prehandler_exception7(void);					\
  void	prehandler_exception8(void);					\
  void	prehandler_exception9(void);					\
  void	prehandler_exception10(void);					\
  void	prehandler_exception11(void);					\
  void	prehandler_exception12(void);					\
  void	prehandler_exception13(void);					\
  void	prehandler_exception14(void);					\
  void	prehandler_exception15(void);					\
  void	prehandler_exception16(void);					\
  void	prehandler_exception17(void);					\
  void	prehandler_exception18(void);					\
  void	prehandler_exception19(void);					\
  void	prehandler_exception20(void);					\
  void	prehandler_exception21(void);					\
  void	prehandler_exception22(void);					\
  void	prehandler_exception23(void);					\
  void	prehandler_exception24(void);					\
  void	prehandler_exception25(void);					\
  void	prehandler_exception26(void);					\
  void	prehandler_exception27(void);					\
  void	prehandler_exception28(void);					\
  void	prehandler_exception29(void);					\
  void	prehandler_exception30(void);					\
  void	prehandler_exception31(void);					\
  									\
  void	prehandler_irq0(void);						\
  void	prehandler_irq1(void);						\
  void	prehandler_irq2(void);						\
  void	prehandler_irq3(void);						\
  void	prehandler_irq4(void);						\
  void	prehandler_irq5(void);						\
  void	prehandler_irq6(void);						\
  void	prehandler_irq7(void);						\
  void	prehandler_irq8(void);						\
  void	prehandler_irq9(void);						\
  void	prehandler_irq10(void);						\
  void	prehandler_irq11(void);						\
  void	prehandler_irq12(void);						\
  void	prehandler_irq13(void);						\
  void	prehandler_irq14(void);						\
  void	prehandler_irq15(void);						\
  									\
  void	prehandler_ipi0(void);						\
  void	prehandler_ipi1(void);						\
  void	prehandler_ipi2(void);						\
  void	prehandler_ipi3(void);						\
  void	prehandler_ipi4(void);						\
  void	prehandler_ipi5(void);						\
  void	prehandler_ipi6(void);						\
  void	prehandler_ipi7(void);						\
  									\
  void	prehandler_syscall0(void);					\
  void	prehandler_syscall1(void);					\
  void	prehandler_syscall2(void);					\
  void	prehandler_syscall3(void);					\
  void	prehandler_syscall4(void);					\
  void	prehandler_syscall5(void);					\
  void	prehandler_syscall6(void);					\
  void	prehandler_syscall7(void);					\
  void	prehandler_syscall8(void);					\
  void	prehandler_syscall9(void);

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/id.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_ia32_interrupt_prehandler)(void);
typedef void			(*t_ia32_interrupt_handler)();

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * on-interrupt data segment to load.
 */

extern t_uint16	ia32_interrupt_ds;

/*
 * on-interrupt PDBR to load.
 */

extern t_uint32	ia32_interrupt_pdbr;


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../interrupt.c
 */

/*
 * ../interrupt.c
 */

t_status		ia32_interrupt_vector_init(void);

void			ia32_handler_exception(t_uint32			nr,
					       t_uint32			code);

void			ia32_handler_irq(t_uint32			nr);

void			ia32_handler_ipi(t_uint32			nr);

void			ia32_handler_syscall(t_uint32			nr);


/*
 * eop
 */

#endif
