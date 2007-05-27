#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_map_simple_reserve_release_01(void)
{
  t_vaddr	addr;
  t_uint32	j;
  t_uint8*	p;

  TEST_ENTER();

  ASSERT(map_reserve(kasid,
		     MAP_OPT_NONE,
		     PAGESZ,
		     PERM_READ | PERM_WRITE,
		     &addr) == ERROR_NONE,
	 "error reserving a map\n");

  for (j = 0, p = (t_uint8*)addr;
       j < PAGESZ;
       p++, j++)
  {
    *p = 0x0d;
    ASSERT(*p == 0x0d, "integrity check failed\n");
  }

  ASSERT(map_release(kasid, addr) == ERROR_NONE,
    "error releasing a map\n");

  TEST_LEAVE();
}
