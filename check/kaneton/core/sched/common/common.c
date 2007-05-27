/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sat oct 14 16:56:32 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_sched_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

static unsigned char	bcd2bin(unsigned char c)
{
  unsigned char res = 0;

  unsigned char c1 = (c & 0xF0) >> 4;
  res += 10 * c1;
  c1 = c & 0x0F;
  res += c1;
  return res;
}

/*
 * get seconds from RTC
 */
#if 0
t_uint32 check_cmos_sec(void)
{
  int i;

  OUTB(0x70, 0);
  INB(0x71, i);
  return bcd2bin(i);
}
#endif
/*
 * starts ALL tests related to time manager
 */

void	check_sched_tests(void)
{
  check_sched_common();
  /* XXX continue tests */
}

int		check_task_create(t_class		class,
				  i_task		*task)
{
  i_as		as;

  if (task_reserve(class,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   task) != ERROR_NONE)
    {
      printf("error in task_reserve\n");
      return -1;
    }

  if (as_reserve(*task, &as) != ERROR_NONE)
    {
      printf("error in as_reserve\n");
      return -1;
    }

  return 0;
}

int		check_thread_create(i_task		task,
				    t_prior		prior,
				    t_vaddr		entry,
				    i_thread		*thread)
{
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;

  if (thread_reserve(task, prior, thread) != ERROR_NONE)
    {
      printf("error in thread_reserve\n");
      return -1;
    }

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(*thread, stack) != ERROR_NONE)
    {
      printf("error in thread_stack\n");
      return -1;
    }

  if (thread_get(*thread, &o) != ERROR_NONE)
    {
      printf("error in thread_get\n");
      return -1;
    }

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)entry;

  if (thread_load(*thread, ctx) != ERROR_NONE)
    {
      printf("error in thread_get\n");
      return -1;
    }

  return 0;
}
