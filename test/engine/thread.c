/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/repo...ries/kaneton.STABLE/test/engine/thread.c
 *
 * created       julien quintard   [sun oct 17 15:45:12 2010]
 * updated       julien quintard   [sun oct 17 15:56:48 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "engine.h"

/*
 * ---------- functions -------------------------------------------------------
 */

int		test_engine_thread_spawn(i_task			task,
					 t_prior		prior,
					 t_vaddr		entry,
					 i_thread*		thread)
{
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  if (thread_reserve(task, prior, thread) != ERROR_NONE)
    {
      printf("[thread_reserve] error\n");
      return (ERROR_UNKNOWN);
    }

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(*thread, stack) != ERROR_NONE)
    {
      printf("[thread_stack] error\n");
      return (ERROR_UNKNOWN);
    }

  if (thread_get(*thread, &o) != ERROR_NONE)
    {
      printf("[thread_get] error\n");
      return (ERROR_UNKNOWN);
    }

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)entry;

  if (thread_load(*thread, ctx) != ERROR_NONE)
    {
      printf("[thread_get] error\n");
      return (ERROR_UNKNOWN);
    }

  if (ia32_get_context(*thread, &ia32_ctx) != ERROR_NONE)
    {
      printf("[ia32_get_context] error\n");
      return (ERROR_UNKNOWN);
    }

  ia32_ctx.eflags |= (1 << 12);
  ia32_ctx.eflags |= (1 << 13);

  if (ia32_set_context(*thread, &ia32_ctx, IA32_CONTEXT_EFLAGS) != ERROR_NONE)
    {
      printf("[ia32_set_context] error\n");
      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}
