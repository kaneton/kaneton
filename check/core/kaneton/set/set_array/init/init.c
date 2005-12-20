#include <klibc.h>
#include <kaneton.h>
#include "init.h"

void	check_set_array_init(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * adds many elements at a time
 */

void		moulinette_many_add(t_setid setid, int nb, ...)
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

void		moulinette_many_remove(t_setid setid, int nb, ...)
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
 * display a set without garbages
 */

void		moulinette_display_set(t_setid setid, t_dispdata disp)
{
  t_iterator	it;
  t_state	state;
  t_setsz	sz;
  void		*pdata;
  char		space[2];

  set_size(setid, &sz);
  printf("display set of %qd elements\n", sz);

  space[0] = space[1] = 0;
  set_foreach(SET_OPT_FORWARD, setid, &it, state)
    {
      if (set_object(setid, it, (void**)&pdata) != ERROR_NONE)
	printf("cannot display set !\n");
      printf("%s%qd", space, *((t_setid*)pdata));
      if (disp)
	disp(pdata);
      space[0] = ' ';
    }
}

/*
 * for leaks tracking
 */

void	moulinette_leaks_init(t_leaks *l)
{
  l->nb_alloc = alloc_nalloc();
  l->nb_free = alloc_nfree();
}

void	moulinette_leaks_display(t_leaks *l)
{
  if ((l->nb_alloc - l->nb_free) != (alloc_nalloc() - alloc_nfree()))
    printf("error: memory leaks detected: %u/%u -> %u/%u\n",
	   l->nb_alloc, l->nb_free, alloc_nalloc(), alloc_nfree());
}
