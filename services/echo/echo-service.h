
#ifndef SERVICES_ECHO_SERVICE_H_
#define SERVICES_ECHO_SERVICE_H_

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libkaneton.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * service name.
 */

#define SERVICE_ECHO_NAME		"echo"

/*
 * service operations.
 */

#define ECHO_SERVICE_ECHO		1

/*
 * message type description.
 */

#define MESSAGE_TYPE_SERVICE_ECHO	(MESSAGE_TYPE_CRT + 8)
#define MESSAGE_SIZE_SERVICE_ECHO	(sizeof (t_service_echo_message))

/*
 * echo buffer size.
 */

#define ECHO_SERVICE_MAX		100

/*
 * ---------- types -----------------------------------------------------------
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
	  char		text[ECHO_SERVICE_MAX];
	}		echo;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  char		text[ECHO_SERVICE_MAX];
	}		echo;
      }			u;
    }			reply;
  } u;
}		t_service_echo_message;

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef ECHO_SPAWN_INTERFACE

#define ECHO_ATTRIBUTE_DEFAULT		static inline

#ifndef ECHO_ATTRIBUTE_INTERFACE
# define ECHO_ATTRIBUTE_INTERFACE	ECHO_ATTRIBUTE_DEFAULT
#endif

ECHO_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
echo_init(void)
{
  return (message_register(MESSAGE_TYPE_SERVICE_ECHO,
			   MESSAGE_SIZE_SERVICE_ECHO));
}

ECHO_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
echo_echo(i_task echo, const char* text, char* buf, size_t max)
{
  t_service_echo_message	message;
  i_node			echo_service;
  t_vsize			size;

  echo_service.machine = 0;
  echo_service.task = echo;

  message.u.request.operation = ECHO_SERVICE_ECHO;
  strcpy(message.u.request.u.echo.text, text);

  if (message_send(echo_service,
		   MESSAGE_TYPE_SERVICE_ECHO, (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (message_receive(MESSAGE_TYPE_SERVICE_ECHO, (t_vaddr)&message,
		      &size, &echo_service) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  strncpy(buf, message.u.reply.u.echo.text, max);

  return (ERROR_NONE);
}

#endif

#endif
