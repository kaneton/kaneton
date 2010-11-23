/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../architecture/ia32/event/context/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sat nov 20 23:58:48 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ia32_event_context_02_handler(t_id		id)
{
  t_uint16		ds;

  thrown = 1;

  if (ia32_gdt_build_selector(14, ia32_prvl_supervisor, &ds) != ERROR_NONE)
    TEST_ERROR("[ia32_gdt_build_selector] error\n");

  asm volatile("movl $0x10034a10, %eax\n"
               "movl $0xf5000100, %ebx\n"
               "movl $0xaaaaaaaa, %ecx\n"
               "movl $0xffffffff, %edx\n"
               "movl $0xa5a5a5a5, %esi\n"
               "movl $0x5a5a5a5a, %edi");

  asm volatile("movw %0, %%ax\n"
               "movw %%ax, %%ds\n"
               "movw %%ax, %%es\n"
               "movw %%ax, %%fs\n"
               "movw %%ax, %%gs"
               :
               : "g" (ds)
               : "ax");
}

void			test_ia32_event_context_02(void)
{
  static t_test_ctx*	ctx1;
  static t_test_ctx*	ctx2;
  static t_uint32	esp;
  static t_ia32_segment	seg;

  TEST_ENTER();

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;

  if (ia32_gdt_add_segment(NULL, 14, seg) != ERROR_NONE)
    TEST_ERROR("[ia32_gdt_add_segment] error\n");

  // XXX
  ia32_gdt_dump(IA32_GDT_CURRENT);

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ia32_event_context_02_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("pushl %%eax\n"
               "pushl %%ebx\n"
               "pushl %%ecx\n"
               "pushl %%edx\n"
               "pushl %%esi\n"
               "pushl %%edi\n"
               "pushl %%ebp\n"
               "pushl %%cs\n"
               "pushl %%ds\n"
               "pushl %%es\n"
               "pushl %%fs\n"
               "pushl %%gs\n"
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
               "pushl %%cs\n"
               "pushl %%ds\n"
               "pushl %%es\n"
               "pushl %%fs\n"
               "pushl %%gs\n"
               "movl %%esp, %1"
               : "=m" (ctx1), "=m" (ctx2), "=m" (esp));

  if (event_release(3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  if (thrown != 1)
    TEST_ERROR("the triggered exception has not been caught\n");

  if (esp != (t_uint32)ctx1)
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

  printf("[XXX] 0x%x :: 0x%x (0x%x)\n", ctx1, ctx2, esp);

  printf("[XXX] %u %u %u %u %u :: %u %u %u %u %u\n",
	 ctx1->cs, ctx1->ds, ctx1->es, ctx1->fs, ctx1->gs,
	 ctx2->cs, ctx2->ds, ctx2->es, ctx2->fs, ctx2->gs);

  if (ctx1->cs != ctx2->cs)
    TEST_ERROR("the CS register is different\n");
  if (ctx1->ds != ctx2->ds)
    TEST_ERROR("the DS register is different\n");
  if (ctx1->es != ctx2->es)
    TEST_ERROR("the ES register is different\n");
  if (ctx1->fs != ctx2->fs)
    TEST_ERROR("the FS register is different\n");
  if (ctx1->gs != ctx2->gs)
    TEST_ERROR("the GS register is different\n");

  TEST_LEAVE();
}
