/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/paddr_vaddr_01/paddr_vaddr_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:31:07 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

# define VTOP(V,P)							\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    v = (V);								\
    assert(as_paddr(as, v, &p) == ERROR_NONE, "error in as_paddr\n");\
    assert(p == (P), "bad vtop conversion\n");			\
  }

# define PTOV(P,V)							\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    p = (P);								\
    assert(as_vaddr(as, p, &v) == ERROR_NONE, "error in as_vaddr\n");\
    assert(v == (V), "bad ptov conversion\n");			\
  }

void		check_as_paddr_vaddr_01(void)
{
  i_task	task;
  i_as	as;
  i_segment	seg;
  i_region	reg;
  t_vaddr	v;
  t_paddr	p;
  t_uint32	i;

  TEST_ENTER();

  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			    2048 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(region_reserve(as,
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

#if 0 /* XXX these test are unsafe since other regions are implicitly mapped */
  v = (t_vaddr)reg + 8 * PAGESZ;
  assert(as_paddr(as, v, &p) != ERROR_NONE,
	    "paddr: conversion of unmapped address (higher)\n");
  v = (t_vaddr)reg + 8 * PAGESZ + 1;
  assert(as_paddr(as, v, &p) != ERROR_NONE,
	    "paddr: conversion of unmapped address (higher)\n");
  v = (t_vaddr)reg - 1;
  assert(as_paddr(as, v, &p) != ERROR_NONE,
	    "paddr: conversion of unmapped address (lower)\n");
#endif

  /* P to V */

  PTOV((t_paddr)seg + 4 * PAGESZ, (t_vaddr)reg);
  PTOV((t_paddr)seg + 4 * PAGESZ + 1, (t_vaddr)reg + 1);
  PTOV((t_paddr)seg + 6 * PAGESZ + 1234, (t_vaddr)reg + 2 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    PTOV((t_paddr)seg + 4 * PAGESZ + i, (t_vaddr)reg + i);

#if 0 /* XXX these test are unsafe since other regions are implicitly mapped */
  p = (t_paddr)seg  + 12 * PAGESZ;
  assert(as_vaddr(as, p, &v) != ERROR_NONE,
	    "vaddr: conversion of unmapped address (higher)\n");
  p = (t_paddr)seg  + 12 * PAGESZ + 1;
  assert(as_vaddr(as, p, &v) != ERROR_NONE,
	    "vaddr: conversion of unmapped address (higher)\n");
  p = (t_paddr)seg  + 4 * PAGESZ - 1;
  assert(as_vaddr(as, p, &v) != ERROR_NONE,
	    "vaddr: conversion of unmapped address (lower)\n");
#endif

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
