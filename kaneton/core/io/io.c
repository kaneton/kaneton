/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/io/io.c
 *
 * created       matthieu bucchianeri   [sat jul 29 17:59:35 2006]
 * updated       matthieu bucchianeri   [thu aug  3 20:10:20 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the I/O  manager is  designed to grant  or deny  low-level hardware
 * accesses to tasks.
 *
 * the only available operation is allowing or denying the access of a
 * given hardware address (called a port) to a task.
 *
 * as low-level I/O are hardware specific, the manager only does calls
 * to the architecture dependent code.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

machdep_include(io);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * io manager variable.
 */

m_io*			io = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function grant or deny I/O to a given port for a task.
 */

t_error			io_grant(i_port				id,
				 i_task				task,
				 t_state			state)
{
  IO_ENTER(io);

  if (state != IO_GRANT && state != IO_DENY)
    IO_LEAVE(io, ERROR_UNKNOWN);

  if (machdep_call(io, io_grant, id, task, state) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  IO_LEAVE(io, ERROR_NONE);
}

/*
 * this function initialises the io manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialise a statistic object.
 * 3) call the machine dependent code.
 */

t_error			io_init(void)
{
  /*
   * 1)
   */

  if ((io = malloc(sizeof(m_io))) == NULL)
    {
      cons_msg('!', "io: cannot allocate memory for the io "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(io, 0x0, sizeof(m_io));

  /*
   * 2)
   */

  STATS_RESERVE("io", &io->stats);

  /*
   * 3)
   */

  if (machdep_call(io, io_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);

}

/*
 * this function cleans the io manager.
 *
 * steps:
 *
 * 1) call the dependent code.
 * 2) release the statistics object.
 * 3) free the manager structure.
 */

t_error			io_clean(void)
{
  /*
   * 1)
   */

  if (machdep_call(io, io_clean) != ERROR_NONE)
    IO_LEAVE(io, ERROR_UNKNOWN);

  /*
   * 2)
   */

  STATS_RELEASE(io->stats);

  /*
   * 3)
   */

  free(io);

  return (ERROR_NONE);
}
