#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_multiple_reserve_01(void)
{
  i_segment	seg;
  i_region	reg1;
  i_region	reg2;
  i_region	reg3;

  TEST_ENTER();

  ASSERT(segment_reserve(kasid,
			    10 * PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg1) == ERROR_NONE, "error region_reserve 1\n");
  ASSERT(region_reserve(kasid,
			   seg,
			   3 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   2 * PAGESZ,
			   &reg2) == ERROR_NONE, "error region_reserve 2\n");
  ASSERT(region_reserve(kasid,
			   seg,
			   6 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   4 * PAGESZ,
			   &reg3) == ERROR_NONE, "error region_reserve 3\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg1;
  for (; p < (t_uint8*)(t_vaddr)reg1 + 2 * PAGESZ; p++)
    *p = 0xaa;
  p = (t_uint8*)(t_vaddr)reg2;
  for (; p < (t_uint8*)(t_vaddr)reg2 + 2 * PAGESZ; p++)
    *p = 0x55;
  p = (t_uint8*)(t_vaddr)reg3;
  for (; p < (t_uint8*)(t_vaddr)reg3 + 4 * PAGESZ; p++)
    *p = 0x0;

  TEST_LEAVE();
}
