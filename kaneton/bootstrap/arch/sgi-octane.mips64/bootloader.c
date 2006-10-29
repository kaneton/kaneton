/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootstrap/arch/sgi-octane.mips64/bootloader.c
 *
 * created       enguerrand raymond   [sun oct 22 22:59:28 2006]
 * updated       enguerrand raymond   [sun oct 29 14:20:16 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

extern unsigned long bootstrap_end;

int	(*bootloader_func)(void);

/*
 * ---------- functions -------------------------------------------------------
 */

int	bootloader_cons_print_char(char	c)
{
  return (write(c));
}

void	bootloader_cons_attr(u_int8_t	attr)
{
}

int	bootloader_cons_init(void)
{
  printf_init(bootloader_cons_print_char, bootloader_cons_attr);
  return (0);
}

void		bootloader(void)
{
  /*  char*		seek;
  char		c = 'e';
  char*		adr;
  int		count = 0;*/

  unsigned long fd;
  char buffer[50];
  unsigned long count = 0;

  //  seek = &c;

  bootloader_cons_init();

  printf("Loading bootloader\n\n");

  /*  while(1)
    {
      if (*seek == 0x7f)
	{
	  printf("seek1 = %x\n", *seek);
	  adr = seek;
	  seek += 8;
	  if (*seek == 0x45)
	    {
	      seek += 4;
	      if(*seek == 0x4c)
		{
		  seek += 4;
		  if(*seek == 0x46)
		    {
		      seek += 4;
		      if(*seek == 0x02)
			{
			  ++count;
			  break;
			  if (count == 2)
			    break;
			}
		    }
		}
	    }
	}
      ++seek;
      }

  memcpy(0xa80000000003f000, 0xa80000000000b66a, 34042);

  printf("Bootloader copied!\n");

  bootloader_func = 0xa800000000040008;
  */

  //bootloader_func();

  /*  printf("open = %d\n", ArcOpen("pci(0)scsi(0)disk(2)rdisk(0)partition(0)bootloader",//xio(0)pci(15)scsi(0)disk(0)rdisk(0)partition(2)bootloader",
      OpenReadOnly, &fd));*/
  printf("open = %d\n", ArcOpen("/unix/bootloader",//pci(0)scsi(0)disk(2)rdisk(0)partition(1)bootloader",
				OpenReadOnly, &fd));

  ArcRead(fd, &buffer, 40, &count);

  printf("SystemPartition = %s\n", ArcGetEnvironmentVariable("SystemPartition"));
  printf("OSLoader = %s\n", ArcGetEnvironmentVariable("OSLoader"));
  printf("OSLoadPartition = %s\n", ArcGetEnvironmentVariable("OSLoadPartition"));
  printf("OSLoadFilename = %s\n", ArcGetEnvironmentVariable("OSLoadFilename"));
  printf("LoadIdentifier = %s\n", ArcGetEnvironmentVariable("LoadIdentifier"));
  printf("OSLoadOptions = %s\n", ArcGetEnvironmentVariable("OSLoadOptions"));
  printf("AutoLoad = %s\n", ArcGetEnvironmentVariable("AutoLoad"));
  printf("FWSearchPath = %s\n", ArcGetEnvironmentVariable("FWSearchPath"));

  //printf("buffer = %x count = %d\n", buffer[1], count);

  while(1);
}
