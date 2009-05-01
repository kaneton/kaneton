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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * cpu manager variable.
 */

m_cpu*			cpu = NULL;

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function show a cpu given its identifier.
 */

t_error			cpu_show(i_cpu				id)
{
  o_cpu*		o;

  CPU_ENTER(cpu);

  if (cpu_get(id, &o) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  cons_msg('#', "  cpu %qd: execution time %qd ms\n", id,
	   o->efficiency);

  if (machine_call(cpu, cpu_show, id) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function dumps the cpu manager.
 */

t_error			cpu_dump(void)
{
  t_state		state;
  o_cpu*		data;
  t_setsz		size;
  t_iterator		i;

  CPU_ENTER(cpu);

  /*
   * 1)
   */

  if (set_size(cpu->cpus, &size) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "dumping %qu cpu(s):\n", size);

  set_foreach(SET_OPT_FORWARD, cpu->cpus, &i, state)
    {
      if (set_object(cpu->cpus, i, (void**)&data) != ERROR_NONE)
	CPU_LEAVE(cpu, ERROR_UNKNOWN);

      if (cpu_show(data->cpuid) != ERROR_NONE)
	CPU_LEAVE(cpu, ERROR_UNKNOWN);
    }

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function get the current cpu id.
 */

t_error			cpu_current(i_cpu*			cpuid)
{
  CPU_ENTER(cpu);

  if (machine_call(cpu, cpu_current, cpuid) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function tells if the kernel is running in multiprocessor mode.
 */

t_error			cpu_multiprocessor(void)
{
  return ((cpu->ncpus == 1) ? ERROR_UNKNOWN : ERROR_NONE);
}

/*
 * this function is used to select a cpu when creating a task.
 *
 */

t_error			cpu_select(i_cpu*			cpuid)
{
  static i_cpu		cur = 0;
  t_setsz		size;

  CPU_ENTER(cpu);

  if (set_size(cpu->cpus, &size) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  *cpuid = cur % size;
  cur++;

  *cpuid = 0;

  /*
   * XXX select the cpu with lowest charge.
   */

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function count the execution time of useful code.
 *
 */

t_error			cpu_stats(i_cpu				cpuid,
				  t_timeslice			time)
{
  o_cpu*		o;

  CPU_ENTER(cpu);

  if (cpu_get(cpuid, &o) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  o->efficiency += time;

  /*
   * XXX we must take account number of context switch and complete schedule
   */

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function is called for load balancing.
 *
 */

t_error			cpu_balance(void)
{
  CPU_ENTER(cpu);

  /* XXX */

  CPU_LEAVE(cpu, ERROR_NONE);
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

t_error			cpu_migrate(i_task			tskid,
				    i_cpu			destination)
{
  o_task*		o;
  t_state		old_state;

  CPU_ENTER(cpu);

  if (task_get(tskid, &o) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  /*
   * 1)
   */

  old_state = o->sched;
  if (o->sched == SCHEDULER_STATE_RUN)
    if (task_state(tskid, SCHEDULER_STATE_STOP) != ERROR_NONE)
      CPU_LEAVE(cpu, ERROR_UNKNOWN);

  /*
   * 2)
   */


  o->cpuid = destination;

  /*
   * 3)
   */

  if (machine_call(cpu, cpu_migrate, tskid, destination) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (old_state == SCHEDULER_STATE_RUN)
    if (task_state(tskid, SCHEDULER_STATE_RUN) != ERROR_NONE)
      CPU_LEAVE(cpu, ERROR_UNKNOWN);

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function gets a cpu object from the cpu set.
 */

t_error			cpu_get(i_cpu				id,
				o_cpu**				o)
{
  CPU_ENTER(cpu);

  if (set_get(cpu->cpus, id, (void**)o) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  CPU_LEAVE(cpu, ERROR_NONE);
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

  if ((cpu = malloc(sizeof(m_cpu))) == NULL)
    {
      cons_msg('!', "cpu: cannot allocate memory for the cpu "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(cpu, 0x0, sizeof(m_cpu));

  /*
   * 2)
   */

  if (set_reserve(array, SET_OPT_ALLOC, init->ncpus,
		  sizeof(o_cpu), &cpu->cpus) != ERROR_NONE)
    {
      cons_msg('!', "cpu: unable to reserve the cpu set\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  for (i = 0; i < init->ncpus; i++)
    {
      if (set_append(cpu->cpus, &init->cpus[i]) != ERROR_NONE)
	{
	  cons_msg('!', "cpu: cannot add a cpu to the cpu set\n");

	  return (ERROR_UNKNOWN);
	}

      cpu->ncpus++;
    }

  if (init->ncpus == 1)
    cons_msg('#', " system is running in mono-processor mode.\n");
  else
    cons_msg('#', " system is running in multi-processor mode.\n");

  /*
   * 4)
   */

  if (machine_call(cpu, cpu_initialize) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

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

  if (machine_call(cpu, cpu_clean) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  /*
   * 2)
   */

  free(cpu);

  return (ERROR_NONE);
}
