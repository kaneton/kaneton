/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/cons-simple/cons-simple-driver.h
 *
 * created       matthieu bucchianeri   [sat jun  9 18:37:48 2007]
 * updated       matthieu bucchianeri   [sat jun  9 22:37:59 2007]
 */

#ifndef DRIVERS_CONS_SIMPLE_DRIVER_H
#define DRIVERS_CONS_SIMPLE_DRIVER_H

/*
 * ---------- information -----------------------------------------------------
 *
 * simple console driver interface.
 *
 * the driver replies to the following requests:
 *
 * - CONS_SIMPLE_WRITE(t_driver_cons_simple_size, t_uint8*)
 *   display the specified string on screen.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libc.h>
#include <crt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define	CONS_SIMPLE_DRIVER_WRITE	1
#define MESSAGE_TYPE_DRIVER_CONS_SIMPLE	(MESSAGE_TYPE_CRT + 0)
#define CONS_SIMPLE_MAX_BUFFER		512
#define MESSAGE_SIZE_DRIVER_CONS_SIMPLE	(sizeof (t_driver_cons_simple_message) + CONS_SIMPLE_MAX_BUFFER)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * basic types.
 */

typedef t_uint32	t_driver_cons_simple_size;

/*
 * simple console messages.
 */

typedef struct
{
  union
  {
    struct
    {
      t_id			operation;
      union
      {
	struct
	{
	  t_driver_cons_simple_size	count;
	  t_uint8			c[1];
	}				write;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  t_driver_cons_simple_size	wrote;
	}				write;
      }			u;
    }			reply;
  } u;
}			t_driver_cons_simple_message;

#endif
