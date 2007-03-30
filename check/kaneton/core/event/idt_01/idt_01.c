/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

static void	check_int3(t_id	id)
{
}

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_idt_01(void)
{
  struct
  {
    t_uint16			size;
    t_uint32			address;
  } __attribute__((packed)) idtr;
  struct
  {
    t_uint16			offset_00_15;
    t_uint16			segsel;
    t_uint8			reserved;
    t_uint8			type;
    t_uint16			offset_16_31;
  } __attribute__ ((packed)) *idte;
  int				i;
  t_uint16			cs;

  TEST_ENTER();

  gdt_build_selector(PMODE_GDT_CORE_CS, ia32_prvl_supervisor, &cs);

  ASSERT(event_reserve(3,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_int3)) == ERROR_NONE,
	 "cannot event_reserve\n");

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  idte = (void*)idtr.address;

  for (i = 0; i < idtr.size / 8; i++)
    {
      if (!(idte[i].type & DESC_TYPE_PRESENT))
	continue;

      if (!(idte[i].type & ((1 << 3) | (1 << 2) | (1 << 1))))
	printf("IDT entry %d is neither an interrupt nor a trap gate\n", i);

      if (idte[i].segsel != cs)
	printf("IDT entry %d as incorrect segment selector\n", i);
    }

  TEST_LEAVE();
}
