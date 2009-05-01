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

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

#undef MASTER_PORT_A
#undef MASTER_PORT_B
#undef SLAVE_PORT_A
#undef SLAVE_PORT_B

#define MASTER_PORT_A           0x20
#define MASTER_PORT_B           0x21
#define SLAVE_PORT_A            0xa0
#define SLAVE_PORT_B            0xa1

static void	check_irq(t_id	id)
{
}

/*
 * XXX
 */

void		check_event_pic_01(void)
{
  t_uint8	mask;

  TEST_ENTER();

  assert(event_reserve(32 + 6,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_irq), 0) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Checking Master PIC\n");
  INB(MASTER_PORT_B, mask);
  assert(((mask & (1 << 6))) == 0,
	 " x Bad PIC mask\n");
  printf(" - Checking Slave PIC\n");
  INB(SLAVE_PORT_B, mask);
  assert(mask == 0xff,
	 " x Bad PIC mask\n");

  TEST_LEAVE();
}
