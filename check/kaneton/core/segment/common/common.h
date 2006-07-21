/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [thu jul 20 19:04:33 2006]
 */

#ifndef CHECK_SEGMENT_COMMON_H_
# define CHECK_SEGMENT_COMMON_H_

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
 *      ../07/07.c
 *      ../08/08.c
 *      ../09/09.c
 *      ../10/10.c
 *      ../11/11.c
 *      ../12/12.c
 */

/*
 * common.c
 */

void	check_segment_common(void);

void	check_segment_tests(void);


/*
 * ../01/01.c
 */

void		check_segment_01(void);


/*
 * ../02/02.c
 */

void		check_segment_02(void);


/*
 * ../03/03.c
 */

void		check_segment_03(void);


/*
 * ../04/04.c
 */

void		check_segment_04(void);


/*
 * ../05/05.c
 */

void		check_segment_05(void);


/*
 * ../06/06.c
 */

void		check_segment_06(void);


/*
 * ../07/07.c
 */

void		check_segment_07(void);


/*
 * ../08/08.c
 */

void		check_segment_08(void);


/*
 * ../09/09.c
 */

void		check_segment_09(void);


/*
 * ../10/10.c
 */

void		check_segment_10(void);


/*
 * ../11/11.c
 */

void		check_segment_11(void);


/*
 * ../12/12.c
 */

void		check_segment_12(void);


/*
 * eop
 */

#endif /* !CHECK_SEGMENT_COMMON_H_ */
