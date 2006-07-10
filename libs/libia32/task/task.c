/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/task.c
 *
 * created       renaud voltz   [tue apr  4 21:45:07 2006]
 * updated       matthieu bucchianeri   [mon jul 10 10:47:23 2006]
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

t_ia32_context ctx[2];

t_uint32 current = 0;

void fun1()
{
  printf("fun1: 1\n");
  asm volatile("int $31");
  printf("fun1: 2\n");
  asm volatile("int $31");
  printf("fun1: 3\n");
  asm volatile("int $31");
  while(1)
    ;
}

void fun2()
{
  printf("fun2: 1\n");
  asm volatile("int $31");
  printf("fun2: 2\n");
  asm volatile("int $31");
  printf("fun2: 3\n");
  asm volatile("int $31");
  while(1);
}

void chiche_ctx(t_uint32 id)
{
  printf("SWITCH %d -> %d\n", current, (current + 1) % 2);
/*  int i;
  for (i = 0; i < 100000000; i++)
    ;*/
  memcpy(&ctx[current], context, sizeof(t_ia32_context));
  printf("old EIP = %p\n", context->eip);
  current = (current + 1) % 2;
  memcpy(context, &ctx[current], sizeof(t_ia32_context));
  printf("new EIP = %p\n", context->eip);
}

void ctx_test()
{
  t_uint32 cr3;
  t_uint32 eflags;

  if (event_reserve(31, EVENT_FUNCTION, (u_event_handler)chiche_ctx)
      != ERROR_NONE)
    {
      printf("error\n");
      return;
    }

  asm volatile("movl %%cr3, %%eax\n\t"
	       "movl %%eax, %0"
	       : "=g" (cr3)
	       :
	       : "%eax");
  asm volatile("pushf\n\t"
	       "popl %0" : "=g" (eflags));
  memset(&ctx[0], 0, sizeof(t_ia32_context));
  memset(&ctx[1], 0, sizeof(t_ia32_context));
  ctx[1].ds = ctx[0].ds = 0x10;
  ctx[1].es = ctx[0].es = 0x10;
  ctx[1].fs = ctx[0].fs = 0x10;
  ctx[1].gs = ctx[0].gs = 0x10;
  ctx[1].ss = ctx[0].ss = 0x10;
  ctx[1].cs = ctx[0].cs = 0x8;
  ctx[0].eip = fun1;
  ctx[1].eip = fun2;
  ctx[1].cr3 = ctx[0].cr3 = cr3;
  ctx[1].eflags = ctx[0].eflags = eflags;

  fun1();
}
