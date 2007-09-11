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
 * updated       matthieu bucchianeri   [tue sep 11 15:16:24 2007]
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

#define INET_SERVICE_ROUTE_ADD		7
#define INET_SERVICE_ROUTE_FLUSH	8
#define INET_SERVICE_ROUTE_DEL		9

#define INET_SERVICE_IF_DUMP		100
#define INET_SERVICE_ROUTE_DUMP		101

/*
 * message type description.
 */

#define MESSAGE_TYPE_SERVICE_INET	(MESSAGE_TYPE_CRT + 7)
#define MESSAGE_SIZE_SERVICE_INET	(sizeof (t_service_inet_message) + 65536)

/*
 * misc
 */

#define INET_IF_TYPE_ETHERNET		1

#define INET_IF_CONFIG_SET		1
#define INET_IF_CONFIG_ADD		2
#define INET_IF_CONFIG_DEL		3

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * inet address
 */

typedef struct
{
  enum
    {
      e_service_inet_ipv4
    }				type;
  union
  {
    uint32_t			ipv4;
  }				addr;
}				t_service_inet_addr;

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
	  t_id				iface;
	}				if_unregister;
	struct
	{
	  t_id				iface;
	  uint8_t			s_mac[16];
	  uint8_t			d_mac[16];
	  size_t			mac_len;
	  uint16_t			proto;
	  uint16_t			size;
	  uint8_t			packet[1];
	}				if_pushpkt;
	struct
	{
	  t_service_inet_addr		addr;
	  t_service_inet_addr		mask;
	  uint32_t			op;
	  uint8_t			name[1];
	}				if_config;
	struct
	{
	  uint8_t			name[1];
	}				if_up;
	struct
	{
	  uint8_t			name[1];
	}				if_down;
	struct
	{
	  t_service_inet_addr		target;
	  t_service_inet_addr		mask;
	  uint32_t			routed;
	  t_service_inet_addr		router;
	  uint8_t			name[1];
	}				route_add;
	struct
	{
	  uint8_t			name[1];
	}				route_flush;
	struct
	{
	  /* XXX route_del */
	}				route_del;
	struct
	{
	  uint8_t			name[1];
	}				if_dump;
	struct
	{
	}				route_dump;
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
	  t_error			result;
	}				if_config;
	struct
	{
	  t_error			result;
	}				if_up;
	struct
	{
	}				if_down;
	struct
	{
	  t_error			result;
	}				route_add;
	struct
	{
	}				route_flush;
	struct
	{
	}				route_del;
	struct
	{
	}				if_dump;
	struct
	{
	}				route_dump;
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

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_unregister(i_task inet, t_id iface)
{
  t_service_inet_message	message;
  i_node			inet_service;

  inet_service.machine = 0;
  inet_service.task = inet;

  message.u.request.operation = INET_SERVICE_IF_UNREGISTER;
  message.u.request.u.if_unregister.iface = iface;

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_pushpkt(i_task inet, t_id iface, uint8_t* s_mac, uint8_t* d_mac,
		size_t mac_len, uint16_t proto, uint8_t* packet, uint16_t size)
{
  t_service_inet_message*	message;
  i_node			inet_service;

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_IF_PUSHPKT;
  message->u.request.u.if_pushpkt.iface = iface;
  memcpy(message->u.request.u.if_pushpkt.s_mac, s_mac, mac_len);
  memcpy(message->u.request.u.if_pushpkt.d_mac, d_mac, mac_len);
  message->u.request.u.if_pushpkt.mac_len = mac_len;
  message->u.request.u.if_pushpkt.proto = proto;
  message->u.request.u.if_pushpkt.size = size;
  memcpy(message->u.request.u.if_pushpkt.packet, packet, size);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  free(message);

  return (ERROR_NONE);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_config(i_task inet, char* iface, uint32_t op, t_service_inet_addr* addr,
	       t_service_inet_addr* mask)
{
  t_service_inet_message*	message;
  i_node			inet_service;
  t_vsize			size = strlen(iface);
  t_error			res;

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_IF_CONFIG;
  memcpy(&message->u.request.u.if_config.addr, addr, sizeof (*addr));
  memcpy(&message->u.request.u.if_config.mask, mask, sizeof (*mask));
  message->u.request.u.if_config.op = op;
  strcpy(message->u.request.u.if_config.name, iface);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (message_receive(MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		      &size, &inet_service) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  res = message->u.reply.u.if_config.result;

  free(message);

  return (res);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_up(i_task inet, char* iface)
{
  t_service_inet_message*	message;
  i_node			inet_service;
  t_vsize			size = strlen(iface);
  t_error			res;

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_IF_UP;
  strcpy(message->u.request.u.if_up.name, iface);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (message_receive(MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		      &size, &inet_service) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  res = message->u.reply.u.if_up.result;

  free(message);

  return (res);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_down(i_task inet, char* iface)
{
  t_service_inet_message*	message;
  i_node			inet_service;
  t_vsize			size = strlen(iface);

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_IF_DOWN;
  strcpy(message->u.request.u.if_down.name, iface);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  free(message);

  return (ERROR_NONE);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_route_add(i_task inet, char* iface, t_service_inet_addr* target,
	       t_service_inet_addr* mask, t_service_inet_addr* router)
{
  t_service_inet_message*	message;
  i_node			inet_service;
  t_vsize			size = strlen(iface);
  t_error			res;

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_ROUTE_ADD;
  memcpy(&message->u.request.u.route_add.target, target, sizeof (*target));
  memcpy(&message->u.request.u.route_add.mask, mask, sizeof (*mask));
  if (router == NULL)
    message->u.request.u.route_add.routed = 0;
  else
    {
      message->u.request.u.route_add.routed = 1;
      memcpy(&message->u.request.u.route_add.router, router, sizeof (*router));
    }
  strcpy(message->u.request.u.route_add.name, iface);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  if (message_receive(MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		      &size, &inet_service) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  res = message->u.reply.u.route_add.result;

  free(message);

  return (res);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_route_flush(i_task inet, char* iface)
{
  t_service_inet_message*	message;
  i_node			inet_service;
  t_vsize			size = strlen(iface);

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_ROUTE_FLUSH;
  strcpy(message->u.request.u.route_flush.name, iface);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  free(message);

  return (ERROR_NONE);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_if_dump(i_task inet, char* iface)
{
  t_service_inet_message*	message;
  i_node			inet_service;
  t_vsize			size = strlen(iface);

  inet_service.machine = 0;
  inet_service.task = inet;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = INET_SERVICE_IF_DUMP;
  strcpy(message->u.request.u.if_dump.name, iface);

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  free(message);

  return (ERROR_NONE);
}

INET_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
inet_route_dump(i_task inet)
{
  t_service_inet_message	message;
  i_node			inet_service;

  inet_service.machine = 0;
  inet_service.task = inet;

  message.u.request.operation = INET_SERVICE_ROUTE_DUMP;

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

#endif

#endif
