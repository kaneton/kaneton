#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_asid kasid;

void		check_segment_copy_01(void)
{
  i_task	task;
  o_segment*	o;
  t_asid	as;
  t_segid	seg;
  t_segid	seg_ref;
  t_regid	reg;
  t_uint32	i;
  t_uint8*	buff;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE, &seg_ref) == ERROR_NONE,
         "Error reserving segment\n");
  ASSERT(region_reserve(kasid,
			   seg_ref,
			   0,
			   REGION_OPT_NONE,
			   0,
			   PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");
  ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

   buff = (t_uint8*)(t_uint32)reg;

     for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  ASSERT(segment_copy(seg, 0, seg_ref, 0,
			 PAGESZ) == ERROR_NONE,
	    "error in copy\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  ASSERT(segment_read(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error reading\n");

  for (i = 0; i < PAGESZ; i++)
    {
      ASSERT(buff[i] == (i * 4 - 1) % 256, "bad data\n");
    }

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
