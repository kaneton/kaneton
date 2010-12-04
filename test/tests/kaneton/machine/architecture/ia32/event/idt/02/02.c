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
 * updated       julien quintard   [sat dec  4 12:27:41 2010]
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
  t_ia32_idt_register	idtr;
  t_ia32_idt_entry*	idte;
  int			i;
  t_uint16		cs;

  if (ia32_gdt_build_selector(IA32_PMODE_GDT_KERNEL_CS,
			      ia32_privilege_kernel,
			      &cs) != ERROR_OK)
    TEST_ERROR("[ia32_gdt_build_selector] error");

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_idt_02_handler),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  idte = (void*)idtr.address;

  for (i = 0; i < idtr.size / 8; i++)
    {
      if (!(idte[i].type & IA32_DESCRIPTOR_PRESENT))
        continue;

      if (!(idte[i].type & ((1 << 3) | (1 << 2) | (1 << 1))))
        TEST_ERROR("IDT entry %u is neither an interrupt nor a trap gate",
		   i);

      if (idte[i].segsel != cs)
        TEST_ERROR("IDT entry %u references an invalidt segment selector",
		   i);
    }

  if (event_release(3) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(acdjif0wv94eg3h34h);

  TEST_LEAVE();
}
