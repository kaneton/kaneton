/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/educational/include/handler.h
 *
 * created       renaud voltz   [fri feb 17 16:48:22 2006]
 * updated       julien quintard   [mon apr 11 13:46:05 2011]
 */

#ifndef ARCHITECTURE_HANDLER_H
#define ARCHITECTURE_HANDLER_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro represents the number of handlers the system must set up,
 * for the exceptions, IRQs, IPIs and syscalls.
 */

#define ARCHITECTURE_HANDLER_SIZE	ARCHITECTURE_IDT_EXCEPTION_SIZE + \
					ARCHITECTURE_IDT_IRQ_SIZE +	  \
					ARCHITECTURE_IDT_IPI_SIZE +	  \
					ARCHITECTURE_IDT_SYSCALL_SIZE

/*
 * these macros defines the error flags contained in the page-fault-specific
 * exception code.
 */

#define ARCHITECTURE_HANDLER_PAGEFAULT_PRIVILEGE	(1 << 0)
#define ARCHITECTURE_HANDLER_PAGEFAULT_WRITE		(1 << 1)
#define ARCHITECTURE_HANDLER_PAGEFAULT_USER		(1 << 2)

/*
 * this macro defines the size of a thread's pile i.e the stack used whenever
 * a privilege change occurs.
 */

#define ARCHITECTURE_HANDLER_PILE_SIZE	___kaneton$pagesz

/*
 * this macro defines the size of the KIS - Kernel Interrupt Stack. this is
 * the stack which is used, within the kernel environment, for treating
 * interrupts.
 */

#define ARCHITECTURE_HANDLER_KIS_SIZE	2 * ___kaneton$pagesz

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this shell is for exceptions: the interrupts are disabled, the context
 * is saved, the exception number is passed to handler which is then called.
 */

#define ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(_n_)		       	\
  asm (".section .handler_code					\n"	\
       "architecture_handler_shell_exception" #_n_ ":		\n"	\
       "  cli							\n");	\
  ARCHITECTURE_CONTEXT_SAVE();						\
  asm ("  pushl %ebx						\n"	\
       "  pushl $" #_n_ "					\n"	\
       "  call architecture_handler_exception			\n"	\
       "  addl $8, %esp						");	\
  ARCHITECTURE_CONTEXT_RESTORE();					\
  asm ("  addl $4, %esp						\n"	\
       "  sti							\n"	\
       "  iret							\n"	\
       ".text							");

/*
 * this shell is identical to the previous one, i.e for exceptions, except
 * that it must be used for exceptions which are devoid of error codes
 * such as the divide-by-zero exception for instance.
 */

#define ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(_n_)		\
  asm (".section .handler_code					\n"	\
       "architecture_handler_shell_exception" #_n_ ":		\n"	\
       "  cli							\n"	\
       "  addl $-4, %esp					");	\
  ARCHITECTURE_CONTEXT_SAVE();						\
  asm ("  pushl $0						\n"	\
       "  pushl $" #_n_ "					\n"	\
       "  call architecture_handler_exception			\n"	\
       "  addl $8, %esp				");			\
  ARCHITECTURE_CONTEXT_RESTORE();					\
  asm ("  addl $4, %esp						\n"	\
       "  sti							\n"	\
       "  iret							\n"	\
       ".text							");

/*
 * this shell is for IRQs - Interrupt Requests. as for the previous shells,
 * the IRQ number is passed to the handler while the interrupts have been
 *  disabled.
 */

#define ARCHITECTURE_HANDLER_SHELL_IRQ(_n_)		       		\
  asm (".section .handler_code					\n"	\
       "architecture_handler_shell_irq" #_n_ ":			\n"	\
       "  cli							\n"	\
       "  addl $-4, %esp					");	\
  ARCHITECTURE_CONTEXT_SAVE();						\
  asm ("  pushl $" #_n_ "					\n"	\
       "  call architecture_handler_irq				\n"	\
       "  addl $4, %esp						");	\
  ARCHITECTURE_CONTEXT_RESTORE();					\
  asm ("  addl $4, %esp						\n"	\
       "  sti							\n"	\
       "  iret							\n"	\
       ".text							");

/*
 * this shell is for system calls and is similar to the previous one.
 */

#define ARCHITECTURE_HANDLER_SHELL_SYSCALL(_n_)		       		\
  asm (".section .handler_code					\n"	\
       "architecture_handler_shell_syscall" #_n_ ":		\n"	\
       "  cli							\n"	\
       "  addl $-4, %esp					");	\
  ARCHITECTURE_CONTEXT_SAVE();						\
  asm ("  pushl $" #_n_ "					\n"	\
       "  call architecture_handler_syscall			\n"	\
       "  addl $4, %esp						");	\
  ARCHITECTURE_CONTEXT_RESTORE();					\
  asm ("  addl $4, %esp						\n"	\
       "  sti							\n"	\
       "  iret							\n"	\
       ".text							");

/*
 * this macro-function generates the prototypes of the handler shells.
 */

#define ARCHITECTURE_HANDLER_SHELL_PROTOTYPES()				\
  void	architecture_handler_shell_exception0(void);			\
  void	architecture_handler_shell_exception1(void)                     \
    __attribute__ ((weak));                                             \
  void	architecture_handler_shell_exception2(void);			\
  void	architecture_handler_shell_exception3(void)			\
    __attribute__ ((weak));                                             \
  void	architecture_handler_shell_exception4(void);			\
  void	architecture_handler_shell_exception5(void);			\
  void	architecture_handler_shell_exception6(void);			\
  void	architecture_handler_shell_exception7(void);			\
  void	architecture_handler_shell_exception8(void);			\
  void	architecture_handler_shell_exception9(void);			\
  void	architecture_handler_shell_exception10(void);			\
  void	architecture_handler_shell_exception11(void);			\
  void	architecture_handler_shell_exception12(void);			\
  void	architecture_handler_shell_exception13(void);			\
  void	architecture_handler_shell_exception14(void);			\
  void	architecture_handler_shell_exception15(void);			\
  void	architecture_handler_shell_exception16(void);			\
  void	architecture_handler_shell_exception17(void);			\
  void	architecture_handler_shell_exception18(void);			\
  void	architecture_handler_shell_exception19(void);			\
  void	architecture_handler_shell_exception20(void);			\
  void	architecture_handler_shell_exception21(void);			\
  void	architecture_handler_shell_exception22(void);			\
  void	architecture_handler_shell_exception23(void);			\
  void	architecture_handler_shell_exception24(void);			\
  void	architecture_handler_shell_exception25(void);			\
  void	architecture_handler_shell_exception26(void);			\
  void	architecture_handler_shell_exception27(void);			\
  void	architecture_handler_shell_exception28(void);			\
  void	architecture_handler_shell_exception29(void);			\
  void	architecture_handler_shell_exception30(void);			\
  void	architecture_handler_shell_exception31(void);			\
  									\
  void	architecture_handler_shell_irq0(void);				\
  void	architecture_handler_shell_irq1(void);				\
  void	architecture_handler_shell_irq2(void);				\
  void	architecture_handler_shell_irq3(void);				\
  void	architecture_handler_shell_irq4(void);				\
  void	architecture_handler_shell_irq5(void);				\
  void	architecture_handler_shell_irq6(void);				\
  void	architecture_handler_shell_irq7(void);				\
  void	architecture_handler_shell_irq8(void);				\
  void	architecture_handler_shell_irq9(void);				\
  void	architecture_handler_shell_irq10(void);				\
  void	architecture_handler_shell_irq11(void);				\
  void	architecture_handler_shell_irq12(void);				\
  void	architecture_handler_shell_irq13(void);				\
  void	architecture_handler_shell_irq14(void);				\
  void	architecture_handler_shell_irq15(void);				\
  									\
  void	architecture_handler_shell_syscall0(void);			\
  void	architecture_handler_shell_syscall1(void);			\
  void	architecture_handler_shell_syscall2(void);			\
  void	architecture_handler_shell_syscall3(void);			\
  void	architecture_handler_shell_syscall4(void);			\
  void	architecture_handler_shell_syscall5(void);			\
  void	architecture_handler_shell_syscall6(void);			\
  void	architecture_handler_shell_syscall7(void);			\
  void	architecture_handler_shell_syscall8(void);			\
  void	architecture_handler_shell_syscall9(void);

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

#include <architecture/register.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this type defines a pointer on a shell function.
 */

typedef void			(*at_handler_shell)(void);

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../handler.c
 */

/*
 * ../handler.c
 */

t_status		architecture_handler_setup(void);

void			architecture_handler_spurious(t_uint32	n);

void			architecture_handler_exception(t_uint32	n,
						       t_uint32	code);

void			architecture_handler_irq(t_uint32	n);

void			architecture_handler_syscall(t_uint32	n);


/*
 * eop
 */

#endif
