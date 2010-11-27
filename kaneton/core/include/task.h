/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.TETON/kaneton/core/include/task.h
 *
 * created       julien quintard   [wed jun  6 14:27:31 2007]
 * updated       julien quintard   [sat nov 27 14:02:29 2010]
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
#define TASK_HPRIORITY_KERNEL		250
#define TASK_LPRIORITY_KERNEL		210

#define TASK_PRIORITY_REALTIME		190
#define TASK_HPRIORITY_REALTIME		210
#define TASK_LPRIORITY_REALTIME		170

#define TASK_PRIORITY_INTERACTIVE	150
#define TASK_HPRIORITY_INTERACTIVE	170
#define TASK_LPRIORITY_INTERACTIVE	130

#define TASK_PRIORITY_TIMESHARING	90
#define TASK_HPRIORITY_TIMESHARING	130
#define TASK_LPRIORITY_TIMESHARING	50

#define TASK_PRIORITY_BACKGROUND	30
#define TASK_HPRIORITY_BACKGROUND	50
#define TASK_LPRIORITY_BACKGROUND	10

/*
 * init sizes for the array data structure set
 */

#define TASK_THREADS_INITSZ	0x1
#define TASK_WAITS_INITSZ	0x1

/*
 * the task state.
 */

#define TASK_STATE_RUN		1
#define TASK_STATE_STOP		2
#define TASK_STATE_ZOMBIE	3
#define TASK_STATE_BLOCK	4

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
  t_wait			wait;

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
 * waitlist
 */

typedef struct
{
  union
  {
    i_task			task;
    i_thread			thread;
  }				u;

  t_options			options;
}				o_wait;

/*
 * the task architecture dependent interface
 */

typedef struct
{
  t_error			(*task_show)(i_task);
  t_error			(*task_clone)(i_task,
					      i_task*);
  t_error			(*task_reserve)(t_class,
						t_behaviour,
						t_priority,
						i_task*);
  t_error			(*task_release)(i_task);
  t_error			(*task_priority)(i_task,
						 t_priority);
  t_error			(*task_run)(i_task);
  t_error			(*task_stop)(i_task);
  t_error			(*task_block)(i_task);
  t_error			(*task_die)(i_task);
  t_error			(*task_wait)(i_task,
					     t_options,
					     t_wait*);
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

t_error			task_current(i_task*			tsk);

t_error			task_show(i_task			id);

t_error			task_dump(void);

t_error			task_clone(i_task			old,
				   i_task*			new);

t_error			task_reserve(t_class			class,
				     t_behaviour		behav,
				     t_priority			prior,
				     i_task*			id);

t_error			task_release(i_task			id);

t_error			task_priority(i_task			id,
				      t_priority		prior);

t_error			task_run(i_task				id);

t_error			task_stop(i_task			id);

t_error			task_block(i_task			id);

t_error			task_die(i_task				id);

t_error			task_wait(i_task			id,
				  t_options			opts,
				  t_wait*			wait);

t_error			task_exist(i_task			id);

t_error			task_get(i_task				id,
				 o_task**			o);

t_error			task_initialize(void);

t_error			task_clean(void);


/*
 * eop
 */

#endif
