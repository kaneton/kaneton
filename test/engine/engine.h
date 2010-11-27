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
 * updated       julien quintard   [sat nov 27 22:57:34 2010]
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
      module_call(report, report_dump);					\
									\
      printf(_format_ "\n", ##_arguments_);				\
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
