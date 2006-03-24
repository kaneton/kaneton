/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [thu mar 23 10:37:06 2006]
 */

#ifndef CHECK_AS_COMMON_H_
# define CHECK_AS_COMMON_H_

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


#define MY_ASSERT(Test,Error)						\
  if (!(Test))								\
    {									\
      printf(Error);							\
      TEST_LEAVE;							\
    }

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
 *      ../07/07.c
 */

/*
 * common.c
 */

void	check_as_common(void);

void	check_as_tests(void);


/*
 * ../01/01.c
 */

void		check_as_01(void);


/*
 * ../02/02.c
 */

void		check_as_02(void);


/*
 * ../03/03.c
 */

void		check_as_03(void);


/*
 * ../04/04.c
 */

void		check_as_04(void);


/*
 * ../05/05.c
 */

void		check_as_05(void);


/*
 * ../06/06.c
 */

void		check_as_06(void);


/*
 * ../07/07.c
 */

void		check_as_07(void);


/*
 * eop
 */

#endif /* !CHECK_AS_COMMON_H_ */
