/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:21:09 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "init.h"

void	check_set_init(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
  printf("init done.\n");
}

/*
 * starts ALL tests related to sets
 */

void	check_set_tests(void)
{
  check_set_init();
  check_set_array_01();
  check_set_array_02();
  check_set_array_03();
  check_set_array_04();
  check_set_array_05();
  check_set_array_06();
  check_set_array_07();
  check_set_array_08();
  check_set_array_09();
  check_set_array_10();
  check_set_array_11();
  check_set_array_12();
  check_set_ll_01();
  check_set_ll_02();
  check_set_ll_03();
  /* XXX continue tests */
}

/*
 * adds many elements at a time
 */

void		check_many_add(t_setid setid, int nb, ...)
{
  va_list	l;
  int		i;
  t_id		id;

  va_start(l, nb);

  for (i = 0; i < nb; ++i)
    {
      id = va_arg(l, t_id);
      if (set_add(setid, &id) != ERROR_NONE)
	printf("error in set_add()\n");
    }

  va_end(l);
}

/*
 * removes many elements at a time
 */

void		check_many_remove(t_setid setid, int nb, ...)
{
  va_list	l;
  int		i;
  t_id		id;

  va_start(l, nb);

  for (i = 0; i < nb; ++i)
    {
      id = va_arg(l, t_id);
      if (set_remove(setid, id) != ERROR_NONE)
	printf("error in set_remove()\n");
    }

  va_end(l);
}

/*
 * displays a set without garbages
 *
 * XXX a revoir car on utilise les iterators, donc si le gars ne gere
 *     pas il passe aucun tests.
 */

void		check_display_set(t_setid setid, t_dispdata disp)
{
  t_iterator	it;
  t_state	state;
  t_setsz	sz;
  void*		pdata;
  char		space[2];

  set_size(setid, &sz);
  printf("display set of %qd elements\n", sz);

  space[0] = space[1] = 0;
  set_foreach(SET_OPT_FORWARD, setid, &it, state)
    {
      if (set_object(setid, it, (void**)&pdata) != ERROR_NONE)
	printf("cannot display set !\n");
      if (!disp)
	printf("%s%qd", space, *((t_setid*)pdata));
      else
	disp(pdata);
      space[0] = ' ';
    }
  printf("\n");
}
