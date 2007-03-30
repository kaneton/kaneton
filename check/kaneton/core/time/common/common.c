/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/common/common.c
 *
 * created       matthieu bucchianeri   [wed mar 15 23:11:33 2006]
 * updated       matthieu bucchianeri   [sat oct 14 16:56:32 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_time_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

static unsigned char	bcd2bin(unsigned char c)
{
  unsigned char res = 0;

  unsigned char c1 = (c & 0xF0) >> 4;
  res += 10 * c1;
  c1 = c & 0x0F;
  res += c1;
  return res;
}

/*
 * get seconds from RTC
 */

t_uint32 check_cmos_sec(void)
{
  int i;

  OUTB(0x70, 0);
  INB(0x71, i);
  return bcd2bin(i);
}

/*
 * starts ALL tests related to time manager
 */

void	check_time_tests(void)
{
  check_time_common();
  /* XXX continue tests */
}
