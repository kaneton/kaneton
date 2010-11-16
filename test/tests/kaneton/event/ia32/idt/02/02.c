/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/event/ia32/idt/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 15 10:44:10 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task		ktask;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_idt_02_handler(t_id		id)
{
}

void			test_event_ia32_idt_02(void)
{
  t_ia32_idtr		idtr;

  TEST_ENTER();

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  if (idtr.size != 2048)
    TEST_ERROR("the IDT is not 256 entries wide\n");

  if ((idtr.address % 8) != 0)
    TEST_ERROR("the IDT is not aligned on a 8-byte address\n");

  TEST_LEAVE();
}
