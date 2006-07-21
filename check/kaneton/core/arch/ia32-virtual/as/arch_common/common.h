/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/as/arch_common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [thu jul 20 19:37:12 2006]
 */

#ifndef CHECK_ARCH_AS_COMMON_H_
# define CHECK_ARCH_AS_COMMON_H_

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
 *      ../arch_02/02.c
 */

/*
 * common.c
 */

void	check_as_arch_common(void);

void	check_as_arch_tests(void);


/*
 * ../arch_01/01.c
 */

void			check_as_arch_01(void);


/*
 * ../arch_02/02.c
 */

void		check_as_arch_02(void);


/*
 * eop
 */

#endif /* !CHECK_ARCH_AS_COMMON_H_ */
