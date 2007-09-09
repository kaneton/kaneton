/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/pci/pci.h
 *
 * created       matthieu bucchianeri   [sat jun  9 17:24:23 2007]
 * updated       matthieu bucchianeri   [sat jun  9 22:38:19 2007]
 */

#ifndef DRIVERS_PCI_H
# define DRIVERS_PCI_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

# include <stdint.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * PCI I/O base.
 */

# define PCI_ADDRIO		0xcf8
# define PCI_DATAIO		0xcfc

/*
 * PCI registers ids.
 */

# define PCI_REG_VENDOR		0x00
# define PCI_REG_DEVID		0x02
# define PCI_REG_CMD		0x04
# define PCI_REG_STATUS		0x06
# define PCI_REG_REVID		0x08
# define PCI_REG_CLASS		0x09

# define PCI_REG_ADDRESS_0	0x10
# define PCI_REG_ADDRESS_1	0x14
# define PCI_REG_ADDRESS_2	0x18
# define PCI_REG_ADDRESS_3	0x1c
# define PCI_REG_ADDRESS_4	0x20
# define PCI_REG_ADDRESS_5	0x24
# define PCI_REG_IRQLINE	0x3c

/*
 * PCI max values.
 */

# define PCI_CONF_MAXBUS	8
# define PCI_CONF_MAXDEVICE	32
# define PCI_CONF_MAXFCN	8
# define PCI_CONF_MAXREG	256

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * check value type.
 */

# define	PCI_ADDRESS_IS_IO(x)		((x) & 1)
# define	PCI_ADDRESS_IS_MEM32(x)		(((x) & 0x5) == 0x0)

/*
 * extract value.
 */

# define	PCI_ADDRESS_IO(x)		((x) & ~0x03)
# define	PCI_ADDRESS_MEM32(x)		((x) & ~0x0f)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      pci.c
 */

/*
 * pci.c
 */

void		pci_probe(void);

void		pci_driver_serve(void);

int		main(void);


/*
 * eop
 */

#endif
