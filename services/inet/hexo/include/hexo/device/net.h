#ifndef DEVICE_NET_H
#define DEVICE_NET_H

#include <hexo/types.h>
#include <hexo/device.h>
#include <netinet/packet.h>

void	inet_send_packet(struct device_s*, struct net_packet_s*, uint16_t);

#define dev_net_preparepkt(dev,packet,size,padding)			\
  ({									\
    struct net_header_s		*nethdr;				\
    uint_fast16_t		total = 0;				\
									\
    total = sizeof (struct ether_header) + size;			\
									\
    if ((packet->packet = mem_alloc(total, MEM_SCOPE_CONTEXT)) == NULL)	\
      return NULL;							\
									\
    nethdr = &packet->header[0];					\
    nethdr->data = packet->packet;					\
    nethdr->size = size;						\
									\
    packet->stage = 1;							\
									\
    packet->sMAC = interface->mac;					\
    packet->MAClen = ETH_ALEN;						\
    packet->packet;							\
  })

#define dev_net_sendpkt(dev,packet,proto)				\
  {									\
    inet_send_packet(dev, packet, proto);				\
  }

#endif
