/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-o2.mips64/init.c
 *
 * created       enguerrand raymond   [sun nov 19 16:19:55 2006]
 * updated       enguerrand raymond   [tue dec 19 16:02:04 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <libmipsr10000.h>
#include <kaneton.h>

#include "bootloader.h"
#include "init.h"

/*
 * ---------- globals -------------------------------------------------------
 */

/*
 * init variable, describing the initial memory layout.
 *
 * this structure will be passed to the kernel describing modules,
 * physical memory reserved by the hardware etc.. thus the kernel will
 * be able to initialise its managers: segment manager, module manager etc..
 */

t_init*		init;

t_uint32	addr_alloc = KERNEL_BASE + 0x64000;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * Function to allocate memory by page of PAGESIZE
 */

t_uint32	alloc(t_uint64	size)
{
  t_uint32	res = addr_alloc;

  addr_alloc += nbr_page(size) * 4096;
  return res;
}


/*
 * Function to perform memory size using firmware
 */

t_uint32		bootloader_memory_size(void)
{
  MEMORYDESCRIPTOR*	mem_desc = NULL;
  t_uint32		page_count = 0;

  while ((mem_desc = ArcGetMemoryDescriptor(mem_desc)) != NULL)
    page_count += mem_desc->PageCount;
  return (page_count * 4) / 1024;
}


/*
 * Copying kernel from tftp server to KERNEL_BASE address.
 * The function returns kernel size
 */

t_uint32		bootloader_kernel_copy(void)
{
  t_uint32		file_id;
  char			buffer[100];
  t_uint32		count = 0;

  //ArcMount("pci(0)scsi(0)cdrom(0)fdisk(0)", LoadMedia);
  ArcOpen("bootp()kernel", OpenReadOnly, &file_id);
  //ArcOpen("rdisk(1)partition(1)bootstrap1", OpenReadOnly, &file_id);
  buffer[99] = 0;
  ArcRead(file_id, buffer, 50, &count);
  printf("buffer = %s\n", buffer);
  return 0;
}
