/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check-clean/check/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [sun may 27 13:02:26 2007]
 */

#include <libc.h>
#include "common.h"

void	check_check_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests manually
 */

void	check_tests(void)
{
  check_task_clone_01();
  /* XXX */
}

/*
 * for leaks tracking
 */

void	check_leaks_init(t_leaks*	l)
{
  l->nb_alloc = alloc_nalloc();
  l->nb_free = alloc_nfree();
}

void	check_leaks_display(t_leaks*	l)
{
  if ((l->nb_alloc - l->nb_free) != (alloc_nalloc() - alloc_nfree()))
/*    printf("error: memory leaks detected: %u/%u -> %u/%u\n",
	   l->nb_alloc, l->nb_free, alloc_nalloc(), alloc_nfree());*/
    printf("-->>LEAK<<--\n");
}

/*
 * exception handling initialization.
 *
 * steps:
 *
 * 1) store caller's ebp in order to be able to restore context as is
      was at the beginning of the function
 * 2) store the old IDT vector pointer.
 * 3) initialize our IDT Register.
 * 4) create our IDT.
 * 5) we do not handler IRQ's.
 * 6) setup our IDT as the active one.
 */

void		check_exc_init(t_exc*		exc,
			       void		(*f)())
{
  typedef struct
  {
    t_uint16	offset_00_15;
    t_uint16	segsel;
    t_uint8	reserved;
    t_uint8	type;
    t_uint16	offset_16_31;
  }		__attribute__ ((packed)) __idte;

  int		i;
  t_uint32	*addr;
  t_uint16	*sz;
  __idte	*idt;

  /*
   * 1)
   */

  asm volatile("movl (%%ebp), %0"
	       : "=r" (exc->ebp));

  /*
   * 2)
   */

  asm volatile("sidt (%0)\n"
	       :
	       : "r" (exc->old_idtr));

  /*
   * 3)
   */

  sz = (t_uint16*)&exc->idtr[0];
  addr = (t_uint32*)&exc->idtr[2];
  *addr = (t_uint32)exc->idt;
  *sz = 32 * 8;

  /*
   * 4)
   */

  idt = (__idte*)exc->idt;

  for (i = 0; i < 32; i++)
    {
      idt[i].offset_00_15 = (t_uint16)(t_uint32)f;
      idt[i].segsel = 0x8;
      idt[i].type = (1 << 7) | (1 << 3) | (1 << 2) | (1 << 1);
      idt[i].offset_16_31 = (t_uint32)f >> 16;
      idt[i].reserved = 0;
    }

  /*
   * 5)
   */

  exc->flag_if = 1; /* XXX */

  asm volatile("cli");

  /*
   * 6)
   */

  asm volatile("lidt (%0)\n"
	       :
	       : "r" (exc->idtr));
}

/*
 * this function is called on exception.
 *
 * steps:
 *
 * 1) print a sweet message.
 * 2) restore old IDT.
 * 3) restore old base pointer. /!\ WE DO NOT RESTORE THE ENTIRE CONTEXT /!\
 * 4) return.
 */

void	check_exc_catch(t_exc*		exc)
{
  /*
   * 1)
   */

  printf("Exception Catched\n");

  /*
   * 2)
   */

  check_exc_reset(exc);

  /*
   * 3)
   */

  asm volatile("movl %0, %%ebp"
	       :
	       : "r" (exc->ebp));
  /*
   * 4)
   */

#ifdef CHECK_DO_NOT_RESUME
  while (1)
    ;
#endif

  asm volatile("leave\n\t"
	       "ret");
}

/*
 * this function restore the interrupt and exception handling
 * environment as it was before.
 *
 * we restore both the IDT and the IF flag.
 */

void	check_exc_reset(t_exc*		exc)
{
  asm volatile("lidt (%0)\n"
	       :
	       : "r" (exc->old_idtr));

  if (exc->flag_if)
    asm volatile("sti");
}

/*
 *:
 */

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

t_uint32 check_cmos_sec(void)
{
  int i;

  OUTB(0x70, 0);
  INB(0x71, i);
  return bcd2bin(i);
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
