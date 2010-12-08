/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/core/as/translation/translation.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [wed nov 24 09:29:37 2010]
 */

#ifndef TESTS_KANETON_CORE_AS_TRANSLATION_TRANSLATION_H
#define TESTS_KANETON_CORE_AS_TRANSLATION_TRANSLATION_H

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
    if (as_paddr(_kernel->as, v, &p) != ERROR_OK)			\
      TEST_ERROR("[as_paddr] error");					\
    									\
    if (p != (_p_))							\
      TEST_ERROR("the virtual to physical conversion failed");	\
  }

#define VIRTUAL(_p_, _v_)						\
  {									\
    t_vaddr	v;							\
    t_paddr	p;							\
									\
    p = (_p_);								\
									\
    if (as_vaddr(_kernel->as, p, &v) != ERROR_OK)			\
      TEST_ERROR("[as_vaddr] error");					\
									\
    if (v != (_v_))							\
      TEST_ERROR("the physical to virtual conversion failed");	\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      translation.c
 */

/*
 * translation.c
 */

void			test_core_as_translation(void);


/*
 * eop
 */

#endif