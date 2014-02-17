/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/architecture/ia32/educational/io.c
 *
 * created       julien quintard   [sun jan  9 22:11:18 2011]
 * updated       julien quintard   [sun jan 30 20:45:26 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains several functions for manipulating I/O bitmaps and
 * accesses.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- exports ---------------------------------------------------------
 */

/*
 * the thread manager.
 */

extern m_thread		_thread;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function clears the given task's I/O permission bitmap.
 *
 * note that a bit set means that access is denied. therefore, this
 * function sets all the bits to one.
 *
 * steps:
 *
 * 1) retrieve the task object.
 * 2) clear the bitmap.
 */

t_status		architecture_io_clear(i_task		task)
{
  o_task*		o;

  /*
   * 1)
   */

  if (task_get(task, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  memset(&o->machine.io.map, 0xFF, sizeof (o->machine.io.map));

  MACHINE_LEAVE();
}

/*
 * this function modifies a bit in a map.
 */

t_status		architecture_io_set(t_uint8*		map,
					    t_uint32		port,
					    t_uint32		value)
{
  if (value)
    map[port / 8] |= (1 << (port % 8));
  else
    map[port / 8] &= ~(1 << (port % 8));

  MACHINE_LEAVE();
}

/*
 * this function returns the value of a given bit in the map.
 */

t_status		architecture_io_get(t_uint8*		map,
					    t_uint32		port,
					    t_uint32*		value)
{
  *value = map[port / 8] & (1 << (port % 8));

  MACHINE_LEAVE();
}

/*
 * this function grants access to the given task on some I/O ports.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the task object.
 * 2) activate the access to the given I/O ports, by setting the map's bit
 *    to zero.
 * 3) retrieve the currently scheduled task's identifier.
 * 4) retrieve the system's TSS structure.
 * 5) update the task's current I/O.
 *   A) if the task is currently running, the CPU needs to be informed
 *      of the modified permissions by updating the TSS' I/O map.
 *   B) otherwise, mark the task's hardware I/O map as needing to be
 *      re-actualized.
 */

t_status		architecture_io_grant(i_task		task,
					      t_uint32		port,
					      t_uint8		width)
{
  i_task		current;
  as_tss*		tss;
  o_task*		o;
  t_uint8		i;

  /*
   * 0)
   */

  if ((port + width) >= ARCHITECTURE_IO_MAP_BITS)
    MACHINE_ESCAPE("out-of-bound I/O port");

  /*
   * 1)
   */

  if (task_get(task, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  for (i = 0; i < width; i++)
    {
      if (architecture_io_set(o->machine.io.map,
			      port + i,
			      0) != STATUS_OK)
	MACHINE_ESCAPE("unable to set the I/O bitmap");
    }

  /*
   * 3)
   */

  if (task_current(&current) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the current task's identifier");

  /*
   * 4)
   */

  tss = (as_tss*)_thread.machine.tss;

  /*
   * 5)
   */

  if (current == task)
    {
      /*
       * A)
       */

      memcpy(((t_uint8*)tss) + tss->io,
	     &o->machine.io.map,
	     sizeof (o->machine.io.map));
    }
  else
    {
      /*
       * B)
       */

      o->machine.io.flush = BOOLEAN_TRUE;
    }

  MACHINE_LEAVE();
}

/*
 * this function denies access to the given task on some I/O ports.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the task object.
 * 2) deactivate the access to the given I/O ports, by setting the map's bit
 *    to one.
 * 3) retrieve the currently scheduled task's identifier.
 * 4) retrieve the system's TSS structure.
 * 5) update the task's current I/O.
 *   A) if the task is currently running, the CPU needs to be informed
 *      of the modified permissions by updating the TSS' I/O map.
 *   B) otherwise, mark the task's hardware I/O map as needing to be
 *      re-actualized.
 */

t_status		architecture_io_deny(i_task		task,
					     t_uint32		port,
					     t_uint8		width)
{
  i_task		current;
  as_tss*		tss;
  o_task*		o;
  t_uint8		i;

  /*
   * 0)
   */

  if ((port + width) >= ARCHITECTURE_IO_MAP_BITS)
    MACHINE_ESCAPE("out-of-bound I/O port");

  /*
   * 1)
   */

  if (task_get(task, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  for (i = 0; i < width; i++)
    {
      if (architecture_io_set(o->machine.io.map,
			      port + i,
			      1) != STATUS_OK)
	MACHINE_ESCAPE("unable to set the I/O bitmap");
    }

  /*
   * 3)
   */

  if (task_current(&current) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the current task's identifier");

  /*
   * 4)
   */

  tss = (as_tss*)_thread.machine.tss;

  /*
   * 5)
   */

  if (current == task)
    {
      /*
       * A)
       */

      memcpy(((t_uint8*)tss) + tss->io,
	     &o->machine.io.map,
	     sizeof (o->machine.io.map));
    }
  else
    {
      /*
       * B)
       */

      o->machine.io.flush = BOOLEAN_TRUE;
    }

  MACHINE_LEAVE();
}

/*
 * this function sets the current I/O PL - I/O Privilege to zero.
 */

t_status		architecture_io_reset(void)
{
  asm volatile("pushf\n\t"
	       "andl $0xFFFFCFFF, %ss:(%esp)\n\t"
	       "popf");

  MACHINE_LEAVE();
}
