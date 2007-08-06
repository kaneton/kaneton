/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/pci/pci.c
 *
 * created       matthieu bucchianeri   [sat jun  9 17:26:20 2007]
 * updated       matthieu bucchianeri   [sat jun  9 22:05:37 2007]
 */

/*
 * Inspired by enum-pci driver from MutekH by Alexandre Becoulet
 *					<alexandre.becoulet@lip6.fr>
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#include "pci.h"
#include "pci-driver.h"

/* XXX elsewhere */

#define		OUTL(_port_, _data_)					\
  asm volatile("outl %%eax, %%dx\n"					\
	       :							\
	       : "d" (_port_), "a" (_data_))

#define		INL(_port_, _data_)					\
  asm volatile("inl %%dx, %%eax\n"					\
	       : "=a" (_data_)						\
	       : "d" (_port_))


/*
 * registered devices.
 */

static t_driver_pci_dev	devices[PCI_MAX_DEVICE];
static uint32_t		device_id = 0;

/*
 * read pci register.
 */

static uint32_t	pci_conf_read(uint32_t	bus,
			      uint32_t	dev,
			      uint32_t	fun,
			      uint32_t	reg)
{
  uint32_t	addr = 0x80000000 |
    (bus << 16) | (dev << 11) | (fun << 8) | (reg & 0xfc);
  uint32_t	val;

  OUTL(PCI_ADDRIO, addr);
  INL(PCI_DATAIO, val);

  return (val >> (8 * (reg & 3)));
}

/*
 * write pci register.
 */

static void	pci_conf_write(uint32_t	bus,
			       uint32_t	dev,
			       uint32_t	fun,
			       uint32_t	reg,
			       uint32_t	data)
{
  uint32_t	addr = 0x80000000 |
    (bus << 16) | (dev << 11) | (fun << 8) | reg;

  OUTL(PCI_ADDRIO, addr);
  OUTL(PCI_DATAIO, data);
}

/*
 * register a new device.
 */

static void	pci_register(uint16_t	vendor,
			     uint16_t	device,
			     uint32_t	class,
			     uint32_t*	io_base,
			     uint32_t*	mem_base,
			     uint32_t	irq)
{
  uint32_t	count;

  printf(" -- pci: new device detected: %x:%x\n", vendor, device);

  devices[device_id].vendor = vendor;
  devices[device_id].device = device;
  devices[device_id].class = class;
  for (count = 0; io_base[count] != 0; count++)
    ;
  memset(devices[device_id].io, 0, sizeof (devices[device_id].io));
  memcpy(devices[device_id].io, io_base, count);
  for (count = 0; mem_base[count] != 0; count++)
    ;
  memset(devices[device_id].mem, 0, sizeof (devices[device_id].mem));
  memcpy(devices[device_id].mem, mem_base, count);
  if (irq == 0xff)
    devices[device_id].irq = (t_driver_pci_irq)-1;
  else
    devices[device_id].irq = irq;
  device_id++;
}

/*
 * pci bus probing.
 *
 * steps:
 *
 * 1) loop through the PCI buses and devices.
 * 2) get presence, vendor, device and class.
 * 3) get io addresses and memory addresses.
 * 4) get irq line.
 * 5) register the device.
 */

void		pci_probe(void)
{
  uint32_t	bus, dev, fun;
  uint16_t	vendor;
  uint16_t	device;
  uint32_t	class;
  uint32_t	io_base[PCI_MAX_ADDRESS_COUNT];
  uint32_t	mem_base[PCI_MAX_ADDRESS_COUNT];
  uint32_t	io_index;
  uint32_t	mem_index;
  uint32_t	irq;
  uint32_t	i;
  uint32_t	reg;
  uint32_t	val;

  /*
   * 1)
   */

  for (bus = 0; bus < PCI_CONF_MAXBUS; bus++)
    for (dev = 0; dev < PCI_CONF_MAXDEVICE; dev++)
      for (fun = 0; fun < PCI_CONF_MAXFCN; fun++)
	{

	  /*
	   * 2)
	   */

	  vendor = pci_conf_read(bus, dev, fun, PCI_REG_VENDOR);
	  if (vendor == 0 || vendor == 0xffff)
	    break;

	  io_index = 0;
	  mem_index = 0;
	  irq = 0;

	  device = pci_conf_read(bus, dev, fun, PCI_REG_DEVID);
	  class = (pci_conf_read(bus, dev, fun, PCI_REG_CLASS) & 0x00ffff00) >> 8;

	  /*
	   * 3)
	   */

	  for (i = 0, reg = PCI_REG_ADDRESS_0;
	       i < PCI_MAX_ADDRESS_COUNT;
	       i++, reg += 4)
	    {
	      val = pci_conf_read(bus, dev, fun, reg);

	      pci_conf_write(bus, dev, fun, reg, 0xffffffff);
	      if (pci_conf_read(bus, dev, fun, reg) == 0)
		break;
	      pci_conf_write(bus, dev, fun, reg, val);

	      if (PCI_ADDRESS_IS_IO(val))
		{
		  io_base[io_index++] = PCI_ADDRESS_IO(val);
		}
	      else if (PCI_ADDRESS_IS_MEM32(val))
		{
		  mem_base[mem_index++] = PCI_ADDRESS_MEM32(val);
		}
	    }

	  /*
	   * 4)
	   */

	  irq = pci_conf_read(bus, dev, fun, PCI_REG_IRQLINE);

	  /*
	   * 5)
	   */

	  io_base[io_index] = 0;
	  mem_base[mem_index] = 0;
	  pci_register(vendor, device, class, io_base, mem_base, irq);
	}
}

/*
 * build list of devices by vendor:device ids.
 */

static t_driver_pci_message*	pci_enum_devices(t_driver_pci_vendor	vendor,
						 t_driver_pci_device	device,
						 uint32_t*		size)
{
  uint32_t		i;
  t_driver_pci_message*	reply;
  uint32_t		sz = 0;

  for (i = 0; i < PCI_MAX_DEVICE; i++)
    {
      if (devices[i].vendor == vendor &&
	  devices[i].device == device)
	{
	  sz += sizeof (t_driver_pci_dev);
	}
    }

  sz += sizeof (t_driver_pci_message) - 1;
  if ((reply = malloc(sz)) == NULL)
    return (NULL);
  reply->u.reply.u.get_device.count = 0;

  for (i = 0; i < PCI_MAX_DEVICE; i++)
    {
      if (devices[i].vendor == vendor &&
	  devices[i].device == device)
	{
	  memcpy(&reply->u.reply.u.get_device.devices[reply->u.reply.u.get_device.count],
		 &devices[i],
		 sizeof (t_driver_pci_dev));

	  reply->u.reply.u.get_device.count++;
	}
    }

  *size = sz;
  return (reply);
}

/*
 * request serving.
 */

void		pci_driver_serve(void)
{
  t_driver_pci_message	message;
  t_driver_pci_vendor	vendor;
  t_driver_pci_device	device;
  t_driver_pci_message*	reply;
  t_vsize		size;
  i_node		node;

  while (1)
    {
#if 0
      if (message_receive(0,
			  &message,
			  sizeof (message),
			  1) == ERROR_NONE)
	{
	  if (message.u.request.operation == PCI_DRIVER_GET_DEVICE)
	    {
	      vendor = message.u.request.u.get_device.vendor;
	      device = message.u.request.u.get_device.device;

	      reply = pci_enum_devices(vendor, device, &size);

	      if (reply == NULL || syscall_sync_send(&message.node,
						     0,
						     reply,
						     size) != ERROR_NONE)
		{
		  /* XXX */
		}

	      free(reply);
	    }
	}
#endif
    }
}

/*
 * driver entry point.
 */

int		main(void)
{
  printf(" -- pci: Tiny PCI driver started.\n");

  if (io_grant(PCI_ADDRIO, _crt_get_task_id(), 4) != ERROR_NONE ||
      io_grant(PCI_DATAIO, _crt_get_task_id(), 4) != ERROR_NONE)
    {
      printf(" -- pci: cannot acquire PCI i/o ports. abording.\n");

      return (-1);
    }

  pci_probe();

  pci_driver_serve();

  return (0);
}
