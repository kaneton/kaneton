/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/engine.h
 *
 * created       julien quintard   [sun oct 17 15:43:58 2010]
 * updated       julien quintard   [sun nov 14 21:00:39 2010]
 */

#ifndef TEST_ENGINE_ENGINE_H
#define TEST_ENGINE_ENGINE_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define TEST_ENTER()

#define TEST_ERROR(_format_, _arguments_...)				\
  do									\
    {									\
      printf(_format_, ##_arguments_);					\
									\
      TEST_LEAVE();							\
    } while (0)

#define TEST_LEAVE()							\
  return;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      engine.c
 */

/*
 * engine.c
 */


/*
 * eop
 */

#endif
