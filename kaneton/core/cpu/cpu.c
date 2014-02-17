/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/cpu/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       julien quintard   [sun jan 30 20:04:19 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the CPU manager is mainly used by the scheduler to perform efficient
 * CPU load balancing when it comes to SMP - Symmetric Multi-Processing.
 *
 * the scheduler must communicate to the CPU manager the statistics
 * regarding the usage of each CPU.
 *
 * then, every time the cpu_balance() function is called, the statistics
 * are compared and the more loaded CPUs give away some tasks to the less
 * loaded ones. the cpu_migrate() function is used when such an event
 * occurs, migrating a task and its threads to a specific CPU.
 *
 * the cpu_select() function is called by the task manager to assign a
 * CPU to a task. like the load balancing algorithm, the selection should
 * take account of the CPUs loads.
 *
 * [XXX:improvements] implement the cpu_balance() function based on the
 *                    CPUs' statistics.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * include the machine-specific definitions required by the core.
 */

machine_include(cpu);

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the init structure.
 */

extern s_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the cpu manager.
 */

m_cpu			_cpu;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function shows a given CPU's attributes.
 *
 * steps:
 *
 * 1) retrieve the CPU object.
 * 2) display the CPU attributes.
 * 3) call the machine.
 */

t_status		cpu_show(i_cpu				id,
				 mt_margin			margin)
{
  o_cpu*		o;

  /*
   * 1)
   */

  if (cpu_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the CPU object");

  /*
   * 2)
   */

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "CPU:\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin));

  module_call(console, message,
	      '#',
	      MODULE_CONSOLE_MARGIN_FORMAT
	      "  core: id(%qd) efficiency(%qu)\n",
	      MODULE_CONSOLE_MARGIN_VALUE(margin),
	      o->id,
	      o->efficiency);

  /*
   * 3)
   */

  if (machine_call(cpu, show, id, margin) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function dumps the CPU manager.
 *
 * steps:
 *
 * 1) display general information.
 * 2) retrieve the size of the set of CPUs.
 * 3) show all the CPU objects.
 * 4) call the machine.
 */

t_status		cpu_dump(void)
{
  o_cpu*		data;
  t_setsz		size;
  t_state		st;
  s_iterator		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "CPU manager:\n");

  module_call(console, message,
	      '#', "  core: cpus(%qd)\n",
	      _cpu.cpus);

  /*
   * 2)
   */

  if (set_size(_cpu.cpus, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of CPUs");

  /*
   * 3)
   */

  module_call(console, message,
	      '#', "    cpus: id(%qd) size(%qd)\n",
	      _cpu.cpus,
	      size);

  set_foreach(SET_OPTION_FORWARD, _cpu.cpus, &i, st)
    {
      if (set_object(_cpu.cpus, i, (void**)&data) != STATUS_OK)
	CORE_ESCAPE("unable to retrieve the CPU identifier");

      if (cpu_show(data->id,
		   3 * MODULE_CONSOLE_MARGIN_SHIFT) != STATUS_OK)
	CORE_ESCAPE("unable to show the CPU");
    }

  /*
   * 4)
   */

  if (machine_call(cpu, dump) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function retrieves the current CPU identifier.
 *
 * note that this functionality is left to the machine to implement.
 *
 * 0) verify the arguments.
 * 1) call the machine.
 */

t_status		cpu_current(i_cpu*			id)
{
  /*
   * 0)
   */

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  if (machine_call(cpu, current, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function returns true if the kaneton kernel evolves in a
 * multiprocessor environment.
 *
 * steps:
 *
 * 1) retrieve the size of the set of CPUs.
 * 2) return false if there is a single CPU, true otherwise.
 */

t_bool			cpu_multiprocessor(void)
{
  t_setsz		size;

  /*
   * 1)
   */

  assert(set_size(_cpu.cpus, &size) == STATUS_OK);

  /*
   * 2)
   */

  if (size == 1)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns the identifier of the CPU which has been used
 * the smallest portion of time.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the size of the set of CPUs.
 * 2) select a CPU.
 * 3) call the machine.
 */

t_status		cpu_select(i_cpu*			id)
{
  static i_cpu		current = 0;
  t_setsz		size;

  /*
   * 0)
   */

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  /*
   * 1)
   */

  if (set_size(_cpu.cpus, &size) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the size of the set of CPUs");

  /*
   * 2)
   */

  *id = current++ % size;

  /*
   * 3)
   */

  if (machine_call(cpu, select, id) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function updates the statistics of a given CPU.
 *
 * steps:
 *
 * 1) retrieve the CPU object.
 * 2) update the CPU statistics.
 */

t_status		cpu_update(i_cpu			id,
				   t_timeslice			timeslice)
{
  o_cpu*		o;

  /*
   * 1)
   */

  if (cpu_get(id, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the CPU object");

  /*
   * 2)
   */

  o->efficiency += timeslice;

  /*
   * 3)
   */

  if (machine_call(cpu, update, id, timeslice) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function load-balance the tasks among the CPUs according to the
 * statistics recorded.
 *
 * steps:
 *
 * 1) call the machine.
 */

t_status		cpu_balance(void)
{
  /*
   * 1)
   */

  if (machine_call(cpu, balance) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function is used to move a task from one CPU to another.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) if the task is running, stop it the time for it to be migrated.
 * 3) change the CPU.
 * 4) call the machine.
 * 5) is the task was running, re-start it.
 */

t_status		cpu_migrate(i_task			task,
				    i_cpu			cpu)
{
  o_task*		o;
  t_state		state;

  /*
   * 1)
   */

  if (task_get(task, &o) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  state = o->state;

  if (o->state == TASK_STATE_START)
    {
      if (task_stop(task) != STATUS_OK)
	CORE_ESCAPE("unable to stop the task");
    }

  /*
   * 3)
   */

  o->cpu = cpu;

  /*
   * 4)
   */

  if (machine_call(cpu, migrate, task, cpu) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  /*
   * 5)
   */

  if (state == TASK_STATE_START)
    {
      if (task_start(task) != STATUS_OK)
	CORE_ESCAPE("unable to stop the task");
    }

  CORE_LEAVE();
}

/*
 * this function returns true if the CPU object exists.
 */

t_bool			cpu_exist(i_cpu				id)
{
  if (set_exist(_cpu.cpus, id) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function retrieves a CPU object from the set.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the object from the set of CPUs.
 */

t_status		cpu_get(i_cpu				id,
				o_cpu**				object)
{
  /*
   * 0)
   */

  if (object == NULL)
    CORE_ESCAPE("the 'object' argument is null");

  /*
   * 1)
   */

  if (set_get(_cpu.cpus, id, (void**)object) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the set of CPUs");

  CORE_LEAVE();
}

/*
 * this function initializes the CPU manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize memory for the manager's structure.
 * 3) reserve the set of CPUs.
 * 4) inject every CPU object provided by the boot loader through the
 *    init structure into the set of CPUs.
 * 5) call the machine.
 */

t_status		cpu_initialize(void)
{
  t_uint32		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "initializing the CPU manager\n");

  /*
   * 2)
   */

  memset(&_cpu, 0x0, sizeof (m_cpu));

  /*
   * 3)
   */

  if (set_reserve(array,
		  SET_OPTION_ALLOCATE,
		  _init->ncpus,
		  sizeof (o_cpu),
		  &_cpu.cpus) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set of CPUs");

  /*
   * 4)
   */

  for (i = 0; i < _init->ncpus; i++)
    {
      if (set_append(_cpu.cpus, &_init->cpus[i]) != STATUS_OK)
	CORE_ESCAPE("unable to add the object to the set of CPUs");
    }

  if (i == 1)
    module_call(console, message,
		'#', "  mode: mono-processor\n");
  else
    module_call(console, message,
		'#', "  mode: multi-processor\n");

  /*
   * 5)
   */

  if (machine_call(cpu, initialize) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}

/*
 * this function cleans the CPU manager.
 *
 * steps:
 *
 * 1) display a message.
 * 2) call the machine.
 */

t_status		cpu_clean(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "cleaning the CPU manager\n");

  /*
   * 2)
   */

  if (machine_call(cpu, clean) != STATUS_OK)
    CORE_ESCAPE("an error occured in the machine");

  CORE_LEAVE();
}
