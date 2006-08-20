/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/cpu/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       matthieu bucchianeri   [fri aug 18 19:54:35 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this manager will be used by the scheduler to perform efficient cpu
 * load balancing  of threads execution  for Symmetric MultiProcessing
 * (SMP).
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(cpu);

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

  /* XXX */

  if (machdep_call(cpu, cpu_show, id) != ERROR_NONE)
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

  if (machdep_call(cpu, cpu_current, cpuid) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function is used to select a cpu when creating a task.
 */

t_error			cpu_select(i_cpu*			cpuid)
{
  CPU_ENTER(cpu);

  *cpuid = 0; /* XXX */

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function count the execution time of useful code.
 */

t_error			cpu_stats(i_cpu				cpuid,
				  t_timeslice			time)
{
  o_cpu*		o;

  CPU_ENTER(cpu);

  if (cpu_get(cpuid, &o) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  o->efficiency += time;

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function is called for load balancing.
 *
 * steps:
 *
 * n) reset the efficiency statistics.
 */

t_error			cpu_balance(void)
{
  t_state		state;
  o_cpu*		o;
  t_iterator		i;

  CPU_ENTER(cpu);

  /*
   * n)
   */

  set_foreach(SET_OPT_FORWARD, cpu->cpus, &i, state)
    {
      if (set_object(cpu->cpus, i, (void**)&o) != ERROR_NONE)
	CPU_LEAVE(cpu, ERROR_UNKNOWN);

      /* XXX */

      o->efficiency = 0;
    }

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * this function is used to move a task from one cpu to another.
 */

t_error			cpu_migrate(i_task			tskid,
				    i_cpu			destination)
{
  CPU_ENTER(cpu);

  /* XXX */

  if (machdep_call(cpu, cpu_migrate, tskid, destination) != ERROR_NONE)
    return (ERROR_UNKNOWN);

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
 * this function initialises the cpu manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialise a statistic object.
 * 3) initialise the cpu set.
 * 4) add the cpus to the cpu set.
 * 5) call the machine dependent code.
 */

t_error			cpu_init(void)
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

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_SORT,
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
      if (set_add(cpu->cpus, &init->cpus[i]) != ERROR_NONE)
	{
	  cons_msg('!', "cpu: cannot add a cpu to the cpu set\n");

	  return (ERROR_UNKNOWN);
	}
    }

  if (init->ncpus == 1)
    cons_msg('#', " system is running in mono-processor mode.\n");
  else
    cons_msg('#', " system is running in multi-processor mode.\n");

  /*
   * 4)
   */

  STATS_RESERVE("cpu", &cpu->stats);

  /*
   * 5)
   */

  if (machdep_call(cpu, cpu_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

}

/*
 * this function cleans the cpu manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 2) release the statistics object.
 * 3) free the manager structure.
 */

t_error			cpu_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(cpu, cpu_clean) != ERROR_NONE)
    CPU_LEAVE(cpu, ERROR_UNKNOWN);

  /*
   * 2)
   */

  STATS_RELEASE(cpu->stats);

  /*
   * 3)
   */

  free(cpu);

  return (ERROR_NONE);
}
