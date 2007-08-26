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

#ifndef CHECK_TIME_COMMON_H_
# define CHECK_TIME_COMMON_H_

# include <libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../data_01/data_01.c
 *	../message_01/message_01.c
 *	../multiple_reserve_01/multiple_reserve_01.c
 *	../precision_01/precision_01.c
 *	../precision_02/precision_02.c
 *	../precision_03/precision_03.c
 *	../release_01/release_01.c
 *	../repeat_01/repeat_01.c
 *	../repeat_02/repeat_02.c
 *	../simple_reserve_01/simple_reserve_01.c
 */

/*
 * common.c
 */

void	check_time_common(void);

t_uint32 check_cmos_sec(void);

void	check_time_tests(void);

unsigned long check_rtc_ticks(void);

void check_rtc_init(void);


/*
 * ../multiple_reserve_01/multiple_reserve_01.c
 */

void		check_time_multiple_reserve_01(void);


/*
 * ../precision_01/precision_01.c
 */

void		check_time_precision_01(void);


/*
 * ../precision_02/precision_02.c
 */

void		check_time_precision_02(void);


/*
 * ../precision_03/precision_03.c
 */

void		check_time_precision_03(void);


/*
 * ../release_01/release_01.c
 */

void		check_time_release_01(void);


/*
 * ../repeat_01/repeat_01.c
 */

void		check_time_repeat_01(void);


/*
 * ../repeat_02/repeat_02.c
 */

void		check_time_repeat_02(void);


/*
 * ../simple_reserve_01/simple_reserve_01.c
 */

void		check_time_simple_reserve_01(void);


/*
 * eop
 */

#endif /* !CHECK_TIME_COMMON_H_ */
