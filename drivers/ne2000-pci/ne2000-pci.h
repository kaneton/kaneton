/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/ne2000-pci/ne2000-pci.h
 *
 * created       matthieu bucchianeri   [sat jun  9 22:14:55 2007]
 * updated       matthieu bucchianeri   [sat jun  9 23:15:54 2007]
 */

#ifndef DRIVERS_NE2000_PCI_H
# define DRIVERS_NE2000_PCI_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

# include <libkaneton.h>
# include <stdint.h>
# include <pthread.h>
# include <sys/queue.h>
# include <spinlock.h>
# include "ether.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * memory size.
 */

# define NE2000_MEM_8K	32
# define NE2000_MEM_16K	64
# define NE2000_MEM_32K	128

/*
 * send buffer size.
 */

# define NE2000_TX_BUFSZ	6

/*
 * command register bits.
 */

# define NE2000_STP	(1 << 0)
# define NE2000_STA	(1 << 1)
# define NE2000_TXP	(1 << 2)
# define NE2000_RD0	(1 << 3)
# define NE2000_RD1	(1 << 4)
# define NE2000_RD2	(1 << 5)
# define NE2000_PS0	(1 << 6)
# define NE2000_PS1	(1 << 7)

# define NE2000_DMA_MASK	(NE2000_RD0 | NE2000_RD1 | NE2000_RD2)
# define NE2000_PG_MASK		(NE2000_PS0 | NE2000_PS1)

/*
 * shortcuts for dma transfers.
 */

# define NE2000_DMA_RD		NE2000_RD0
# define NE2000_DMA_WR		NE2000_RD1
# define NE2000_DMA_SEND	(NE2000_RD1 | NE2000_RD0)
# define NE2000_DMA_ABRT	NE2000_RD2

/*
 * shortcuts for page select.
 */

# define NE2000_P0	0
# define NE2000_P1	NE2000_PS0
# define NE2000_P2	NE2000_PS1

/*
 * interrupt register bits.
 */

# define NE2000_PRX	(1 << 0)
# define NE2000_PTX	(1 << 1)
# define NE2000_RXE	(1 << 2)
# define NE2000_TXE	(1 << 3)
# define NE2000_OVW	(1 << 4)
# define NE2000_CNT	(1 << 5)
# define NE2000_RDC	(1 << 6)

/*
 * interrupt mask.
 */

# define NE2000_PRXE	(1 << 0)
# define NE2000_PTXE	(1 << 1)
# define NE2000_RXEE	(1 << 2)
# define NE2000_TXEE	(1 << 3)
# define NE2000_OVWE	(1 << 4)
# define NE2000_CNTE	(1 << 5)
# define NE2000_RDCE	(1 << 6)

/*
 * receive status register flags.
 */

# define NE2000_SPRX	(1 << 0)

/*
 * register addresses.
 */

# define NE2000_CMD	0x0
# define NE2000_PSTART	0x1
# define NE2000_PAR	0x1
# define NE2000_PSTOP	0x2
# define NE2000_BOUND	0x3
# define NE2000_TPSR	0x4
# define NE2000_TBCR0	0x5
# define NE2000_TBCR1	0x6
# define NE2000_ISR	0x7
# define NE2000_CURR	0x7
# define NE2000_MAR	0x8
# define NE2000_RSAR0	0x8
# define NE2000_RSAR1	0x9
# define NE2000_RBCR0	0xA
# define NE2000_RBCR1	0xB
# define NE2000_RCR	0xC
# define NE2000_RSR	0xC
# define NE2000_TCR	0xD
# define NE2000_DCR	0xE
# define NE2000_IMR	0xF
# define NE2000_DATA	0x10
# define NE2000_RESET	0x1F

/*
 * data configuration flags.
 */

# define NE2000_8BITS	0
# define NE2000_16BITS	(1 << 0)

# define NE2000_LE	0
# define NE2000_BE	(1 << 1)

# define NE2000_LOOPBACK	0
# define NE2000_NORMAL		(1 << 3)

# define NE2000_FIFO1	0
# define NE2000_FIFO2	(1 << 5)
# define NE2000_FIFO4	(1 << 6)
# define NE2000_FIFO6	((1 << 5) | (1 << 6))

# define NE2000_NOCRC	(1 << 0)
# define NE2000_AUTOCRC	0

/*
 * receive flags.
 */

# define NE2000_REJECT_ON_ERROR	0
# define NE2000_RESUME_ON_ERROR	(1 << 0)

# define NE2000_REJECT_BCAST	0
# define NE2000_ACCEPT_BCAST	(1 << 2)

# define NE2000_REJECT_MCAST	0
# define NE2000_ACCEPT_MCAST	(1 << 3)

# define NE2000_PROMISCUOUS	(1 << 4)
# define NE2000_MONITOR		(1 << 5)

# define ETH_ALEN		6

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * packet queue
 */

SIMPLEQ_HEAD(__packetqueue, __packetqueue_entry);

struct __packetqueue_entry
{
  char*					packet;
  size_t				size;
  uint8_t				mac[ETH_ALEN];
  uint16_t				proto;
  SIMPLEQ_ENTRY(__packetqueue_entry)	entry;
};

/*
 * device
 */

typedef struct
{
  /* io base address */
  uint32_t			addr;
  /* is it a 8 bit or 16 bit controller */
  uint8_t			io_16;
  /* memory size in 256 bytes blocks */
  uint8_t			mem;
  /* transmit buffer offset in 256 bytes blocks */
  uint8_t			tx_buf;
  /* receive buffer offset in 256 bytes blocks */
  uint8_t			rx_buf;
  /* device physical address */
  uint8_t			mac[ETH_ALEN];
  /* irq thread id */
  pthread_t			irq_thread;
  /* spinlock */
  IA32_SPIN_FIELD_DECLARE(lock);
  /* current packet */
  struct
  {
    uint8_t			busy;
    uint16_t			size;
    uint8_t			tries;
  }				current;
  /* send queue */
  struct __packetqueue		sendqueue;
  /* bound interface */
  t_id				iface;
}		t_driver_ne2000_dev;

/*
 * ne2000 ring buffer header
 */

struct		ne2000_header_s
{
  uint8_t	status;
  uint8_t	next;
  uint16_t	size;
} __attribute__ ((packed));

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ne2000-pci.c
 */

/*
 * ne2000-pci.c
 */

t_error			ibmpc_irq_acknowledge(uint8_t		irq);

int	main(void);


/*
 * eop
 */

#endif
