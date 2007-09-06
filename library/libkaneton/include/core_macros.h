/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...library/libkaneton/include/core_macros.h
 *
 * created       matthieu bucchianeri   [sun aug  5 23:37:49 2007]
 * updated       matthieu bucchianeri   [thu sep  6 21:35:24 2007]
 */

#ifndef LIBKANETON_CORE_MACROS_H
#define LIBKANETON_CORE_MACROS_H

#define PERM_READ		(1 << 0)
#define PERM_WRITE		(1 << 1)
#define PERM_EXEC		(1 << 2)

#define EVENT_FUNCTION		0
#define EVENT_MESSAGE		1

#define EVENT_TASK(_task_)						\
  ((u_event_handler)(i_task)(_task_))

#define MAP_OPT_NONE		0
#define MAP_OPT_FORCE		(1 << 0)
#define MAP_OPT_USER		(0 << 1)
#define MAP_OPT_PRIVILEGED	(1 << 1)

#define MESSAGE_TYPE_INTERFACE		0
#define MESSAGE_TYPE_EVENT		1
#define MESSAGE_TYPE_TIMER		2

#define REGION_OPT_NONE		0
#define REGION_OPT_FORCE	(1 << 0)
#define REGION_OPT_USER		(0 << 1)
#define REGION_OPT_PRIVILEGED	(1 << 1)
#define REGION_OPT_LOCAL	(0 << 2)
#define REGION_OPT_GLOBAL	(1 << 2)

#define SCHEDULER_STATE_RUN		0
#define SCHEDULER_STATE_STOP		1
#define SCHEDULER_STATE_ZOMBIE		2
#define SCHEDULER_STATE_BLOCK		3

#define SEGMENT_TYPE_MEMORY	(1 << 0)
#define SEGMENT_TYPE_CATCH	(1 << 1)
#define SEGMENT_TYPE_SYSTEM	(1 << 2)

#define TASK_CLASS_CORE		(1 << 0)
#define TASK_CLASS_DRIVER	(1 << 1)
#define TASK_CLASS_SERVICE	(1 << 2)
#define TASK_CLASS_PROGRAM	(1 << 3)

#define TASK_BEHAV_CORE		(1 << 0)
#define TASK_BEHAV_REALTIME	(1 << 1)
#define TASK_BEHAV_INTERACTIVE	(1 << 2)
#define TASK_BEHAV_TIMESHARING	(1 << 3)
#define TASK_BEHAV_BACKGROUND	(1 << 4)

#define TASK_PRIOR_CORE		230
#define TASK_HPRIOR_CORE	250
#define TASK_LPRIOR_CORE	210

#define TASK_PRIOR_REALTIME	190
#define TASK_HPRIOR_REALTIME	210
#define TASK_LPRIOR_REALTIME	170

#define TASK_PRIOR_INTERACTIVE	150
#define TASK_HPRIOR_INTERACTIVE	170
#define TASK_LPRIOR_INTERACTIVE	130

#define TASK_PRIOR_TIMESHARING	90
#define TASK_HPRIOR_TIMESHARING	130
#define TASK_LPRIOR_TIMESHARING	50

#define TASK_PRIOR_BACKGROUND	30
#define TASK_HPRIOR_BACKGROUND	50
#define TASK_LPRIOR_BACKGROUND	10

#define THREAD_PRIOR		130
#define THREAD_HPRIOR		250
#define THREAD_LPRIOR		10

#define TIMER_FUNCTION		0
#define TIMER_MESSAGE		1

#define TIMER_TASK(_task_)						\
  ((u_timer_handler)(i_task)(_task_))

#define TIMER_REPEAT_DISABLE    0
#define TIMER_REPEAT_ENABLE     1

#define PAGESZ			4096

#endif
