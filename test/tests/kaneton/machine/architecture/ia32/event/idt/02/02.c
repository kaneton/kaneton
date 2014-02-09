/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/educational/event/idt/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [thu mar  3 17:42:25 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */
 
void			test_architecture_event_idt_02_handler(t_id	id)
{
}

void			test_architecture_event_idt_02(void)
{
  as_idtr		idtr;
  at_idte*		idte;
  int			i;
  t_uint16		cs;

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&cs) != STATUS_OK)
    TEST_ERROR("[architecture_gdt_selector] error");

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_idt_02_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  memset(&idtr, 0, sizeof (idtr));

  ARCHITECTURE_SIDT(idtr);

  idte = (void*)idtr.address;

  for (i = 0; i < idtr.size / 8; i++)
    {
      if (!(idte[i] & ARCHITECTURE_IDTE_PRESENT))
        continue;

      if (!(idte[i] &
	    ((1LL << 43) | (1LL << 42) | (1LL << 41) | (1LL << 40))))
        TEST_ERROR("IDT entry %u is neither an interrupt nor a trap gate",
		   i);

      if (ARCHITECTURE_IDTE_SELECTOR_GET(idte[i]) != cs)
        TEST_ERROR("IDT entry %u references an invalid segment selector",
		   i);
    }

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_BP) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(acdjif0wv94eg3h34h);

  TEST_LEAVE();
}
