/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/ne2000-pci/ne2000-pci-driver.h
 *
 * created       matthieu bucchianeri   [sat jun  9 22:15:54 2007]
 * updated       matthieu bucchianeri   [sat jun  9 22:38:03 2007]
 */

#ifndef DRIVERS_NE2000_PCI_DRIVER_H
# define DRIVERS_NE2000_PCI_DRIVER_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libkaneton.h>
#include <crt.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ether.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * driver name
 */

#define DRIVER_NE2000_PCI_NAME		"ne2000-pci"

/*
 * driver messages description.
 */

#define MESSAGE_TYPE_DRIVER_NE2000_PCI	(MESSAGE_TYPE_CRT + 6)
#define MESSAGE_SIZE_DRIVER_NE2000_PCI	(sizeof (t_driver_ne2000_pci_message) + ETH_DATA_LEN)

/*
 * driver operations
 */

#define NE2000_PCI_DRIVER_SENDPKT	1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * driver messages.
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
	  uint8_t			mac[ETH_ALEN];
	  uint16_t			proto;
	  uint16_t			size;
	  uint8_t			packet[1];
	}				sendpkt;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	}				sendpkt;
      }			u;
    }			reply;
  } u;
}			t_driver_ne2000_pci_message;

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef NE2000_PCI_SPAWN_INTERFACE

#define NE2000_PCI_ATTRIBUTE_DEFAULT		static inline

#ifndef NE2000_PCI_ATTRIBUTE_INTERFACE
# define NE2000_PCI_ATTRIBUTE_INTERFACE		NE2000_PCI_ATTRIBUTE_DEFAULT
#endif

NE2000_PCI_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
ne2000_pci_init(void)
{
  return message_register(MESSAGE_TYPE_DRIVER_NE2000_PCI,
			  MESSAGE_SIZE_DRIVER_NE2000_PCI);
}

NE2000_PCI_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
ne2000_pci_sendpkt(i_task ne2000_pci, t_id dev, uint8_t* mac, uint8_t* packet,
		   uint16_t size, uint16_t proto)
{
  t_driver_ne2000_pci_message*	message;
  i_node			ne2000_pci_driver;

  if (size > ETH_DATA_LEN)
    return (ERROR_UNKNOWN);

  ne2000_pci_driver.machine = 0;
  ne2000_pci_driver.task = ne2000_pci;

  if ((message = malloc(sizeof (*message) + size)) == NULL)
    return (ERROR_UNKNOWN);

  message->u.request.operation = NE2000_PCI_DRIVER_SENDPKT;
  message->u.request.u.sendpkt.dev = dev;
  memcpy(message->u.request.u.sendpkt.mac, mac, ETH_ALEN);
  memcpy(message->u.request.u.sendpkt.packet, packet, size);
  message->u.request.u.sendpkt.size = size;
  message->u.request.u.sendpkt.proto = proto;

  if (message_send(ne2000_pci_driver,
		   MESSAGE_TYPE_DRIVER_NE2000_PCI,
		   (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    {
      free(message);
      return (ERROR_UNKNOWN);
    }

  free(message);

  return (ERROR_NONE);
}

#endif

#endif
