/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license
 *
 * file          /home/buckman/kaneton/services/inet/inet.c
 *
 * created       matthieu bucchianeri   [fri sep  7 23:49:49 2007]
 * updated       matthieu bucchianeri   [tue sep 11 01:20:08 2007]
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

	      /* XXX remove me */
  struct net_addr_s	addr;
  struct net_addr_s	mask;
  struct net_addr_s	target;
  struct net_if_s	*interface;
  struct net_route_s	*route;
  if_up("eth0");
  interface = if_get_by_name("eth0");
  IPV4_ADDR_SET(addr, 0xC0A80201);
  IPV4_ADDR_SET(mask, 0xffffff00);
  if_config(interface->index, IF_SET, &addr, &mask);
  IPV4_ADDR_SET(target, 0xC0A80200);
  route = route_obj_new(NULL, &target, &mask, interface);
  route->is_routed = 0;
  route_add(route);
  if_dump("eth0");
  route_dump();

	    }
	  else if (message->u.request.operation == INET_SERVICE_IF_PUSHPKT)
	    {
	      struct net_if_s*	iface;
	      struct net_packet_s		*packet;
	      struct net_header_s		*nethdr;

	      iface = (struct net_if_s*)(t_vaddr)message->u.request.u.if_pushpkt.iface;

	      /* create a new packet object */
	      if ((packet = packet_obj_new(NULL)) == NULL ||
		  (packet->packet = malloc(message->u.request.u.if_pushpkt.size)) == NULL)
		{
		  printf(" -- inet: memory exhausted, dropping packet.\n");
		  while (1)
		    ;
		}

	      memcpy(packet->packet, message->u.request.u.if_pushpkt.packet,
		     message->u.request.u.if_pushpkt.size);

	      nethdr = &packet->header[0];
	      nethdr->data = packet->packet;
	      nethdr->size = message->u.request.u.if_pushpkt.size;

	      nethdr[1].data = nethdr->data;
	      nethdr[1].size = nethdr->size;

	      /* fill some info */
	      packet->MAClen = message->u.request.u.if_pushpkt.mac_len;
	      packet->sMAC = malloc(packet->MAClen);
	      memcpy(packet->sMAC, message->u.request.u.if_pushpkt.s_mac,
		     packet->MAClen);
	      packet->tMAC = malloc(packet->MAClen);
	      memcpy(packet->tMAC, message->u.request.u.if_pushpkt.d_mac,
		     packet->MAClen);
	      packet->proto = endian_be16(message->u.request.u.if_pushpkt.proto);

	      packet->stage = 1;

	      if_pushpkt(iface, packet);
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

  if (ne2000_pci_sendpkt(ne2000,
			 (t_id)(t_vaddr)dev,
			 (uint8_t*)packet->tMAC,
			 packet->header[0].data,
			 packet->header[0].size,
			 endian_be16(packet->proto)) != ERROR_NONE)
    printf(" -- inet: error sending packet to driver.\n");
}
