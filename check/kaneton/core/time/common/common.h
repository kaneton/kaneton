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

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../multiple_reserve_01/multiple_reserve_01.c
 *	../release_01/release_01.c
 *	../repeat_01/repeat_01.c
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
 * ../release_01/release_01.c
 */

void		check_time_release_01(void);


/*
 * ../repeat_01/repeat_01.c
 */

void		check_time_repeat_01(void);


/*
 * ../simple_reserve_01/simple_reserve_01.c
 */

void		check_time_simple_reserve_01(void);


/*
 * eop
 */

#endif /* !CHECK_TIME_COMMON_H_ */
