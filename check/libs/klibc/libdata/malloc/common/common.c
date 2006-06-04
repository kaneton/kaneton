/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:17:39 2005]
 * updated       matthieu bucchianeri   [sun jun  4 19:38:42 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_malloc_common(void)
{
  /*
   * nothing here since this test is only for injecting code for next tests.
   */
}

/*
 * stars ALL malloc tests
 */

void	check_malloc_tests(void)
{
  check_malloc_common();
  check_malloc_01();
  check_malloc_02();
  check_malloc_03();
  check_malloc_04();
  check_malloc_05();
  /* XXX */
}

/*
 * initialise memory with generated data.
 */

void		*check_init_memory(char *s, size_t sz)
{
  size_t	i;

  if (!s)
    {
      printf("NULL pointer\n");
      return NULL;
    }
  for (i = 0; i < sz; ++i)
    s[i] = 'a' + (i % 26);
  return s;
}

/*
 * checks the alignment and the integrity of memory chunk.
 * the block must have been initialised with previous function.
 */

void		check_memory(char *s, size_t sz)
{
  size_t	i;

  if (!s)
    {
      printf("NULL pointer\n");
      return;
    }
  if ((unsigned long)s % sizeof (void *))
    printf("bad alignment\n");
  for (i = 0; i < sz; ++i)
    if (s[i] != (char)('a' + (i % 26)))
      printf("integrity error\n");
}
