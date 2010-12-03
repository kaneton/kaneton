/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...ries/kaneton.STABLE/test/engine/engine.h
 *
 * created       julien quintard   [sun oct 17 15:43:58 2010]
 * updated       julien quintard   [wed dec  1 23:36:36 2010]
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

/*
 * this macro-function is called at the test entrace.
 */

#define TEST_ENTER()

/*
 * this macro-function can be called multiple times in order to transmit
 * the test system a signature string.
 *
 * this string ensures that the test has reached a certain point. these
 * signatures have been introduced in order to limit cheating.
 *
 * note that the signature value is communicated over serial to prevent
 * students to retrieve those values through the debugging output.
 */

#define TEST_SIGNATURE(_signature_)					\
  {									\
    module_call(test, test_issue, "[signature] " #_signature_);		\
  }

/*
 * this macro-function is called whenever an error occurs in a test.
 *
 * the macro-function both displays the report, should the report module
 * be activated, and prints a formatted string 'explaining' the error,
 * though, most of the time, the string only indicates in which function
 * the error occured.
 */

#define TEST_ERROR(_format_, _arguments_...)				\
  do									\
    {									\
      module_call(report, report_dump);					\
									\
      printf(_format_ "\n", ##_arguments_);				\
									\
      TEST_LEAVE();							\
    } while (0)

/*
 * this macro-function is equivalent to the one above except that this
 * one must be used inside threads.
 */

#define TEST_HANG(_format_, _arguments_...)				\
  do									\
    {									\
      module_call(report, report_dump);					\
									\
      printf(_format_ "\n", ##_arguments_);				\
									\
      while (1)								\
	;								\
    } while (0)

/*
 * this macro-function is called whenever the test reaches the end
 * without a problem.
 */

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
