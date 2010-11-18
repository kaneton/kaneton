/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/event/ia32/context/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue nov 16 18:21:22 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_context_01_handler(t_id		id)
{
  thrown = 1;

  asm volatile("movl $0x10034a10, %eax\n"
               "movl $0xf5000100, %ebx\n"
               "movl $0xaaaaaaaa, %ecx\n"
               "movl $0xffffffff, %edx\n"
               "movl $0xa5a5a5a5, %esi\n"
               "movl $0x5a5a5a5a, %edi");
}

void			test_event_ia32_context_01(void)
{
  static t_test_ctx*	ctx1;
  static t_test_ctx*	ctx2;
  static t_uint32	esp;

  TEST_ENTER();

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_context_01_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("pushl %%eax\n"
	       "pushl %%ebx\n"
	       "pushl %%ecx\n"
	       "pushl %%edx\n"
	       "pushl %%esi\n"
	       "pushl %%edi\n"
	       "pushl %%ebp\n"
	       "movl %%esp, %0\n"
	       "int $3\n"
	       "movl %%esp, %2\n"
	       "pushl %%eax\n"
	       "pushl %%ebx\n"
	       "pushl %%ecx\n"
	       "pushl %%edx\n"
	       "pushl %%esi\n"
	       "pushl %%edi\n"
	       "pushl %%ebp\n"
	       "movl %%esp, %1"
	       : "=m" (ctx1), "=m" (ctx2), "=m" (esp));

  if (event_release(3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  if (thrown != 1)
    TEST_ERROR("the triggered exception has not been caught\n");

  if (esp != ctx1)
    TEST_ERROR("the ESP register is different from before the exception\n");

  if (ctx1->eax != ctx2->eax)
    TEST_ERROR("the EAX register is different\n");
  if (ctx1->ebx != ctx2->ebx)
    TEST_ERROR("the EBX register is different\n");
  if (ctx1->ecx != ctx2->ecx)
    TEST_ERROR("the ECX register is different\n");
  if (ctx1->edx != ctx2->edx)
    TEST_ERROR("the EDX register is different\n");
  if (ctx1->esi != ctx2->esi)
    TEST_ERROR("the ESI register is different\n");
  if (ctx1->edi != ctx2->edi)
    TEST_ERROR("the EDI register is different\n");
  if (ctx1->ebp != ctx2->ebp)
    TEST_ERROR("the EBP register is different\n");

  TEST_LEAVE();
}
