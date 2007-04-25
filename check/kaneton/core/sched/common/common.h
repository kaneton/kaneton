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

extern i_task ktask;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../simple_01/simple_01.c
 */

/*
 * common.c
 */

void	check_time_common(void);

t_uint32 check_cmos_sec(void);

void	check_time_tests(void);

unsigned long check_rtc_ticks(void);

void check_rtc_init(void);

int		check_thread_create(i_task		task,
				    t_prior		prior,
				    t_vaddr		entry,
				    i_thread		*thread);


/*
 * ../simple_01/simple_01.c
 */

void		check_sched_simple_01(void);


/*
 * eop
 */

#endif /* !CHECK_TIME_COMMON_H_ */
