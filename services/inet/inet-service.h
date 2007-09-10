/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/services/inet/inet-service.h
 *
 * created       matthieu bucchianeri   [mon sep 10 00:00:58 2007]
 * updated       matthieu bucchianeri   [mon sep 10 17:59:41 2007]
 */

#ifndef SERVICES_INET_SERVICE_H
#define SERVICES_INET_SERVICE_H

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

#define SERVICE_INET_NAME		"inet"

/*
 * service operations.
 */

#define	INET_SERVICE_IF_REGISTER	1
#define	INET_SERVICE_IF_UNREGISTER	2
#define	INET_SERVICE_IF_PUSHPKT		3
#define	INET_SERVICE_IF_CONFIG		4
#define	INET_SERVICE_IF_UP		5
#define	INET_SERVICE_IF_DOWN		6

/*
 * message type description.
 */

#define MESSAGE_TYPE_SERVICE_INET	(MESSAGE_TYPE_CRT + 7)
#define MESSAGE_SIZE_SERVICE_INET	(sizeof (t_service_inet_message) + 65536)

/*
 * misc
 */

#define INET_IF_TYPE_ETHERNET		1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * inet messages.
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
	  t_id				dev;
	  uint32_t			type;
	  uint8_t			mac[16];
	  size_t			mac_len;
	  uint16_t			mtu;
	}				if_register;
	struct
	{
	}				if_unregister;
	struct
	{
	}				if_pushpkt;
	struct
	{
	}				if_config;
	struct
	{
	}				if_up;
	struct
	{
	}				if_down;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  t_id				iface;
	}				if_register;
	struct
	{
	}				if_unregister;
	struct
	{
	}				if_pushpkt;
	struct
	{
	}				if_config;
	struct
	{
	}				if_up;
	struct
	{
	}				if_down;
      }			u;
    }			reply;
  } u;
}			t_service_inet_message;

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef INET_SPAWN_INTERFACE

#define INET_ATTRIBUTE_DEFAULT		static inline

#ifndef INET_ATTRIBUTE_INTERFACE
# define INET_ATTRIBUTE_INTERFACE	INET_ATTRIBUTE_DEFAULT
#endif

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_init(void)
{
  return message_register(MESSAGE_TYPE_SERVICE_INET,
			  MESSAGE_SIZE_SERVICE_INET);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_register(i_task inet, t_id dev, uint32_t type, uint8_t* mac,
		 size_t mac_len, uint16_t mtu, t_id* iface)
{
  t_service_inet_message	message;
  i_node			inet_service;
  t_vsize			size;

  inet_service.machine = 0;
  inet_service.task = inet;

  message.u.request.operation = INET_SERVICE_IF_REGISTER;
  message.u.request.u.if_register.dev = dev;
  message.u.request.u.if_register.type = type;
  memcpy(message.u.request.u.if_register.mac, mac, mac_len);
  message.u.request.u.if_register.mac_len = mac_len;
  message.u.request.u.if_register.mtu = mtu;

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (message_receive(MESSAGE_TYPE_SERVICE_INET, (t_vaddr)&message,
		      &size, &inet_service) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (iface != NULL)
    *iface =  message.u.reply.u.if_register.iface;

  return (ERROR_NONE);
}

#endif

#endif
