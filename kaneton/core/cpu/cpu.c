/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/cpu/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       matthieu bucchianeri   [wed mar 21 23:57:13 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this manager is used by the scheduler to perform efficient cpu load
 * balancing of tasks execution for Symmetric MultiProcessing (SMP).
 *
 * the scheduler  must communicate to  the cpu manager  the statistics
 * for each cpus.
 *
 * then,  each time the  cpu_balance function  is called  (the machine
 * dependent   part   is  responsible   for   calling  this   function
 * periodically), the statistics are compared and the more loaded cpus
 * give some tasks to the less loaded one. the cpu_migrate function is
 * used when such event append.
 *
 * the cpu_select function  is called by the task  manager to assign a
 * task to  a cpu.  like the load  balancing algorithm,  the selection
 * must take account cpus load.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

machine_include(cpu);

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * cpu manager variable.
 */

m_cpu*			_cpu = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function show a cpu given its identifier.
 */

t_error			cpu_show(i_cpu				id)
{
  o_cpu*		o;

  if (cpu_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the CPU object");

  module_call(console, console_message,
	      '#', "  cpu %qd: execution time %qd ms\n", id,
	      o->efficiency);

  if (machine_call(cpu, cpu_show, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the cpu manager.
 */

t_error			cpu_dump(void)
{
  t_state		st;
  o_cpu*		data;
  t_setsz		size;
  t_iterator		i;

  /*
   * 1)
   */

  if (set_size(_cpu->cpus, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of CPUs");

  /*
   * 2)
   */

  module_call(console, console_message,
	      '#', "dumping %qu cpu(s):\n", size);

  set_foreach(SET_OPTION_FORWARD, _cpu->cpus, &i, st)
    {
      if (set_object(_cpu->cpus, i, (void**)&data) != ERROR_OK)
	CORE_ESCAPE("unable to retrieve the CPU object");

      if (cpu_show(data->id) != ERROR_OK)
	CORE_ESCAPE("unable to show the CPU");
    }

  CORE_LEAVE();
}

/*
 * this function get the current cpu id.
 */

t_error			cpu_current(i_cpu*			id)
{
  if (machine_call(cpu, cpu_current, id) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function tells if the kernel is running in multiprocessor mode.
 */

t_error			cpu_multiprocessor(void)
{
  if (_cpu->ncpus == 1)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function is used to select a cpu when creating a task.
 *
 */

t_error			cpu_select(i_cpu*			id)
{
  static i_cpu		current = 0;
  t_setsz		size;

  if (set_size(_cpu->cpus, &size) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of CPUs");

  *id = current % size;

  current++;

  CORE_LEAVE();
}

/*
 * this function count the execution time of useful code.
 *
 */

t_error			cpu_statistics(i_cpu			id,
				       t_timeslice		time)
{
  o_cpu*		o;

  if (cpu_get(id, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the CPU object");

  o->efficiency += time;

  CORE_LEAVE();
}

/*
 * this function is called for load balancing.
 *
 */

t_error			cpu_balance(void)
{
  /* XXX */

  CORE_LEAVE();
}

/*
 * this function is used to move a task from one cpu to another.
 *
 * steps:
 *
 * 1) stop the task if necessary.
 * 2) change the running cpu.
 * 3) call the dependent code.
 * 4) restart the task if necessary.
 *
 */

t_error			cpu_migrate(i_task			task,
				    i_cpu			cpu)
{
  o_task*		o;
  t_state		state;

  if (task_get(task, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 1)
   */

  state = o->state;

  if (o->state == TASK_STATE_RUN)
    {
      if (task_stop(task) != ERROR_OK)
	CORE_ESCAPE("unable to stop the task");
    }

  /*
   * 2)
   */


  o->cpu = cpu;

  /*
   * 3)
   */

  if (machine_call(cpu, cpu_migrate, task, cpu) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 4)
   */

  if (state == TASK_STATE_RUN)
    {
      if (task_run(task) != ERROR_OK)
	CORE_ESCAPE("unable to stop the task");
    }

  CORE_LEAVE();
}

/*
 * this function returns true if the cpu object exists.
 */

t_error			cpu_exist(i_cpu				id)
{
  if (set_exist(_cpu->cpus, id) != ERROR_TRUE)
    CORE_ESCAPE("unable to retrieve the size of the set of CPUs");

  CORE_LEAVE();
}

/*
 * this function gets a cpu object from the cpu set.
 */

t_error			cpu_get(i_cpu				id,
				o_cpu**				o)
{
  if (set_get(_cpu->cpus, id, (void**)o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of CPUs");

  CORE_LEAVE();
}

/*
 * this function initializes the cpu manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialize a statistic object.
 * 3) initialize the cpu set.
 * 4) call the machine dependent code.
 */

t_error			cpu_initialize(void)
{
  t_uint32		i;

  /*
   * 1)
   */

  if ((_cpu = malloc(sizeof(m_cpu))) == NULL)
    CORE_ESCAPE("unable to allocate memory for the CPU manager's structure");

  memset(_cpu, 0x0, sizeof(m_cpu));

  /*
   * 2)
   */

  if (set_reserve(array, SET_OPTION_ALLOC, _init->ncpus,
		  sizeof(o_cpu), &_cpu->cpus) != ERROR_OK)
    CORE_ESCAPE("unable to reserve the set of CPUs");

  /*
   * 3)
   */

  for (i = 0; i < _init->ncpus; i++)
    {
      if (set_append(_cpu->cpus, &_init->cpus[i]) != ERROR_OK)
	CORE_ESCAPE("unable to add the object to the set of CPUs");

      _cpu->ncpus++;
    }

  if (_cpu->ncpus == 1)
    module_call(console, console_message,
		'#', " system is running in mono-processor mode\n");
  else
    module_call(console, console_message,
		'#', " system is running in multi-processor mode\n");

  /*
   * 4)
   */

  if (machine_call(cpu, cpu_initialize) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the cpu manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 2) free the manager structure.
 */

t_error			cpu_clean(void)
{
  /*
   * 1)
   */

  if (machine_call(cpu, cpu_clean) != ERROR_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 2)
   */

  free(_cpu);

  CORE_LEAVE();
}
