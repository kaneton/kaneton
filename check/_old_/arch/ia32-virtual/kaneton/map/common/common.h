/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/map/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sun apr  2 19:01:10 2006]
 */

#ifndef CHECK_MAP_COMMON_H_
# define CHECK_MAP_COMMON_H_

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
 *      ../02/02.c
 */

/*
 * common.c
 */

void	check_map_common(void);

void	check_map_tests(void);


/*
 * ../01/01.c
 */

void		check_map_01(void);


/*
 * ../02/02.c
 */

void		check_map_02(void);


/*
 * eop
 */

#endif /* !CHECK_MAP_COMMON_H_ */
