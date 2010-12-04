/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/idt/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 12:27:31 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_idt_01(void)
{
  t_ia32_idt_register	idtr;

  TEST_ENTER();

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  if (idtr.size != 2048)
    TEST_ERROR("the IDT is not 256 entries wide");

  if ((idtr.address % 8) != 0)
    TEST_ERROR("the IDT is not aligned on a 8-byte address");

  TEST_SIGNATURE(0cvwe0fkwiofj2ig);

  TEST_LEAVE();
}
