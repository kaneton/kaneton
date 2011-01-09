/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/idt/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [sat jan  8 19:21:02 2011]
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
  as_idt_register	idtr;
  as_idt_entry*		idte;
  int			i;
  t_uint16		cs;

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&cs) != ERROR_OK)
    TEST_ERROR("[architecture_gdt_selector] error");

  if (event_reserve(3,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_idt_02_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  memset(&idtr, 0, sizeof (idtr));

  ARCHITECTURE_SIDT(idtr);

  idte = (void*)idtr.address;

  for (i = 0; i < idtr.size / 8; i++)
    {
      if (!(idte[i].type & ARCHITECTURE_IDT_TYPE_PRESENT))
        continue;

      if (!(idte[i].type & ((1 << 3) | (1 << 2) | (1 << 1))))
        TEST_ERROR("IDT entry %u is neither an interrupt nor a trap gate",
		   i);

      if (idte[i].selector != cs)
        TEST_ERROR("IDT entry %u references an invalidt segment selector",
		   i);
    }

  if (event_release(3) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(acdjif0wv94eg3h34h);

  TEST_LEAVE();
}
