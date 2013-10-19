/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/modules/debug/handler.h
 *
 * created       julio guerra   [sun oct 6 22:20:22 2013]
 * updated       julio guerra   [sun oct 6 22:46:05 2013]
 */

#ifndef MODULES_DEBUG_HANDLER_H
# define MODULES_DEBUG_HANDLER_H

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this shell is for debug exceptions: the interrupts are disabled, the context
 * is saved, the exception number is passed to handler which is then called.
 *
 */

#define DEBUG_HANDLER_SHELL_EXCEPTION_CODE(_n_)                         \
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
 * ---------- prototypes ------------------------------------------------------
 *
 */

/*
 * ../handler.S
 */

void architecture_handler_shell_exception1();
void architecture_handler_shell_exception3();

/*
 * eop
 */

#endif /* !MODULES_DEBUG_HANDLER_H */
