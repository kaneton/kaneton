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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * driver name.
 */

#define DRIVER_CONS_SIMPLE_NAME		"cons-simple"

/*
 * driver operations.
 */

#define	CONS_SIMPLE_DRIVER_WRITE	1

/*
 * message type description.
 */

#define MESSAGE_TYPE_DRIVER_CONS_SIMPLE	(MESSAGE_TYPE_CRT + 0)
#define MESSAGE_SIZE_DRIVER_CONS_SIMPLE	(sizeof (t_driver_cons_simple_message) + CONS_SIMPLE_MAX_BUFFER)

/*
 * maximum buffer to display.
 */

#define CONS_SIMPLE_MAX_BUFFER		512

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

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef CONS_SIMPLE_SPAWN_INTERFACE

#define CONS_SIMPLE_ATTRIBUTE_DEFAULT		static inline

#ifndef CONS_SIMPLE_ATTRIBUTE_INTERFACE
# define CONS_SIMPLE_ATTRIBUTE_INTERFACE	CONS_SIMPLE_ATTRIBUTE_DEFAULT
#endif

CONS_SIMPLE_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
cons_simple_init(void)
{
  return message_register(MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
			  MESSAGE_SIZE_DRIVER_CONS_SIMPLE);
}

CONS_SIMPLE_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
cons_simple_write(i_task cons_simple, t_driver_cons_simple_size n, char* buff,
		  t_driver_cons_simple_size* wrote)
{
  t_driver_cons_simple_message*	message;
  i_node			cons_driver;
  t_vsize			size;

  if (n > CONS_SIMPLE_MAX_BUFFER)
    return (ERROR_UNKNOWN);

  cons_driver.machine = 0;
  cons_driver.task = cons_simple;

  if ((message = malloc(sizeof (*message) + n)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = CONS_SIMPLE_DRIVER_WRITE;
  message->u.request.u.write.count = n;
  memcpy(&message->u.request.u.write.c[0], buff, n);

  if (message_send(cons_driver,
		   MESSAGE_TYPE_DRIVER_CONS_SIMPLE, (t_vaddr)message,
		   sizeof (*message) + n) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (message_receive(MESSAGE_TYPE_DRIVER_CONS_SIMPLE, (t_vaddr)message,
		      &size, &cons_driver) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (wrote != NULL)
    *wrote = message->u.reply.u.write.wrote;

  free(message);

  return (ERROR_NONE);
}

#endif

#endif
