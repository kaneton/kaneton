/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sat oct 14 17:00:54 2006]
 */

#ifndef CHECK_THREAD_COMMON_H_
# define CHECK_THREAD_COMMON_H_

# include <libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

extern i_task ktask;

#undef TEST_LEAVE
# define TEST_LEAVE()							\
  STI();								\
  printf("%s done.\n", __FUNCTION__);					\
  return

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../driver_01/driver_01.c
 */

/*
 * common.c
 */

void	check_task_common(void);

void	check_task_tests(void);


/*
 * ../driver_01/driver_01.c
 */

void		check_task_driver_01(void);


/*
 * eop
 */

#endif /* !CHECK_THREAD_COMMON_H_ */
