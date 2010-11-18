/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/as/translation/translation.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [wed nov 17 08:03:25 2010]
 */

#ifndef TESTS_KANETON_AS_TRANSLATION_TRANSLATION_H
#define TESTS_KANETON_AS_TRANSLATION_TRANSLATION_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../engine/engine.h"

/*
 * ---------- macro functions -------------------------------------------------
 */

#define PHYSICAL(_v_, _p_)						\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    v = (_v_);								\
									\
    if (as_paddr(kasid, v, &p) != ERROR_NONE)				\
      TEST_ERROR("[as_paddr] error\n");					\
    									\
    if (p != (_p_))							\
      TEST_ERROR("the virtual to physical conversion failed\n");	\
  }

#define VIRTUAL(_p_, _v_)						\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    p = (_p_);								\
									\
    if (as_vaddr(kasid, p, &v) != ERROR_NONE)				\
      TEST_ERROR("[as_vaddr] error\n");					\
									\
    if (v != (_v_))							\
      TEST_ERROR("the physical to virtual conversion failed\n");	\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      translation.c
 */

/*
 * translation.c
 */

void			test_as_translation(void);


/*
 * eop
 */

#endif
