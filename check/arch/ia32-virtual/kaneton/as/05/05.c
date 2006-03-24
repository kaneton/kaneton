/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/05/05.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [fri mar 24 15:44:10 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

# define VTOP(V,P)							\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    v = (V);								\
    MY_ASSERT(as_paddr(as, v, &p) == ERROR_NONE, "error in as_paddr\n");\
    MY_ASSERT(p == (P), "bad vtop conversion\n");			\
  }

# define PTOV(P,V)							\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    p = (P);								\
    MY_ASSERT(as_vaddr(as, p, &v) == ERROR_NONE, "error in as_vaddr\n");\
    MY_ASSERT(v == (V), "bad ptov conversion\n");			\
  }

void		check_as_05(void)
{
  t_tskid	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  t_vaddr	v;
  t_paddr	p;
  t_uint32	i;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    2048 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   4 * PAGESZ,
			   REGION_OPT_NONE,
			   0,
			   8 * PAGESZ,
			   &reg) == ERROR_NONE,
	    "error reserving region\n");

  /* V to P */

  VTOP((t_vaddr)reg, (t_paddr)seg + 4 * PAGESZ);
  VTOP((t_vaddr)reg + 1, (t_paddr)seg + 4 * PAGESZ + 1);
  VTOP((t_vaddr)reg + 2 * PAGESZ + 1234, (t_paddr)seg + 6 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    VTOP((t_vaddr)reg + i, (t_paddr)seg + 4 * PAGESZ + i);

  v = (t_vaddr)reg + 8 * PAGESZ;
  MY_ASSERT(as_paddr(as, v, &p) != ERROR_NONE,
	    "conversion of unmapped address (higher)\n");
  v = (t_vaddr)reg + 8 * PAGESZ + 1;
  MY_ASSERT(as_paddr(as, v, &p) != ERROR_NONE,
	    "conversion of unmapped address (higher)\n");
  v = (t_vaddr)reg - 1;
  MY_ASSERT(as_paddr(as, v, &p) != ERROR_NONE,
	    "conversion of unmapped address (lower)\n");

  /* P to V */

  PTOV((t_paddr)seg + 4 * PAGESZ, (t_vaddr)reg);
  PTOV((t_paddr)seg + 4 * PAGESZ + 1, (t_vaddr)reg + 1);
  PTOV((t_paddr)seg + 6 * PAGESZ + 1234, (t_vaddr)reg + 2 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    PTOV((t_paddr)seg + 4 * PAGESZ + i, (t_vaddr)reg + i);

  p = (t_paddr)seg  + 12 * PAGESZ;
  MY_ASSERT(as_vaddr(as, p, &v) != ERROR_NONE,
	    "conversion of unmapped address (lower)\n");
  p = (t_paddr)seg  + 12 * PAGESZ + 1;
  MY_ASSERT(as_vaddr(as, p, &v) != ERROR_NONE,
	    "conversion of unmapped address (lower)\n");
  p = (t_paddr)seg  + 4 * PAGESZ - 1;
  MY_ASSERT(as_vaddr(as, p, &v) != ERROR_NONE,
	    "conversion of unmapped address (lower)\n");


  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
