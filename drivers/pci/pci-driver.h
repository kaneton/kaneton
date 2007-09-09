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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * driver name
 */

#define DRIVER_PCI_NAME		"pci"

/*
 * some constants.
 */

#define PCI_MAX_DEVICE		256
#define PCI_MAX_ADDRESS_COUNT	6

/*
 * driver messages description.
 */

#define MESSAGE_TYPE_DRIVER_PCI	(MESSAGE_TYPE_CRT + 3)
#define MESSAGE_SIZE_DRIVER_PCI	(sizeof (t_driver_pci_message))

/*
 * driver operations
 */

#define PCI_DRIVER_GET_DEVICE	1

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

  uint32_t		mem[PCI_MAX_ADDRESS_COUNT];

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
	  t_error			status;
	  t_uint32			has_more;
	  t_driver_pci_dev		device;
	}				get_device;
      }			u;
    }			reply;
  } u;
}			t_driver_pci_message;

/*
 * ---------- interface -------------------------------------------------------
 */

#ifdef PCI_SPAWN_INTERFACE

#define PCI_ATTRIBUTE_DEFAULT		static inline

#ifndef PCI_ATTRIBUTE_INTERFACE
# define PCI_ATTRIBUTE_INTERFACE	PCI_ATTRIBUTE_DEFAULT
#endif

typedef void (*t_driver_pci_new_device)(t_driver_pci_dev*, void*);

PCI_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
pci_init(void)
{
  return message_register(MESSAGE_TYPE_DRIVER_PCI,
			  MESSAGE_SIZE_DRIVER_PCI);
}

PCI_ATTRIBUTE_INTERFACE __attribute__((unused)) t_error
pci_get_device(i_task pci, t_driver_pci_vendor vendor,
	       t_driver_pci_device device, t_driver_pci_new_device callback,
	       void* pv)
{
  t_driver_pci_message	message;
  i_node		pci_driver;
  t_vsize		size;

  pci_driver.machine = 0;
  pci_driver.task = pci;

  message.u.request.operation = PCI_DRIVER_GET_DEVICE;
  message.u.request.u.get_device.vendor = vendor;
  message.u.request.u.get_device.device = device;

  if (message_send(pci_driver,
		   MESSAGE_TYPE_DRIVER_PCI,
		   (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    {
      return (ERROR_UNKNOWN);
    }

  do
    {
      if (message_receive(MESSAGE_TYPE_DRIVER_PCI,
			  (t_vaddr)&message,
			  &size,
			  &pci_driver) != ERROR_NONE)
	{
	  return (ERROR_UNKNOWN);
	}

      if (!message.u.reply.u.get_device.has_more)
	break;

      callback(&message.u.reply.u.get_device.device, pv);

    } while (1);

  return (ERROR_NONE);
}

#endif

#endif
