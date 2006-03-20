/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/check/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [mon mar 20 11:31:21 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_check_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests manually
 */

void	check_tests(void)
{
  check_check_common();
  /*  check_set_tests();
  check_malloc_tests();
  check_printf_tests();
  check_stats_tests();
  check_id_tests();
  check_libia32_tests();
  check_event_tests();*/
  /* XXX continue tests */
  check_as_tests ();
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
