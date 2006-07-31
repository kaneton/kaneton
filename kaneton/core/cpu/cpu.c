/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/cpu/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       matthieu bucchianeri   [sat jul 29 18:01:20 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
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
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initialises the cpu manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialise a statistic object.
 * 3) call the machine dependent code.
 */

t_error			cpu_init(void)
{
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

  STATS_RESERVE("cpu", &cpu->stats);

  /*
   * 3)
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
