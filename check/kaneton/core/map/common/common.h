/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/map/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [thu jul 20 18:46:52 2006]
 */

#ifndef CHECK_MAP_COMMON_H_
# define CHECK_MAP_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

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

void	check_map_common(void);

void	check_map_tests(void);


/*
 * ../01/01.c
 */

void		check_map_01(void);


/*
 * eop
 */

#endif /* !CHECK_MAP_COMMON_H_ */
