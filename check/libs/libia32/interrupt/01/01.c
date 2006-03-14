/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/libs/libia32/interrupt/01/01.c
 *
 * created       renaud voltz   [mon mar 13 14:14:18 2006]
 * updated       renaud voltz   [mon mar 13 14:14:18 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * check idt alignment and size.
 */

void			check_interrupt_01(void)
{
  t_idtr		idtr;

  LIBIA32_TEST_ENTER;

  SIDT(idtr);

  if (idtr.address % 8)
    {
      printf("error: idt base address should be aligned on 8 bytes.\n");
      LIBIA32_TEST_LEAVE;
    }

  if (!idtr.size || idtr.size % sizeof (t_idte))
    {
      printf("error: idt size: %u\n", idtr.size);
      LIBIA32_TEST_LEAVE;
    }

  if (idtr.size > IDT_MAX_ENTRIES * sizeof(t_idte))
    {
      printf("error: idt size is to large.\n"
	     "idt cannot contain more than 256 entries.\n");

      LIBIA32_TEST_LEAVE;
    }

  LIBIA32_TEST_LEAVE;
}
