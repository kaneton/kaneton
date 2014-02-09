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
 * updated       julien quintard   [sat dec 18 11:07:11 2010]
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
 * the classes of tasks. the class has an impact on the range of priorities
 * for the task hence on the scheduling priority.
 *
 * besides, the class indicates the task's privileges on the system. indeed
 * while the kernel task can do everything, a driver can access hardware
 * devices. a service is a task which provide a so-called service i.e waits
 * for messages and performs an operation on behalf of the caller. finally
 * a guest task in an application which does nothing but to compute and
 * request other tasks to perform operations on its behalf.
 */

#define TASK_CLASS_KERNEL		(1 << 0)
#define TASK_CLASS_DRIVER		(1 << 1)
#define TASK_CLASS_SERVICE		(1 << 2)
#define TASK_CLASS_GUEST		(1 << 3)

/*
 * the behaviour specifies the way the task behaves depending on the
 * program it contains. the kernel behaviour is reserved for the kernel
 * task. a real-time task is often I/O bound and therefore needs to be
 * scheduled quickly once woken up. an interactive task is a normal task
 * which needs a little more of reactivity since interacting with the
 * user through a user interface. the timesharing behaviour is the default
 * for tasks performing normal computations. finally, the background
 * behaviour can be used by tasks running in background, blocked most of
 * the time waiting for time to pass for instance.
 *
 * note that the behaviour obviously has an impact on the scheduling
 * election algorithm.
 */

#define TASK_BEHAVIOUR_KERNEL		(1 << 0)
#define TASK_BEHAVIOUR_REALTIME		(1 << 1)
#define TASK_BEHAVIOUR_INTERACTIVE	(1 << 2)
#define TASK_BEHAVIOUR_TIMESHARING	(1 << 3)
#define TASK_BEHAVIOUR_BACKGROUND	(1 << 4)

/*
 * the priority ranges below are specific to the task's class. therefore
 * a task's priority can only evolve in this range.
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
 * the initial sizes for the sets of threads etc.
 */

#define TASK_THREADS_INITSZ	0x1
#define TASK_WAITS_INITSZ	0x1

/*
 * the current task's state.
 *
 * note that whenever a task exists, its state is set to ZOMBIE. a task
 * stays in this state until a thread probes its state through the task_wait()
 * functionality. at this point, the task's state is set to dead until its
 * resources are completely cleaned from the system.
 */

#define TASK_STATE_START	1
#define TASK_STATE_STOP		2
#define TASK_STATE_BLOCK	3
#define TASK_STATE_ZOMBIE	4
#define TASK_STATE_DEAD		5

/*
 * this macro defines the delay, in milliseconds, for the morgue to
 * be triggered and clean the dead tasks.
 */

#define TASK_MORGUE_DELAY	10000

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the task object's structure.
 *
 * the 'cpu' indicates the CPU on which the task will be scheduled.
 *
 * the 'parent' attribute specifies the identifier of the task which
 * creted this task while 'children' references the set of children tasks.
 *
 * the 'class', 'behaviour' and 'priority' specific the task's characteristics.
 * for more information, please refer to the macro related to these
 * attributes.
 *
 * the 'as' field contains the identifier of the task's address space.
 *
 * the 'threads' specifies the set containing the identifiers of the threads
 * living in this task.
 *
 * the 'state' indicates the current task's state: started, stopped, blocked
 * etc.
 *
 * the 'waits' set contains the identifiers of the threads which are waiting
 * for the task to change its state to a particular one.
 *
 * the 'value' attribute contains the exit code provided through the
 * task_exit() call.
 *
 * finally, the 'message' contains the set of messages associated with
 * the task.
 *
 * note that a timer is provided should the task need to trigger an
 * action. for instance the timer is used for the task to sleep for some
 * time.
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

  t_value			value;

  i_set				messages;

  i_timer			timer;

  machine_data(o_task);
}				o_task;

/*
 * the task manager's structure.
 */

typedef struct
{
  o_id				id;

  i_set				tasks;

  struct
  {
    i_set			field;
    i_timer			timer;
  }				morgue;

  machine_data(m_task);
}				m_task;

/*
 * the task dispatcher.
 */

typedef struct
{
  t_status		(*task_show)(i_task,
					     mt_margin);
  t_status		(*task_dump)(void);
  t_status		(*task_reserve)(t_class,
						t_behaviour,
						t_priority,
						i_task*);
  t_status		(*task_release)(i_task);
  t_status		(*task_priority)(i_task,
						 t_priority);
  t_status		(*task_start)(i_task);
  t_status		(*task_stop)(i_task);
  t_status		(*task_block)(i_task);
  t_status		(*task_exit)(i_task,
					     t_value);
  t_status		(*task_wait)(i_thread,
					     i_task,
					     t_state,
					     s_wait*);
  t_status		(*task_sleep)(i_task,
					      t_delay);
  t_status		(*task_initialize)(void);
  t_status		(*task_clean)(void);
}				d_task;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/task/task.c
 */

/*
 * ../../core/task/task.c
 */

t_status		task_show(i_task			id,
				  mt_margin			margin);

t_status		task_dump(void);

t_status		task_reserve(t_class			class,
				     t_behaviour		behav,
				     t_priority			prior,
				     i_task*			id);

t_status		task_release(i_task			id);

t_status		task_priority(i_task			id,
				      t_priority		prior);

t_status		task_start(i_task			id);

t_status		task_stop(i_task			id);

t_status		task_block(i_task			id);

t_status		task_exit(i_task			id,
				  t_value			value);

void			task_bury(i_timer			timer,
				  t_vaddr			data);

t_status		task_wait(i_thread			id,
				  i_task			target,
				  t_state			state,
				  s_wait*			wait);

void			task_wakeup(i_timer			timer,
				    t_vaddr			data);

t_status		task_sleep(i_task			id,
				   t_delay			milliseconds);

t_status		task_current(i_task*			task);

t_bool			task_exist(i_task			id);

t_status		task_get(i_task				id,
				 o_task**			object);

t_status		task_initialize(void);

t_status		task_clean(void);


/*
 * eop
 */

#endif
