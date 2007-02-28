#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

void		check_segment_clone_01(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  t_uint8*	buff;
  t_uint32	i;
  o_segment*	o;

  TEST_ENTER();
  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");
  ASSERT(region_reserve(kasid,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  buff = (t_uint8*)(t_uint32)reg;
  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  ASSERT(segment_write(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error writing to segment\n");

  ASSERT(segment_clone(as, seg, &seg) == ERROR_NONE,
	    "error cloning segment\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment after cloning\n");

  ASSERT(o->asid == as, "Bad as field after cloning\n");
  ASSERT(o->segid == seg, "Bad segid field after cloning\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after cloning\n");
  ASSERT(o->address == (t_paddr)seg, "Bad address field after cloning\n");
  ASSERT(o->perms == (PERM_READ | PERM_WRITE), "Bad as field after cloning\n");
  ASSERT(o->size == PAGESZ, "Bad as field after cloning\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  ASSERT(segment_read(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error reading to segment\n");

  for (i = 0; i < PAGESZ; i++)
    {
      ASSERT(buff[i] == (i * 2 + 4) % 256, "bad data after cloning\n");
    }

  TEST_LEAVE_AS(task, as);
  TEST_LEAVE();
}
