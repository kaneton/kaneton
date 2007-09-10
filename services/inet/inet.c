/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license
 *
 * file          /home/buckman/crypt/kaneton/services/inet/inet.c
 *
 * created       matthieu bucchianeri   [fri sep  7 23:49:49 2007]
 * updated       matthieu bucchianeri   [mon sep 10 18:18:13 2007]
 */

#include <stdio.h>
#include <hexo/device.h>
#include <netinet/packet.h>
#include <netinet/if.h>

#include "inet-service.h"

#define MOD_SPAWN_INTERFACE
#include "../mod/mod-service.h"
#define NE2000_PCI_SPAWN_INTERFACE
#include "../../drivers/ne2000-pci/ne2000-pci-driver.h"

void timer_init(void);

/*
 * service incoming requests.
 */

static int	inet_serve(void)
{
  t_service_inet_message*	message;
  i_node			sender;
  t_vsize			size;

  if ((message = malloc(sizeof (*message) + 65536)) == NULL)
    {
      printf(" -- inet: memory exhausted\n");
      return (-1);
    }

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_SERVICE_INET,
			  (t_vaddr)message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  if (message->u.request.operation == INET_SERVICE_IF_REGISTER)
	    {
	      struct device_s*	dev;
	      void*		mac;
	      net_if_type_t	type;

	      dev = (struct device_s*)(t_vaddr)message->u.request.u.if_register.dev;

	      if ((mac = malloc(message->u.request.u.if_register.mac_len)) == NULL)
		{
		  /* XXX mega echec */
		}

	      memcpy(mac, message->u.request.u.if_register.mac,
		     message->u.request.u.if_register.mac_len);

	      type = IF_ETHERNET; /* XXX */

	      message->u.reply.u.if_register.iface = (t_id)(t_vaddr)
		if_register(dev,
			    type,
			    mac,
			    message->u.request.u.if_register.mtu);

	      if (message_send(sender,
			       MESSAGE_TYPE_SERVICE_INET,
			       (t_vaddr)message,
			       sizeof (*message)) != ERROR_NONE)
		{
		  printf(" -- inet: error in request\n");
		}
	    }
	}
    }

  return (0);
}

/*
 * service entry point.
 */

int	main(void)
{
  if (message_register(MESSAGE_TYPE_SERVICE_INET,
		       MESSAGE_SIZE_SERVICE_INET) != ERROR_NONE)
    return (-1);

  printf(" -- inet: TCP/IP service started.\n");

  /* hexo timer emulation layer */
  timer_init();

  ne2000_pci_init();

  inet_serve();

  return (0);
}

/*
 * send packet wrapper.
 */

void	inet_send_packet(struct device_s*	dev,
			 struct net_packet_s*	packet,
			 uint16_t		proto)
{
  static i_task	ne2000 = ID_UNUSED;

  if (ne2000 == ID_UNUSED)
    {
      if (mod_get_service(_crt_get_mod_id(),
			  DRIVER_NE2000_PCI_NAME,
			  &ne2000) != ERROR_NONE)
	{
	  printf(" -- inet: cannot find driver.\n");
	  return ;
	}
    }

  ne2000_pci_sendpkt(ne2000,
		     (t_id)(t_vaddr)dev,
		     (uint8_t*)packet->tMAC,
		     packet->header[0].data,
		     packet->header[0].size,
		     packet->proto);
}
