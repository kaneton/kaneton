#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_simple_write_01(void)
{
  i_segment	seg;
  i_region	reg;

  TEST_ENTER();

  ASSERT(segment_reserve(kasid,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_FORCE,
			   0x70000000,
			   PAGESZ,
			   &reg) == ERROR_NONE, "error region_reserve\n");

  t_uint8* p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + PAGESZ; p++)
    {
      *p = 0x42;
    }

  TEST_LEAVE();
}
