/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/task.c
 *
 * created       renaud voltz   [tue apr  4 21:45:07 2006]
 * updated       matthieu bucchianeri   [tue jul 25 16:08:57 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>
#if 0
/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * backup the current context.
 *
 * steps:
 *
 * 1) push the context on the stack.
 * 2) copy it from the stack.
 * 3) adjust the stack before leaving.
 */

t_error			context_store(t_ia32_context*		context)
{
  t_ia32_context*	tmp;

  /*
   * 1)
   */

  CONTEXT_SAVE(tmp);

  /*
   * 2)
   */

  memcpy(context, tmp, sizeof(t_ia32_context));

  /*
   * 3)
   */

  asm volatile("addl $36,%esp\n\t");

  return ERROR_NONE;
}

/*
 * dump the current context.
 */

t_error			context_dump(t_ia32_context*		context)
{
  printf("general purpose registers:\n"
	 "  eax:\t0x%x\n"
	 "  ebx:\t0x%x\n"
	 "  ecx:\t0x%x\n"
	 "  edx:\t0x%x\n"
	 "  esi:\t0x%x\n"
	 "  edi:\t0x%x\n",
	 context->eax, context->ebx, context->ecx,
	 context->edx, context->esi, context->edi);

  printf("segments settings:\n"
	 "  cs:\t0x%x\n"
	 "  ds:\t0x%x\n"
	 "  es:\t0x%x\n"
	 "  fs:\t0x%x\n"
	 "  gs:\t0x%x\n"
	 "  ss:\t0x%x\n",
	 context->cs, context->ds, context->es,
	 context->fs, context->gs, context->ss);

  printf("stack settings:\n"
	 "  esp:\t0x%x (before context saving)\n"
	 "  ebp:\t0x%x\n",
	 context->esp, context->ebp);

  printf("other information:\n"
	 "  eflags:\t0x%x\n"
	 "  cr3:\t0x%x\n",
	 context->eflags, context->cr3);

  return ERROR_NONE;
}
#endif

t_ia32_context* context = NULL;
