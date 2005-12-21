/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [tue dec 20 23:11:34 2005]
 */

#include <klibc.h>
#include "init.h"

void	check_init(void);
void	check_tests(void);
void	check_set_tests(void);
void	check_malloc_tests(void);

void	check_init(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
  printf("init done.\n");
}

/*
 * starts ALL tests manually
 */

void	check_tests(void)
{
  check_init();
  check_set_tests();
  check_malloc_tests();
//  check_stats_01();
//  check_stats_02();
//  check_stats_03();
  /* XXX continue tests */
}

/*
 * for leaks tracking
 */

void	check_leaks_init(t_leaks*	l)
{
  l->nb_alloc = alloc_nalloc();
  l->nb_free = alloc_nfree();
}

void	check_leaks_display(t_leaks*	l)
{
  if ((l->nb_alloc - l->nb_free) != (alloc_nalloc() - alloc_nfree()))
    printf("error: memory leaks detected: %u/%u -> %u/%u\n",
	   l->nb_alloc, l->nb_free, alloc_nalloc(), alloc_nfree());
}
