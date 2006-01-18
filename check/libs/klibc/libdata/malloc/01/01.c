/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libdata/malloc/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:24:05 2005]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * simple test: allocate some memory, check its validity and write/read.
 */

void	check_malloc_01(void)
{
  void	*p;

  TEST_ENTER;

  p = check_init_memory(malloc(42), 42);
  check_memory(p, 42);
  free(p);

  TEST_LEAVE;
}
