/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/services/mod/mod-service.h
 *
 * created       matthieu bucchianeri   [mon aug  6 21:35:25 2007]
 * updated       matthieu bucchianeri   [thu aug 23 16:44:23 2007]
 */

#ifndef SERVICES_MOD_SERVICE_H
#define SERVICES_MOD_SERVICE_H

/*
 * ---------- information -----------------------------------------------------
 *
 * mod service interface
 *
 * the service replies to the following requests:
 *
 * - MOD_GET_SERVICE(string name)
 *   return the task id of the requested service.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libc.h>
#include <crt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define	MOD_SERVICE_GET_SERVICE	1
#define MESSAGE_TYPE_SERVICE_MOD	(MESSAGE_TYPE_CRT + 1)
#define MOD_MAX_LEN			64
#define MESSAGE_SIZE_SERVICE_MOD	(sizeof (t_service_mod_message) + MOD_MAX_LEN)


/*
 * ---------- types -----------------------------------------------------------
 */


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
	  t_uint8			name[1];
	}				get_service;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  i_task			id;
	}				get_service;
      }			u;
    }			reply;
  } u;
}			t_service_mod_message;

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef MOD_SPAWN_INTERFACE

#ifdef MOD_INLINE_INTERFACE
# define MOD_ATTRIBUTE_INTERFACE	inline __attribute__((unused))
#else
# define MOD_ATTRIBUTE_INTERFACE	__attribute__((unused))
#endif

static MOD_ATTRIBUTE_INTERFACE t_error
mod_init(void)
{
  return message_register(MESSAGE_TYPE_SERVICE_MOD, MESSAGE_SIZE_SERVICE_MOD);
}

static MOD_ATTRIBUTE_INTERFACE t_error
mod_get_service(i_task mod, char* name, i_task* service)
{
  t_service_mod_message*	message;
  i_node			mod_service;
  t_vsize			size;

  mod_service.machine = 0;
  mod_service.task = mod;

  if ((message = malloc(sizeof (*message) + 12)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = MOD_SERVICE_GET_SERVICE;
  strcpy(message->u.request.u.get_service.name, name);

  if (message_send(mod_service,
		   MESSAGE_TYPE_SERVICE_MOD, (t_vaddr)message,
		   sizeof (*message) + strlen(name) + 1) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (message_receive(MESSAGE_TYPE_SERVICE_MOD, (t_vaddr)message,
		      &size, &mod_service) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (service != NULL)
    *service = message->u.reply.u.get_service.id;

  free(message);

  return (ERROR_NONE);
}

#endif

#endif
