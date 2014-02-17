/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/random.c
 *
 * created       julien quintard   [fri dec 10 14:03:56 2010]
 * updated       julien quintard   [fri dec 10 14:48:09 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

t_uint64		_library_random_seed;

/*
 * ---------- functions -------------------------------------------------------
 */

void			random_seed(void)
{
  s_clock		clock;
  t_uint64		rounds;
  t_uint64		round;

  assert(clock_current(&clock) == STATUS_OK);

  _library_random_seed = CLOCK_UNIQUE(&clock);

  rounds = random_generate() % 32;

  for (round = 0; round < rounds; round++)
    random_generate();
}

unsigned long		random_generate(void)
{
  _library_random_seed =
    (1103515245 * _library_random_seed + 12345) % 18446744073709551615ULL;

  return (_library_random_seed);
}
