/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [sun jun  4 19:54:41 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * simple test: allocate some memory, check its validity and write/read.
 */

void	check_malloc_01(void)
{
  void	*p;

  TEST_ENTER(1);

  p = check_init_memory(malloc(42), 42);
  check_memory(p, 42);
  free(p);

  TEST_LEAVE;
}
