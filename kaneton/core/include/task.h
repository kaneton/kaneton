/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/task.h
 *
 * created       julien quintard   [wed jun  6 14:27:31 2007]
 * updated       julien quintard   [thu dec  9 15:11:50 2010]
 */

#ifndef CORE_TASK_H
#define CORE_TASK_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>
#include <core/wait.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * classes
 */

#define TASK_CLASS_KERNEL		(1 << 0)
#define TASK_CLASS_DRIVER		(1 << 1)
#define TASK_CLASS_SERVICE		(1 << 2)
#define TASK_CLASS_GUEST		(1 << 3)

/*
 * behaviours
 */

#define TASK_BEHAVIOUR_KERNEL		(1 << 0)
#define TASK_BEHAVIOUR_REALTIME		(1 << 1)
#define TASK_BEHAVIOUR_INTERACTIVE	(1 << 2)
#define TASK_BEHAVIOUR_TIMESHARING	(1 << 3)
#define TASK_BEHAVIOUR_BACKGROUND	(1 << 4)

/*
 * priorities
 */

#define TASK_PRIORITY_KERNEL		230
#define TASK_PRIORITY_KERNEL_HIGH	250
#define TASK_PRIORITY_KERNEL_LOW	210

#define TASK_PRIORITY_REALTIME		190
#define TASK_PRIORITY_REALTIME_HIGH	210
#define TASK_PRIORITY_REALTIME_LOW	170

#define TASK_PRIORITY_INTERACTIVE	150
#define TASK_PRIORITY_INTERACTIVE_HIGH	170
#define TASK_PRIORITY_INTERACTIVE_LOW	130

#define TASK_PRIORITY_TIMESHARING	90
#define TASK_PRIORITY_TIMESHARING_HIGH	130
#define TASK_PRIORITY_TIMESHARING_LOW	50

#define TASK_PRIORITY_BACKGROUND	30
#define TASK_PRIORITY_BACKGROUND_HIGH	50
#define TASK_PRIORITY_BACKGROUND_LOW	10

/*
 * init sizes for the array data structure set
 */

#define TASK_THREADS_INITSZ	0x1
#define TASK_WAITS_INITSZ	0x1

/*
 * the task state.
 */

#define TASK_STATE_START	1
#define TASK_STATE_STOP		2
#define TASK_STATE_BLOCK	3
#define TASK_STATE_ZOMBIE	4
#define TASK_STATE_DEAD		5

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * task object
 */

typedef struct
{
  i_task			id;

  i_cpu				cpu;

  i_task			parent;
  i_set				children;

  t_class			class;
  t_behaviour			behaviour;
  t_priority			priority;

  i_as				as;
  i_set				threads;

  t_state			state;

  i_set				waits;
  s_wait			wait;

  i_set				messages;

  machine_data(o_task);
}				o_task;

/*
 * task manager
 */

typedef struct
{
  o_id				id;

  i_set				tasks;

  machine_data(m_task);
}				m_task;

/*
 * the task architecture dependent interface
 */

typedef struct
{
  t_error			(*task_show)(i_task);
  t_error			(*task_reserve)(t_class,
						t_behaviour,
						t_priority,
						i_task*);
  t_error			(*task_release)(i_task);
  t_error			(*task_priority)(i_task,
						 t_priority);
  t_error			(*task_start)(i_task);
  t_error			(*task_stop)(i_task);
  t_error			(*task_block)(i_task);
  t_error			(*task_exit)(i_task,
					     t_value);
  t_error			(*task_initialize)(void);
  t_error			(*task_clean)(void);
}				d_task;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/task/task.c
 */

/*
 * ../../core/task/task.c
 */

t_error			task_show(i_task			id);

t_error			task_dump(void);

t_error			task_reserve(t_class			class,
				     t_behaviour		behav,
				     t_priority			prior,
				     i_task*			id);

t_error			task_release(i_task			id);

t_error			task_priority(i_task			id,
				      t_priority		prior);

t_error			task_start(i_task			id);

t_error			task_stop(i_task			id);

t_error			task_block(i_task			id);

t_error			task_exit(i_task			id,
				  t_value			value);

t_error			task_wait(i_task			id,
				  t_options			opts,
				  s_wait*			wait);

t_error			task_current(i_task*			task);

t_error			task_exist(i_task			id);

t_error			task_get(i_task				id,
				 o_task**			o);

t_error			task_initialize(void);

t_error			task_clean(void);


/*
 * eop
 */

#endif
