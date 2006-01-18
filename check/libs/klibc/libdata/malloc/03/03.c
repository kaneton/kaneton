/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libdata/malloc/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:23:55 2005]
 */

#include <klibc.h>
#include "../common/common.h"

#define RESIZE(Array,Sz)						\
  (Array) = realloc((Array), (Sz) * sizeof(int))

/*
 * manipulating an array with malloc and realloc.
 */

void		check_malloc_03(void)
{
  int		*array = NULL;
  int		i;

  TEST_ENTER;

  RESIZE(array, 10);

  if (array)
    {
      for (i = 0; i < 10; i++)
	array[i] = i;

      RESIZE(array, 20);

      for (i = 0; i < 10; i++)
	if (array[i] != i)
	  printf("error integrity\n");

      for (i = 10; i < 20; i++)
	array[i] = i * 2;

      RESIZE(array, 15);

      for (i = 0; i < 10; i++)
	if (array[i] != i)
	  printf("error integrity\n");
      for (i = 10; i < 15; i++)
	if (array[i] != 2 * i)
	  printf("error integrity\n");

      free(array);
    }
  else
    printf("error realloc()\n");

  TEST_LEAVE;
}
