/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/task/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [tue apr  4 01:07:40 2006]
 */

#ifndef CHECK_TASK_COMMON_H_
# define CHECK_TASK_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../../check/common/common.h"

# undef TEST_ENTER

# define TEST_ENTER							\
  printf("Test %s\n", __FUNCTION__);

# undef TEST_LEAVE

# define TEST_LEAVE							\
  printf("%s done.\n", __FUNCTION__);					\
  return

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 */

/*
 * common.c
 */

void	check_task_common(void);

void	check_task_tests(void);


/*
 * ../01/01.c
 */

void		check_task_01(void);


/*
 * eop
 */

#endif /* !CHECK_TASK_COMMON_H_ */
