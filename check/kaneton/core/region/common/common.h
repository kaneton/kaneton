/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/region/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [thu jul 20 19:10:46 2006]
 */

#ifndef CHECK_REGION_COMMON_H_
# define CHECK_REGION_COMMON_H_

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
 *      ../02/02.c
 *      ../03/03.c
 *      ../04/04.c
 *      ../05/05.c
 *      ../06/06.c
 */

/*
 * common.c
 */

void	check_region_common(void);

void	check_region_tests(void);


/*
 * ../01/01.c
 */

void		check_region_01(void);


/*
 * ../02/02.c
 */

void		check_region_02(void);


/*
 * ../03/03.c
 */

void		check_region_03(void);


/*
 * ../04/04.c
 */

void		check_region_04(void);


/*
 * ../05/05.c
 */

void		check_region_05(void);


/*
 * ../06/06.c
 */

void		check_region_06(void);


/*
 * eop
 */

#endif /* !CHECK_REGION_COMMON_H_ */
