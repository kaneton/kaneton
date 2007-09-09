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
 * updated       matthieu bucchianeri   [sun sep  9 23:58:17 2007]
 */

#include <stdio.h>
#include <hexo/device.h>
#include <netinet/packet.h>

void timer_init(void);

static void	inet_serve(void)
{
  while (1)
    {
      /* XXX */
    }
}

int	main(void)
{
  printf(" -- inet: TCP/IP service started.\n");

  /* hexo timer emulation layer */
  timer_init();

  inet_serve();

  return (0);
}

void	inet_send_packet(struct device_s*	dev,
			 struct net_packet_s*	packet,
			 uint16_t		proto)
{
  /* XXX */
}
