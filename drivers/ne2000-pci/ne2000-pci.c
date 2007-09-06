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

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#include <pthread.h>
#include <queue.h>
#include "ne2000-pci.h"
#include "ne2000-pci-driver.h"

/*
 * ---------- dependencies ----------------------------------------------------
 */

#define PCI_SPAWN_INTERFACE
#define PCI_INLINE_INTERFACE
#include "../pci/pci-driver.h"
#define MOD_SPAWN_INTERFACE
#define MOD_INLINE_INTERFACE
#include "../../services/mod/mod-service.h"

/* XXX elsewhere */

#define		OUTB(_port_, _data_)					\
  asm volatile("outb %%al, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		OUTW(_port_, _data_)					\
  asm volatile("outw %%ax, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		INB(_port_, _data_)					\
  asm volatile("inb %%dx, %%al\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))

#define		INW(_port_, _data_)					\
  asm volatile("inw %%dx, %%ax\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))

t_error			ibmpc_irq_acknowledge(uint8_t		irq)
{
  if (irq > 15)
    return (ERROR_UNKNOWN);

  if (irq < 8)
    {
      OUTB(0x20, 0x60 + irq);
    }
  else
    {
      OUTB(0xa0, 0x60 + (irq - 8));
      OUTB(0x20, 0x60 + 2);
    }

  return (ERROR_NONE);
}
/* ---*/

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
 * update the command register.
 */

static void	ne2000_command(t_driver_ne2000_dev*	dev,
			       uint8_t			cmd)
{
  uint16_t	addr = dev->addr + NE2000_CMD;
  uint8_t	val;

  INB(addr, val);
  OUTB(addr, val | cmd);
}

/*
 * update the command register for dma operations.
 */

static void	ne2000_dma(t_driver_ne2000_dev*	dev,
			   uint8_t		cmd)
{
  uint16_t	addr = dev->addr + NE2000_CMD;
  uint8_t	val;

  INB(addr, val);
  OUTB(addr, (val & ~NE2000_DMA_MASK) | cmd);
}

/*
 * update the command register for page selection.
 */

static void	ne2000_page(t_driver_ne2000_dev*dev,
			    uint8_t		cmd)
{
  uint16_t	addr = dev->addr + NE2000_CMD;
  uint8_t	val;

  INB(addr, val);
  OUTB(addr, (val & ~NE2000_PG_MASK) | cmd);
}

/*
 * read from the device's memory.
 */

static void	ne2000_mem_read(t_driver_ne2000_dev*	dev,
				uint16_t		offs,
				void*			dst,
				uint16_t		size)
{
  uint16_t	val16;
  uint8_t	val8;

  /* select register bank 0 */
  ne2000_page(dev, NE2000_P0);

  /* ensure dma operations are reset */
  ne2000_dma(dev, NE2000_DMA_ABRT);
  OUTB(dev->addr + NE2000_ISR, NE2000_RDC);

  /* setup size */
  OUTB(dev->addr + NE2000_RBCR0, size);
  OUTB(dev->addr + NE2000_RBCR1, size >> 8);

  /* setup position */
  OUTB(dev->addr + NE2000_RSAR0, offs);
  OUTB(dev->addr + NE2000_RSAR1, offs >> 8);

  /* start read operation */
  ne2000_dma(dev, NE2000_DMA_RD);

  /* copy the whole packet */
  if (dev->io_16)
    {
      uint16_t*	d = (uint16_t*)dst;
      uint16_t	size_w = size >> 1;

      while (size_w--)
	{
	  INW(dev->addr + NE2000_DATA, val16);
	  *d++ = val16;
	}
      if (size & 0x1)
	{
	  INB(dev->addr + NE2000_DATA, val8);
	  *(uint8_t*)d = val8;
	}
    }
  else
    {
      uint8_t*	d = dst;

      while (size--)
	{
	  INB(dev->addr + NE2000_DATA, val8);
	  *d++ = val8;
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
  OUTB(dev->addr + NE2000_ISR, NE2000_RDC);

  /* setup size */
  OUTB(dev->addr + NE2000_RBCR0, size);
  OUTB(dev->addr + NE2000_RBCR1, size >> 8);

  /* setup position */
  OUTB(dev->addr + NE2000_RSAR0, offs);
  OUTB(dev->addr + NE2000_RSAR1, offs >> 8);

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
	OUTW(dev->addr + NE2000_DATA, *d++);
    }
  else
    {
      uint8_t*	d = src;

      while (size--)
	{
	  OUTB(dev->addr + NE2000_DATA, *d++);
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
  uint8_t	val;

  /* configure the device for the test */
  /* send a reset signal */
  INB(dev->addr + NE2000_RESET, val);
  OUTB(dev->addr + NE2000_RESET, val);

  /* wait a moment */
  while (timeout)
    timeout--;
  timeout = 200000;

  /* stop completely the device */
  OUTB(dev->addr + NE2000_CMD, NE2000_P0 | NE2000_DMA_ABRT | NE2000_STP);

  endian = NE2000_LE;

  /* setup data configuration registers */
  if (dev->io_16)
    OUTB(dev->addr + NE2000_DCR,
	 NE2000_16BITS | endian | NE2000_NORMAL | NE2000_FIFO4);
  else
    OUTB(dev->addr + NE2000_DCR,
	 NE2000_8BITS | endian | NE2000_NORMAL | NE2000_FIFO4);
  /* setup transmit and receive in loopback */
  OUTB(dev->addr + NE2000_RCR, NE2000_MONITOR);
  /* setup rw ring */
  OUTB(dev->addr + NE2000_PSTART, dev->tx_buf);
  OUTB(dev->addr + NE2000_PSTOP, dev->mem);

  /* start the device */
  OUTB(dev->addr + NE2000_CMD, NE2000_P0 | NE2000_DMA_ABRT | NE2000_STA);

  /* do the r/w test */
  ne2000_dma_init_write(dev, dev->tx_buf << 8, sizeof (ref));
  ne2000_dma_do_write(dev, ref, sizeof (ref));
  while (timeout && !(val & NE2000_RDC))
    {
      INB(dev->addr + NE2000_ISR, val);
      timeout--;
    }
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
  OUTB(dev->addr + NE2000_CMD, NE2000_P0 | NE2000_DMA_ABRT | NE2000_STP);

  endian = NE2000_LE;

  /* setup data configuration registers */
  if (dev->io_16)
    OUTB(dev->addr + NE2000_DCR,
	 NE2000_16BITS | endian | NE2000_NORMAL | NE2000_FIFO4);
  else
    OUTB(dev->addr + NE2000_DCR,
	 NE2000_8BITS | endian | NE2000_NORMAL | NE2000_FIFO4);
  /* clear dma state */
  OUTB(dev->addr + NE2000_RBCR0, 0);
  OUTB(dev->addr + NE2000_RBCR1, 0);
  /* setup receive configuration register */
  OUTB(dev->addr + NE2000_RCR,
       NE2000_REJECT_ON_ERROR | NE2000_ACCEPT_BCAST | NE2000_REJECT_MCAST);
  /* enter loopback mode */
  OUTB(dev->addr + NE2000_TCR, 0x2);
  /* initialize tx and rx buffers */
  OUTB(dev->addr + NE2000_TPSR, dev->tx_buf);
  OUTB(dev->addr + NE2000_PSTART, dev->rx_buf);
  OUTB(dev->addr + NE2000_PSTOP, dev->mem);
  OUTB(dev->addr + NE2000_BOUND, dev->rx_buf);
  /* clear isr */
  OUTB(dev->addr + NE2000_ISR, 0xff);
  /* activate interrupts */
  OUTB(dev->addr + NE2000_IMR,
		 NE2000_PRXE | NE2000_PTXE | NE2000_TXEE | NE2000_OVWE |
		 NE2000_RDCE);
  /* init mac */
  ne2000_page(dev, NE2000_P1);
  for (i = 0; i < ETH_ALEN; i++)
    OUTB(dev->addr + NE2000_PAR + i, dev->mac[i]);
  for (i = 0; i < ETH_ALEN; i++)
    OUTB(dev->addr + NE2000_MAR + i, 0xff);
  /* init current receive buffer pointer */
  OUTB(dev->addr + NE2000_CURR, dev->rx_buf + 1);
  /* bring the device up */
  OUTB(dev->addr + NE2000_CMD,
		 NE2000_P0 | NE2000_DMA_ABRT | NE2000_STA);
  /* setup transmit configuration register */
  OUTB(dev->addr + NE2000_TCR, NE2000_AUTOCRC);
  /* clear isr */
  OUTB(dev->addr + NE2000_ISR, 0xff);
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
  uint8_t	st;

  /* take lock */
  IA32_SPIN_LOCK(device->lock);

  /* if the device is busy */
  INB(device->addr + NE2000_CMD, st);
  if ((st & NE2000_TXP) ||
      device->current.busy)
    {
      /* queue the packet */
      struct __packetqueue_entry*	e;

      if ((e = malloc(sizeof (*e))) == NULL)
	{
	  free(packet);
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
      device->current.size = size;
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

static t_error			ne2000_rx(t_driver_ne2000_dev*	dev,
					  uint8_t**		data,
					  uint16_t*		size)
{
  struct ne2000_header_s	hdr;
  uint8_t			next;
  uint8_t			curr;
  uint16_t			dma;
  uint8_t*			buf;

  /* get next packet pointer */
  INB(dev->addr + NE2000_BOUND, next);
  next++;
  if (next > dev->mem)
    next = dev->rx_buf;
  ne2000_page(dev, NE2000_P1);
  INB(dev->addr + NE2000_CURR, curr);
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
  *size = hdr.size - sizeof (struct ne2000_header_s);

  if ((buf = *data = malloc(*size)) == NULL)
    return (ERROR_UNKNOWN);
  ne2000_mem_read(dev, dma + sizeof (struct ne2000_header_s),
		  buf, *size);

  /* update the next packet pointer */
  next = hdr.next;
  if (next > dev->mem)
    next = dev->rx_buf;
  OUTB(dev->addr + NE2000_BOUND, next - 1);
  return (ERROR_NONE);
}

/*
 * push a packet into the network service
 */

static void	ne2000_push(t_driver_ne2000_dev*	dev,
			    uint8_t*			data,
			    uint16_t			size)
{
  struct ether_header*		hdr;

  /* get the good header */
  hdr = (struct ether_header *)data;

  /* fill some info */
  printf(" -- ne2000-pci: new packet %02x:%02x:%02x:%02x:%02x:%02x of %d\n",
	 hdr->ether_shost[0], hdr->ether_shost[1], hdr->ether_shost[2],
	 hdr->ether_shost[3], hdr->ether_shost[4], hdr->ether_shost[5],
	 size);

  /* XXX */

  free(data);
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

  while (1)
    {
      INB(dev->addr + NE2000_ISR, isr);

      if (!isr || max_svc++ > 6)
	break;

      /* remote DMA completed */
      if (isr & NE2000_RDC)
	{
	  if (dev->current.busy)
	    {
	      uint16_t	length = dev->current.size;

	      /* the packet is in the device memory, we can send it */
	      ne2000_dma(dev, NE2000_DMA_ABRT);

	      /* set page start */
	      OUTB(dev->addr + NE2000_TPSR, dev->tx_buf);

	      /* set packet size */
	      if (length < ETH_ZLEN)
		length = ETH_ZLEN;
	      OUTB(dev->addr + NE2000_TBCR0, length);
	      OUTB(dev->addr + NE2000_TBCR1, length >> 8);

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
	  INB(dev->addr + NE2000_CMD, cr);

	  /* stop completely the device */
	  OUTB(dev->addr + NE2000_CMD,
	       NE2000_P0 | NE2000_DMA_ABRT | NE2000_STP);

	  /* wait for the NIC to complete operations */
	  /* usleep(1600); XXX */

	  /* reset remote byte count registers */
	  OUTB(dev->addr + NE2000_RBCR0, 0);
	  OUTB(dev->addr + NE2000_RBCR1, 0);

	  /* do we need to resend the outgoing packet ? */
	  resend = (cr & NE2000_TXP) && !((isr & NE2000_PTX) || (isr & NE2000_TXE));

	  /* enter loopback mode */
	  OUTB(dev->addr + NE2000_TCR, 0x2);

	  /* bring the device up */
	  OUTB(dev->addr + NE2000_CMD,
	       NE2000_P0 | NE2000_DMA_ABRT | NE2000_STA);

	  /* read some packets until half the memory is free */
	  for (total = 0; total < ((dev->mem - dev->rx_buf) << 8) / 2; )
	    {
	      uint16_t	size;
	      uint8_t*	data;

	      /* read a packet */
	      if (ne2000_rx(dev, &data, &size) != ERROR_NONE)
		break;

	      /* push the packet into the network stack */
	      ne2000_push(dev, data, size);

	      total += size;
	    }

	  /* force ISR reset */
	  OUTB(dev->addr + NE2000_ISR, isr);

	  /* setup transmit configuration register (disable loopback) */
	  OUTB(dev->addr + NE2000_TCR, NE2000_AUTOCRC);

	  /* if a transmission was aborted, restart it */
	  if (resend)
	    ne2000_command(dev, NE2000_PTX);
	}

      /* packet received */
      if (isr & NE2000_PRX)
	{
	  uint8_t	err;

	  /* no errors */
	  INB(dev->addr + NE2000_RSR, err);
	  if (err & NE2000_SPRX)
	    {
	      uint16_t	size;
	      uint8_t*	data;

	      /* read the packet */
	      while (ne2000_rx(dev, &data, &size) == ERROR_NONE)
		{
		  /* push the packet into the network stack */
		  ne2000_push(dev, data, size);
		}
	    }
	}

      OUTB(dev->addr + NE2000_ISR, isr);
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
    if (io_grant(device->io[0] + i, _crt_get_task_id(), 1) != ERROR_NONE)
      {
	printf(" -- net2000-pci: cannot acquire i/o ports. abording.\n");

	return;
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
}

/*
 * probe the system for ne2000 devices.
 */

static void    	ne2000_pci_probe(void)
{
  int		i;
  i_task	pci;

  printf(" -- ne2000-pci: probing devices.\n");

  if (mod_get_service(_crt_get_mod_id(), "pci", &pci) != ERROR_NONE)
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

static void	ne2000_pci_driver_serve(void)
{
  while (1)
    {
      /* XXX */
    }
}

/*
 * driver entry point.
 */

int	main(void)
{
  printf(" -- ne2000-pci: NE2000-PCI driver started.\n");

  /* init dependencies */
  mod_init();
  pci_init();

  /* XXX to be moved */
  io_grant(0x20, _crt_get_task_id(), 1);
  io_grant(0xa0, _crt_get_task_id(), 1);

  ne2000_pci_probe();

  ne2000_pci_driver_serve();

  return 0;
}
