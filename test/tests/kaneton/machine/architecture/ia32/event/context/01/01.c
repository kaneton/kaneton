/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/ia32/educational/event/context/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat feb  5 20:55:28 2011]
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

void			test_architecture_event_context_01_handler(t_id	id)
{
  thrown = 1;

  asm volatile("movl $0x10034a10, %eax\n"
               "movl $0xf5000100, %ebx\n"
               "movl $0xaaaaaaaa, %ecx\n"
               "movl $0xffffffff, %edx\n"
               "movl $0xa5a5a5a5, %esi\n"
               "movl $0x5a5a5a5a, %edi");
}

void			test_architecture_event_context_01(void)
{
  t_test_ctx*		ctx1;
  t_test_ctx*		ctx2;
  t_uint32		esp;

  TEST_ENTER();

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_context_01_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  if (event_enable() != STATUS_OK)
    TEST_ERROR("[event_enable] error");

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

  if (event_disable() != STATUS_OK)
    TEST_ERROR("[event_disable] error");

  if (thrown != 1)
    TEST_ERROR("the triggered exception has not been caught");

  if (esp != (t_uint32)ctx1)
    TEST_ERROR("the ESP register is different from before the exception");

  if (ctx1->eax != ctx2->eax)
    TEST_ERROR("the EAX register is different");
  if (ctx1->ebx != ctx2->ebx)
    TEST_ERROR("the EBX register is different");
  if (ctx1->ecx != ctx2->ecx)
    TEST_ERROR("the ECX register is different");
  if (ctx1->edx != ctx2->edx)
    TEST_ERROR("the EDX register is different");
  if (ctx1->esi != ctx2->esi)
    TEST_ERROR("the ESI register is different");
  if (ctx1->edi != ctx2->edi)
    TEST_ERROR("the EDI register is different");
  if (ctx1->ebp != ctx2->ebp)
    TEST_ERROR("the EBP register is different");

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_BP) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(sdvni20fwg9i3h0);

  TEST_LEAVE();
}
