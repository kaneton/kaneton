/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/libs/libia32/time/01/01.c
 *
 * created       renaud voltz   [mon mar 13 14:14:18 2006]
 * updated       renaud voltz   [mon mar 13 14:14:18 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * check pit 8259 settings.
 */

void			check_time_01(void)
{
  t_uint8		status;;

  LIBIA32_TEST_ENTER;

  OUTB(0x43, 0xe2);

  INB(0x40, status);

  printf("pit 8254 settings:\n");

  if (!(status & 0x1))
    printf("  bcd: off\n");
  else
    printf("  error: bcd should not be set\n");

  if (((status >> 1) & 0x7) == 0x2)
    printf("  operating mode: rate\n");
  else
    printf("  error: operating mode should be set to rate\n");

  if (((status >> 4) & 0x3) == 0x3)
    printf("  access mode: lobyte/hibyte\n");
  else
    printf("  error: access mode should be set to lobyte/hibyte\n");

  LIBIA32_TEST_LEAVE;
}
