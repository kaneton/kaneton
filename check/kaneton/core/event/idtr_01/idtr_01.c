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

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_idtr_01(void)
{
  struct
  {
    t_uint16			size;
    t_uint32			address;
  } __attribute__((packed)) idtr;

  TEST_ENTER();

  memset(&idtr, 0, sizeof (idtr));

  SIDT(idtr);

  ASSERT(idtr.size == 2048, "IDT is not 256 entries wide\n");

  ASSERT(!(idtr.address % 8), "IDT is not aligned on 8-byte address\n");

  TEST_LEAVE();
}
