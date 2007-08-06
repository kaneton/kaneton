/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/pci/pci-driver.h
 *
 * created       matthieu bucchianeri   [sat jun  9 18:17:15 2007]
 * updated       matthieu bucchianeri   [sat jun  9 23:32:36 2007]
 */

#ifndef DRIVERS_PCI_DRIVER_H
# define DRIVERS_PCI_DRIVER_H

/*
 * ---------- information -----------------------------------------------------
 *
 * pci driver interface.
 *
 * the pci driver replies to the following requests:
 *
 * - PCI_DRIVER_GET_DEVICE(t_driver_pci_vendor, t_driver_pci_device)
 *   retrieve the list of the pci devices matching to the vendor and
 *   device id.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

# include <libc.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * some constants.
 */

# define PCI_MAX_DEVICE		256
# define PCI_MAX_ADDRESS_COUNT	6

/*
 * driver messages.
 */

# define PCI_DRIVER_GET_DEVICE	1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * basic types.
 */

typedef uint16_t	t_driver_pci_vendor;
typedef uint16_t	t_driver_pci_device;
typedef uint32_t	t_driver_pci_class;
typedef uint32_t	t_driver_pci_io;
typedef uint32_t	t_driver_pci_irq;

/*
 * device struct.
 */

typedef struct
{
  /*
   * vendor identifier of the device.
   */

  t_driver_pci_vendor	vendor;

  /*
   * device identifier.
   */

  t_driver_pci_device	device;

  /*
   * pci device class.
   */

  t_driver_pci_class	class;

  /*
   * device io bases.
   */

  t_driver_pci_io	io[PCI_MAX_ADDRESS_COUNT];

  /*
   * device mem bases.
   */

  paddr_t		mem[PCI_MAX_ADDRESS_COUNT];

  /*
   * device irq line.
   */

  t_driver_pci_irq	irq;

}			t_driver_pci_dev;

/*
 * device identifier.
 */

typedef struct
{
  /*
   * vendor identifier of the device.
   */

  t_driver_pci_vendor	vendor;

  /*
   * device identifier.
   */

  t_driver_pci_device	device;

  /*
   * string device name.
   */

  const char*		string;
}			t_driver_pci_ids;

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
	  t_driver_pci_vendor		vendor;
	  t_driver_pci_device		device;
	}				get_device;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  uint32_t			count;
	  uint8_t			devices[1];
	}				get_device;
      }			u;
    }			reply;
  } u;
}			t_driver_pci_message;

#endif
