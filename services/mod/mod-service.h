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
 * updated       matthieu bucchianeri   [mon aug  6 21:41:02 2007]
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


#endif
