/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/task/arch_common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sun jul 30 18:19:18 2006]
 */

#ifndef CHECK_TASK_COMMON_H_
# define CHECK_TASK_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../../../check/common/common.h"

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
 *      ../arch_01/01.c
 */

/*
 * common.c
 */

void	check_task_arch_common(void);

void	check_task_arch_tests(void);


/*
 * ../arch_01/01.c
 */

void		check_task_arch_01(void);


/*
 * eop
 */

#endif /* !CHECK_TASK_COMMON_H_ */
