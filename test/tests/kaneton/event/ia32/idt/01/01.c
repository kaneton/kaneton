/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/event/ia32/idt/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 15 10:37:34 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task		ktask;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_idt_01_handler(t_id		id)
{
}

void			test_event_ia32_idt_01(void)
{
  t_ia32_idtr		idtr;
  t_ia32_idte*		idte;
  int			i;
  t_uint16		cs;

  if (ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_CS,
			      ia32_prvl_supervisor,
			      &cs) != ERROR_NONE)
    TEST_ERROR("[ia32_gdt_build_selector] error\n");

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_idt_01_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  idte = (void*)idtr.address;

  for (i = 0; i < idtr.size / 8; i++)
    {
      if (!(idte[i].type & IA32_DESC_TYPE_PRESENT))
        continue;

      if (!(idte[i].type & ((1 << 3) | (1 << 2) | (1 << 1))))
        TEST_ERROR("IDT entry %u is neither an interrupt nor a trap gate\n",
		   i);

      if (idte[i].segsel != cs)
        TEST_ERROR("IDT entry %u references an invalidt segment selector\n",
		   i);
    }

  if (event_release(3) != ERROR_NONE)
    TEST_ERROR("[event_release] error\n");

  TEST_LEAVE();
}
