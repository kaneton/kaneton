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

#undef TEST_LEAVE
# define TEST_LEAVE()							\
  STI();								\
  printf("%s done.\n", __FUNCTION__);					\
  return

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../simple_01/simple_01.c
 *	../simple_02/simple_02.c
 *	../simple_03/simple_03.c
 *	../medium_01/medium_01.c
 *	../medium_02/medium_02.c
 *	../medium_03/medium_03.c
 *	../flood_01/flood_01.c
 *	../flood_02/flood_02.c
 *	../priority_01/priority_01.c
 *	../priority_02/priority_02.c
 *	../context_01/context_01.c
 *	../context_02/context_02.c
 *	../context_03/context_03.c

 */

/*
 * common.c
 */

void	check_time_common(void);

t_uint32 check_cmos_sec(void);

void	check_time_tests(void);

unsigned long check_rtc_ticks(void);

void check_rtc_init(void);

int		check_task_create(t_class		class,
				  i_task		*task);

int		check_thread_create(i_task		task,
				    t_prior		prior,
				    t_vaddr		entry,
				    i_thread		*thread);


/*
 * ../simple_01/simple_01.c
 */

void		check_sched_simple_01(void);


/*
 * ../simple_02/simple_02.c
 */

void		check_sched_simple_02(void);


/*
 * ../simple_03/simple_03.c
 */

void		check_sched_simple_03(void);


/*
 * ../medium_01/medium_01.c
 */

void		check_sched_medium_01(void);


/*
 * ../medium_02/medium_02.c
 */

void		check_sched_medium_02(void);


/*
 * ../medium_03/medium_03.c
 */

void		check_sched_medium_03(void);


/*
 * ../flood_01/flood_01.c
 */

void		check_sched_flood_01(void);


/*
 * ../flood_02/flood_02.c
 */

void		check_sched_flood_02(void);


/*
 * ../priority_01/priority_01.c
 */

void		check_sched_priority_01(void);


/*
 * ../priority_02/priority_02.c
 */

void		check_sched_priority_02(void);


/*
 * ../context_01/context_01.c
 */

void		check_sched_context_01(void);


/*
 * ../context_02/context_02.c
 */

void		check_sched_context_02(void);


/*
 * ../context_03/context_03.c
 */

void		check_sched_context_03(void);


/*
 * eop
 */

#endif /* !CHECK_TIME_COMMON_H_ */
