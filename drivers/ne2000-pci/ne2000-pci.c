/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/ne2000-pci/ne2000-pci.c
 *
 * created       matthieu bucchianeri   [sat jun  9 22:14:19 2007]
 * updated       matthieu bucchianeri   [sat jun  9 23:44:45 2007]
 */

/*
 * Inspired by net-ne2000 driver from MutekH by Matthieu Bucchianeri
 *					<matthieu.bucchianeri@epita.fr>
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>
#include <libkaneton.h>
#include <pthread.h>
#include <sys/queue.h>
#include "ne2000-pci.h"
#include "ne2000-pci-driver.h"

#include <sys/io.h>

/*
 * ---------- dependencies ----------------------------------------------------
 */

#define PCI_SPAWN_INTERFACE
#include "../pci/pci-driver.h"
#define MOD_SPAWN_INTERFACE
#include "../../services/mod/mod-service.h"
#define INET_SPAWN_INTERFACE
#include "../../services/inet/inet-service.h"

/* XXX beurk */
t_error			ibmpc_irq_acknowledge(uint8_t		irq)
{
  if (irq > 15)
    return (ERROR_UNKNOWN);

  if (irq < 8)
    {
      outb(0x60 + irq, 0x20);
    }
  else
    {
      outb(0x60 + (irq - 8), 0xa0);
      outb(0x60 + 2, 0x20);
    }

  return (ERROR_NONE);
}
/* ---*/

/*
 * ---------- macros ----------------------------------------------------------
 */

#define LOOKUP_INET()							\
  if (inet == ID_UNUSED)						\
    {									\
      if (mod_get_service(_crt_get_mod_id(), SERVICE_INET_NAME, &inet) != \
	  ERROR_NONE)							\
	{								\
	  printf(" -- ne2000-pci: cannot find inet service\n");		\
	  while (1)							\
	    ;								\
	}								\
    }

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * pci identifiers of compatible cards.
 */

static const t_driver_pci_ids	net_ne2000[] =
  {
    { .vendor = 0x10ec, .device = 0x8029, .string = "Realtek 8029" },
    { .vendor = 0x1050, .device = 0x0940, .string = "Winbond 89C940" },
    { .vendor = 0x1050, .device = 0x5a5a, .string = "Winbond 89C940F" },
    { .vendor = 0x8c4a, .device = 0x1980, .string = "Winbond 89C940" },
    { .vendor = 0x11f6, .device = 0x1401, .string = "Compex ReadyLink 2000" },
    { .vendor = 0x8e2e, .device = 0x3000, .string = "KTI ET32P2" },
    { .vendor = 0x4a14, .device = 0x5000, .string = "NetVin NV5000SC" },
    { .vendor = 0x12c3, .device = 0x0058, .string = "HolTek HT80232" },
    { .vendor = 0x1106, .device = 0x0926, .string = "Via 86C926" },
    { .vendor = 0x10bd, .device = 0x0e34, .string = "SureCom NE34" },
  };

/*
 * bound inet service.
 */

static i_task	inet = ID_UNUSED;

/*
 * update the command register.
 */

static void	ne2000_command(t_driver_ne2000_dev*	dev,
			       uint8_t			cmd)
{
  uint16_t	addr = dev->addr + NE2000_CMD;

  outb(inb(addr) | cmd, addr);
}

/*
 * update the command register for dma operations.
 */

static void	ne2000_dma(t_driver_ne2000_dev*	dev,
			   uint8_t		cmd)
{
  uint16_t	addr = dev->addr + NE2000_CMD;

  outb((inb(addr) & ~NE2000_DMA_MASK) | cmd, addr);
}

/*
 * update the command register for page selection.
 */

static void	ne2000_page(t_driver_ne2000_dev*dev,
			    uint8_t		cmd)
{
  uint16_t	addr = dev->addr + NE2000_CMD;

  outb((inb(addr) & ~NE2000_PG_MASK) | cmd, addr);
}

/*
 * read from the device's memory.
 */

static void	ne2000_mem_read(t_driver_ne2000_dev*	dev,
				uint16_t		offs,
				void*			dst,
				uint16_t		size)
{
  /* select register bank 0 */
  ne2000_page(dev, NE2000_P0);

  /* ensure dma operations are reset */
  ne2000_dma(dev, NE2000_DMA_ABRT);
  outb(NE2000_RDC, dev->addr + NE2000_ISR);

  /* setup size */
  outb(size, dev->addr + NE2000_RBCR0);
  outb(size >> 8, dev->addr + NE2000_RBCR1);

  /* setup position */
  outb(offs, dev->addr + NE2000_RSAR0);
  outb(offs >> 8, dev->addr + NE2000_RSAR1);

  /* start read operation */
  ne2000_dma(dev, NE2000_DMA_RD);

  /* copy the whole packet */
  if (dev->io_16)
    {
      uint16_t*	d = (uint16_t*)dst;
      uint16_t	size_w = size >> 1;

      while (size_w--)
	{
	  *d++ = inw(dev->addr + NE2000_DATA);
	}
      if (size & 0x1)
	{
	  *(uint8_t*)d = inb(dev->addr + NE2000_DATA);
	}
    }
  else
    {
      uint8_t*	d = dst;

      while (size--)
	{
	  *d++ = inb(dev->addr + NE2000_DATA);
	}
    }
}

/*
 * init a remote dma writing transfer.
 */

static void	ne2000_dma_init_write(t_driver_ne2000_dev*	dev,
				      uint16_t			offs,
				      uint16_t			size)
{
  /* select register bank 0 */
  ne2000_page(dev, NE2000_P0);

  /* ensure dma operations are reset */
  ne2000_dma(dev, NE2000_DMA_ABRT);
  outb(NE2000_RDC, dev->addr + NE2000_ISR);

  /* setup size */
  outb(size, dev->addr + NE2000_RBCR0);
  outb(size >> 8, dev->addr + NE2000_RBCR1);

  /* setup position */
  outb(offs, dev->addr + NE2000_RSAR0);
  outb(offs >> 8, dev->addr + NE2000_RSAR1);

  /* start write operation */
  ne2000_dma(dev, NE2000_DMA_WR);
}

/*
 * output data for remote dma.
 */

static void	ne2000_dma_do_write(t_driver_ne2000_dev*	dev,
				    void*			src,
				    uint16_t			size)
{
  /* copy the whole packet */
  if (dev->io_16)
    {
      uint16_t*	d = (uint16_t*)src;

      if (size % 2)
	size++;

      size >>= 1;
      while (size--)
	outw(*d++, dev->addr + NE2000_DATA);
    }
  else
    {
      uint8_t*	d = src;

      while (size--)
	{
	  outb(*d++, dev->addr + NE2000_DATA);
	}
    }
}

/*
 * try reading and writing a stupid sentence to memory.
 */

static int	ne2000_rw_test(t_driver_ne2000_dev*	dev)
{
  uint8_t	endian;
  uint32_t	timeout = 2000;
  char		ref[] = "kaneton NE2000 Driver";
  char		buf[sizeof (ref)];

  /* configure the device for the test */
  /* send a reset signal */
  outb(inb(dev->addr + NE2000_RESET), dev->addr + NE2000_RESET);

  /* wait a moment */
  while (timeout)
    timeout--;
  timeout = 200000;

  /* stop completely the device */
  outb(NE2000_P0 | NE2000_DMA_ABRT | NE2000_STP, dev->addr + NE2000_CMD);

  endian = NE2000_LE;

  /* setup data configuration registers */
  if (dev->io_16)
    outb(NE2000_16BITS | endian | NE2000_NORMAL | NE2000_FIFO4,
	 dev->addr + NE2000_DCR);
  else
    outb(NE2000_8BITS | endian | NE2000_NORMAL | NE2000_FIFO4,
	 dev->addr + NE2000_DCR);
  /* setup transmit and receive in loopback */
  outb(NE2000_MONITOR, dev->addr + NE2000_RCR);
  /* setup rw ring */
  outb(dev->tx_buf, dev->addr + NE2000_PSTART);
  outb(dev->mem, dev->addr + NE2000_PSTOP);

  /* start the device */
  outb(NE2000_P0 | NE2000_DMA_ABRT | NE2000_STA, dev->addr + NE2000_CMD);

  /* do the r/w test */
  ne2000_dma_init_write(dev, dev->tx_buf << 8, sizeof (ref));
  ne2000_dma_do_write(dev, ref, sizeof (ref));
  while (timeout && !(inb(dev->addr + NE2000_ISR) & NE2000_RDC))
    timeout--;
  memset(buf, 0, sizeof (ref));
  ne2000_mem_read(dev, dev->tx_buf << 8, buf, sizeof (ref));

  return !memcmp(ref, buf, sizeof (ref));
}

/*
 * probe device capabilities.
 */

static int	ne2000_probe(t_driver_ne2000_dev*	dev)
{
  uint8_t			buf[ETH_ALEN * 2];
  int				i;

  /* try 16 bits mode with 32k */
  dev->io_16 = 1;
  dev->mem = NE2000_MEM_32K;
  dev->tx_buf = NE2000_MEM_16K;
  dev->rx_buf = NE2000_MEM_16K + NE2000_TX_BUFSZ;

  if (ne2000_rw_test(dev))
    goto ok;

  /* try 16 bits mode with 16k */
  dev->io_16 = 1;
  dev->mem = NE2000_MEM_16K;
  dev->tx_buf = NE2000_MEM_8K;
  dev->rx_buf = NE2000_MEM_8K + NE2000_TX_BUFSZ;

  if (ne2000_rw_test(dev))
    goto ok;

  /* try 8 bits mode with 32k */
  dev->io_16 = 0;
  dev->mem = NE2000_MEM_32K;
  dev->tx_buf = NE2000_MEM_16K;
  dev->rx_buf = NE2000_MEM_16K + NE2000_TX_BUFSZ;

  if (ne2000_rw_test(dev))
    goto ok;

  /* try 8 bits mode with 16k */
  dev->io_16 = 0;
  dev->mem = NE2000_MEM_16K;
  dev->tx_buf = NE2000_MEM_8K;
  dev->rx_buf = NE2000_MEM_8K + NE2000_TX_BUFSZ;

  if (ne2000_rw_test(dev))
    goto ok;

  /* all configuration failed */
  return 0;

 ok:
  /* everything ok */

  /* determine mac address, the first 6 bytes/words of the prom */
  ne2000_mem_read(dev, 0, buf, ETH_ALEN * 2);

  if (dev->io_16)
    for (i = 0; i < ETH_ALEN; i++)
      dev->mac[i] = buf[i << 1];
  else
    for (i = 0; i < ETH_ALEN; i++)
      dev->mac[i] = buf[i];

  return 1;
}

/*
 * init device. refer to the 8390 documentation for this sequence.
 */

static void	ne2000_init(t_driver_ne2000_dev*	dev)
{
  uint8_t	endian;
  int		i;

  /* stop completely the device */
  outb(NE2000_P0 | NE2000_DMA_ABRT | NE2000_STP, dev->addr + NE2000_CMD);

  endian = NE2000_LE;

  /* setup data configuration registers */
  if (dev->io_16)
    outb(NE2000_16BITS | endian | NE2000_NORMAL | NE2000_FIFO4,
	 dev->addr + NE2000_DCR);
  else
    outb(NE2000_8BITS | endian | NE2000_NORMAL | NE2000_FIFO4,
	 dev->addr + NE2000_DCR);
  /* clear dma state */
  outb(0, dev->addr + NE2000_RBCR0);
  outb(0, dev->addr + NE2000_RBCR1);
  /* setup receive configuration register */
  outb(NE2000_REJECT_ON_ERROR | NE2000_ACCEPT_BCAST | NE2000_REJECT_MCAST,
       dev->addr + NE2000_RCR);
  /* enter loopback mode */
  outb(0x2, dev->addr + NE2000_TCR);
  /* initialize tx and rx buffers */
  outb(dev->tx_buf, dev->addr + NE2000_TPSR);
  outb(dev->rx_buf, dev->addr + NE2000_PSTART);
  outb(dev->mem, dev->addr + NE2000_PSTOP);
  outb(dev->rx_buf, dev->addr + NE2000_BOUND);
  /* clear isr */
  outb(0xff, dev->addr + NE2000_ISR);
  /* activate interrupts */
  outb(NE2000_PRXE | NE2000_PTXE | NE2000_TXEE | NE2000_OVWE | NE2000_RDCE,
       dev->addr + NE2000_IMR);
  /* init mac */
  ne2000_page(dev, NE2000_P1);
  for (i = 0; i < ETH_ALEN; i++)
    outb(dev->mac[i], dev->addr + NE2000_PAR + i);
  for (i = 0; i < ETH_ALEN; i++)
    outb(0xff, dev->addr + NE2000_MAR + i);
  /* init current receive buffer pointer */
  outb(dev->rx_buf + 1, dev->addr + NE2000_CURR);
  /* bring the device up */
  outb(NE2000_P0 | NE2000_DMA_ABRT | NE2000_STA,
       dev->addr + NE2000_CMD);
  /* setup transmit configuration register */
  outb(NE2000_AUTOCRC, dev->addr + NE2000_TCR);
  /* clear isr */
  outb(0xff, dev->addr + NE2000_ISR);
}

/*
 * send a packet
 */

static void	ne2000_send(t_driver_ne2000_dev*	device,
			    char*			mac,
			    char*			packet,
			    uint16_t			size,
			    uint16_t			proto)
{
  /* take lock */
  IA32_SPIN_LOCK(device->lock);

  /* if the device is busy */
  if (inb(device->addr + NE2000_CMD) & NE2000_TXP ||
      device->current.busy)
    {
      /* queue the packet */
      struct __packetqueue_entry*	e;
      void*				copy;

      if ((copy = malloc(size)) == NULL)
	goto leave;

      memcpy(copy, packet, size);

      if ((e = malloc(sizeof (*e))) == NULL)
	{
	  free(copy);
	  goto leave;
	}

      e->packet = packet;
      e->size = size;
      memcpy(e->mac, mac, ETH_ALEN);
      e->proto = proto;

      SIMPLEQ_INSERT_TAIL(&device->sendqueue, e, entry);
    }
  else
    {
      struct ether_header	header;

      /* otherwise, send the datagram immediately */
      device->current.busy = 1;
      device->current.size = size + sizeof (struct ether_header);
      memcpy(header.ether_shost, device->mac, ETH_ALEN);
      memcpy(header.ether_dhost, mac, ETH_ALEN);
      header.ether_type = proto;

      /* reset the tries counter */
      device->current.tries = 0;

      /* initialize writing */
      ne2000_dma_init_write(device, device->tx_buf << 8,
			    sizeof (struct ether_header) + size);

      /* write header then data */
      ne2000_dma_do_write(device, &header,
			  sizeof (struct ether_header));
      ne2000_dma_do_write(device, packet, size);

      /* the data being written we wait for remote DMA to be completed (IRQ) */
    }

 leave:
  /* release lock */
  IA32_SPIN_UNLOCK(device->lock);
}

/*
 * read a packet from the NIC
 */

static t_error			ne2000_rx(t_driver_ne2000_dev*		dev,
					  t_service_inet_message**	data,
					  struct ether_header*		header,
					  uint16_t*			size)
{
  struct ne2000_header_s	hdr;
  uint8_t			next;
  uint8_t			curr;
  uint16_t			dma;
  uint8_t*			buf;

  /* get next packet pointer */
  next = inb(dev->addr + NE2000_BOUND) + 1;
  if (next > dev->mem)
    next = dev->rx_buf;
  ne2000_page(dev, NE2000_P1);
  curr = inb(dev->addr + NE2000_CURR);
  if (curr > dev->mem)
    curr = dev->rx_buf;
  ne2000_page(dev, NE2000_P0);
  if (curr == next)
    return (ERROR_UNKNOWN);

  /* read the packet header (automatically appended by the chip) */
  dma = next << 8;
  ne2000_mem_read(dev, dma, (struct ne2000_header_s *)&hdr,
		  sizeof (struct ne2000_header_s));

  /* read the packet itself */
  ne2000_mem_read(dev, dma + sizeof (struct ne2000_header_s),
		  header, ETH_HLEN);

  *size = hdr.size - sizeof (struct ne2000_header_s) - ETH_HLEN;

  if ((*data = malloc(sizeof (t_service_inet_message) + *size)) == NULL)
    return (ERROR_UNKNOWN);

  buf = (*data)->u.request.u.if_pushpkt.packet;

  ne2000_mem_read(dev, dma + sizeof (struct ne2000_header_s) + ETH_HLEN,
		  buf, *size);

  /* update the next packet pointer */
  next = hdr.next;
  if (next > dev->mem)
    next = dev->rx_buf;
  outb(next - 1, dev->addr + NE2000_BOUND);
  return (ERROR_NONE);
}

/*
 * push a packet into the network service
 */

static void	ne2000_push(t_driver_ne2000_dev*	dev,
			    t_service_inet_message*	message,
			    struct ether_header*	hdr,
			    uint16_t			size)
{
  i_node			inet_service;

  inet_service.machine = 0;
  inet_service.task = inet;

  message->u.request.operation = INET_SERVICE_IF_PUSHPKT;
  message->u.request.u.if_pushpkt.iface = dev->iface;
  memcpy(message->u.request.u.if_pushpkt.s_mac, dev->mac, ETH_ALEN);
  memcpy(message->u.request.u.if_pushpkt.d_mac, hdr->ether_shost, ETH_ALEN);
  message->u.request.u.if_pushpkt.mac_len = ETH_ALEN;
  message->u.request.u.if_pushpkt.proto = hdr->ether_type;
  message->u.request.u.if_pushpkt.size = size;

  if (message_send(inet_service,
		   MESSAGE_TYPE_SERVICE_INET, (t_vaddr)message,
		   sizeof (*message) + size) != ERROR_NONE)
    printf(" -- ne2000-pci: failed to push packet.\n");

  free(message);
}

/*
 * IRQ handler
 */

static void	ne2000_irq(t_driver_ne2000_dev*		dev)
{
  uint8_t	isr;
  uint8_t	tx_serviced = 0;
  uint8_t	aborted = 0;
  uint8_t	max_svc = 0;

  /* lock the device */
  IA32_SPIN_LOCK(dev->lock);

  /* select register bank 0 */
  ne2000_page(dev, NE2000_P0);

  while ((isr = inb(dev->addr + NE2000_ISR)) && max_svc++ < 6)
    {
      /* remote DMA completed */
      if (isr & NE2000_RDC)
	{
	  if (dev->current.busy)
	    {
	      uint16_t	length = dev->current.size;

	      /* the packet is in the device memory, we can send it */
	      ne2000_dma(dev, NE2000_DMA_ABRT);

	      /* set page start */
	      outb(dev->tx_buf, dev->addr + NE2000_TPSR);

	      /* set packet size */
	      if (length < ETH_ZLEN)
		length = ETH_ZLEN;
	      outb(length, dev->addr + NE2000_TBCR0);
	      outb(length >> 8, dev->addr + NE2000_TBCR1);

	      /* send it ! */
	      ne2000_command(dev, NE2000_TXP);
	    }
	}

      /* transmit error */
      if (isr & NE2000_TXE)
	{
	  /* increment the retry counter */
	  dev->current.tries++;

	  /* too many retries, abording */
	  if (dev->current.tries > 3)
	    {
	      /* set the interrupt flags as "transmitted without error",
		 so the next queued packet can be sent */
	      aborted = 1;
	    }
	  else
	    {
	      /* resend the transmit command */
	      ne2000_command(dev, NE2000_TXP);
	    }
	}

      /* packet transmitted */
      if (isr & NE2000_PTX || aborted)
	{
	  /* the big dirty test. on some device, the PTX bit of ISR is
	     not correctly acknowleged */
	  if (!tx_serviced && dev->current.busy)
	    {
	      struct __packetqueue_entry*	e;

	      /* packet sent successfully, drop it */
	      dev->current.busy = 0;

	      /* one or more packets in the wait queue ? */
	      if ((e = SIMPLEQ_FIRST(&dev->sendqueue)) != NULL)
		{
		  SIMPLEQ_REMOVE_HEAD(&dev->sendqueue, entry);

		  ne2000_send(dev, e->mac, e->packet, e->size, e->proto);

		  free(e->packet);
		  free(e);
		}

	      tx_serviced = 1;
	      aborted = 0;
	    }
	}

      /* buffer full */
      if (isr & NE2000_OVW)
	{
	  uint8_t	cr;
	  uint8_t	resend = 0;
	  uint16_t	total;

	  /* save the NIC state */
	  cr = inb(dev->addr + NE2000_CMD);

	  /* stop completely the device */
	  outb(NE2000_P0 | NE2000_DMA_ABRT | NE2000_STP,
	       dev->addr + NE2000_CMD);

	  /* wait for the NIC to complete operations */
	  /* usleep(1600); XXX */

	  /* reset remote byte count registers */
	  outb(0, dev->addr + NE2000_RBCR0);
	  outb(0, dev->addr + NE2000_RBCR1);

	  /* do we need to resend the outgoing packet ? */
	  resend = (cr & NE2000_TXP) && !((isr & NE2000_PTX) || (isr & NE2000_TXE));

	  /* enter loopback mode */
	  outb(0x2, dev->addr + NE2000_TCR);

	  /* bring the device up */
	  outb(NE2000_P0 | NE2000_DMA_ABRT | NE2000_STA,
	       dev->addr + NE2000_CMD);

	  /* read some packets until half the memory is free */
	  for (total = 0; total < ((dev->mem - dev->rx_buf) << 8) / 2; )
	    {
	      uint16_t			size;
	      t_service_inet_message*	data;
	      struct ether_header	header;

	      /* read a packet */
	      if (ne2000_rx(dev, &data, &header, &size) != ERROR_NONE)
		break;

	      /* push the packet into the network stack */
	      ne2000_push(dev, data, &header, size);

	      total += size;
	    }

	  /* force ISR reset */
	  outb(isr, dev->addr + NE2000_ISR);

	  /* setup transmit configuration register (disable loopback) */
	  outb(NE2000_AUTOCRC, dev->addr + NE2000_TCR);

	  /* if a transmission was aborted, restart it */
	  if (resend)
	    ne2000_command(dev, NE2000_PTX);
	}

      /* packet received */
      if (isr & NE2000_PRX)
	{
	  /* no errors */
	  if (inb(dev->addr + NE2000_RSR) & NE2000_SPRX)
	    {
	      uint16_t			size;
	      t_service_inet_message*	data;
	      struct ether_header	header;

	      /* read the packet */
	      while (ne2000_rx(dev, &data, &header, &size) == ERROR_NONE)
		{
		  /* push the packet into the network stack */
		  ne2000_push(dev, data, &header, size);
		}
	    }
	}

      outb(isr, dev->addr + NE2000_ISR);
    }

  /* release lock */
  IA32_SPIN_UNLOCK(dev->lock);
}

/*
 * IRQ server thread
 */

static void*	ne2000_pci_irq_serve(void*		device)
{
  o_event_message	evt;
  t_vsize		sz;
  i_node		sender;

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_EVENT, (t_vaddr)&evt, &sz, &sender) ==
	  ERROR_NONE)
	{
	  ne2000_irq((t_driver_ne2000_dev*)evt.data);

	  ibmpc_irq_acknowledge(evt.id - 32);
	}
    }

  return (NULL);
}

/*
 * initialize a ne2000 device.
 */

static void		ne2000_pci_init(t_driver_pci_dev*	device,
					void*			pv)
{
  t_driver_ne2000_dev*	dev;
  char*			string = (char*)pv;
  int			i;

  printf(" -- ne2000-pci: found new device.\n");
  printf(" -- ne2000-pci: PCI ID %04x:%04x, IO 0x%x, IRQ %d\n",
	 device->vendor, device->device, device->io[0], device->irq);

  for (i = 0; i < 0x20; i++)
    {
      if (io_grant(device->io[0] + i, _crt_get_task_id(), 1) != ERROR_NONE)
	{
	  printf(" -- net2000-pci: cannot acquire i/o ports. abording.\n");

	  return;
	}
    }

  if ((dev = malloc(sizeof (t_driver_ne2000_dev))) == NULL)
    {
      printf(" -- ne2000-pci: error, no more memory.\n");
      return;
    }

  dev->addr = device->io[0];
  memset(&dev->current, 0, sizeof (dev->current));

  if (!ne2000_probe(dev))
    {
      free(dev);
      printf(" -- ne2000-pci: error, cannot identify device.\n");
      return;
    }

  if (event_reserve(32 + device->irq, EVENT_MESSAGE,
		    EVENT_TASK(_crt_get_task_id()),
		    (t_vaddr)dev) != ERROR_NONE)
    {
      free(dev);
      printf(" -- ne2000-pci: error, cannot register IRQ.\n");
      return;
    }

  if (pthread_create(&dev->irq_thread, NULL, ne2000_pci_irq_serve, &dev))
    {
      event_release(device->irq);
      free(dev);
      printf(" -- ne2000-pci: error, cannot create IRQ server.\n");
      return;
    }

  ne2000_init(dev);

  printf(" -- ne2000-pci: is a %s (%d bits) with %d kb of RAM\n",
	 string, dev->io_16 ? 16 : 8, dev->mem << 8);
  printf(" -- ne2000-pci: MAC %02x:%02x:%02x:%02x:%02x:%02x\n", dev->mac[0],
	 dev->mac[1], dev->mac[2], dev->mac[3], dev->mac[4], dev->mac[5]);

  LOOKUP_INET();
  inet_if_register(inet, (t_id)(t_vaddr)dev, INET_IF_TYPE_ETHERNET, dev->mac,
		   ETH_ALEN, ETH_DATA_LEN, &dev->iface);
}

/*
 * probe the system for ne2000 devices.
 */

static void    	ne2000_pci_probe(void)
{
  int		i;
  i_task	pci;

  printf(" -- ne2000-pci: probing devices.\n");

  if (mod_get_service(_crt_get_mod_id(), DRIVER_PCI_NAME, &pci) != ERROR_NONE)
    {
      printf(" -- ne2000-pci: cannot find pci driver.\n");
      return ;
    }

  for (i = 0; i < sizeof (net_ne2000) / sizeof (t_driver_pci_ids); i++)
    {
      if (pci_get_device(pci, net_ne2000[i].vendor, net_ne2000[i].device,
			 ne2000_pci_init, (char*)net_ne2000[i].string) != ERROR_NONE)
	printf(" -- ne2000-pci: error while probing.\n");
    }
}

/*
 * serve requests.
 */

static int	ne2000_pci_driver_serve(void)
{
  t_driver_ne2000_pci_message*	message;
  i_node			sender;
  t_vsize			size;

  if ((message = malloc(sizeof (*message) + ETH_DATA_LEN)) == NULL)
    {
      printf(" -- ne2000-pci: memory exhausted\n");
      return (-1);
    }

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_DRIVER_NE2000_PCI,
			  (t_vaddr)message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  if (message->u.request.operation == NE2000_PCI_DRIVER_SENDPKT)
	    {
	      t_driver_ne2000_dev*	dev;

	      /* XXX use id rather than pointers */
	      dev = (t_driver_ne2000_dev*)(t_vaddr)message->u.request.u.sendpkt.dev;

	      ne2000_send(dev,
			  message->u.request.u.sendpkt.mac,
			  message->u.request.u.sendpkt.packet,
			  message->u.request.u.sendpkt.size,
			  message->u.request.u.sendpkt.proto);
	    }
	}
    }

  return (0);
}

/*
 * driver entry point.
 */

int	main(void)
{
  if (message_register(MESSAGE_TYPE_DRIVER_NE2000_PCI,
		       MESSAGE_SIZE_DRIVER_NE2000_PCI) != ERROR_NONE)
    return (-1);

  printf(" -- ne2000-pci: NE2000-PCI driver started.\n");

  /* init dependencies */
  mod_init();
  pci_init();
  inet_init();

  /* XXX to be moved */
  io_grant(0x20, _crt_get_task_id(), 1);
  io_grant(0xa0, _crt_get_task_id(), 1);

  ne2000_pci_probe();

  ne2000_pci_driver_serve();

  return 0;
}
