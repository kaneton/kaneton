
#include <stdio.h>
#include <crt.h>
#include <sys/io.h>
#include <libkaneton.h>

#include "ide-share.h"
#include "ide.h"

t_driver_ide			device;

static int			ide_check_disk(uint16_t	ctr,
					       uint16_t	dev)
{
  uint16_t			dx;
  uint8_t			al;
  uint32_t			ecx;
  int				i;

  dx = ctr;
  al = dev;
  outb(al, dx);
  dx = ctr + 1;
  al = 0xec;
  outb(al, dx);
  ecx = 5000;
  dx = ctr + 1;
  do {
    al = inb(dx);
    al = al & 0xd8;
    ecx--;
  } while ((0x58 != al) && (ecx > 0));
  if (ecx == 0)
    return (0);
  
  dx = ctr - 6;
  for (i = 0; i < 256; i++)
    device.buf[i] = 0;
  for (i = 0; i < 256; i++)
    device.buf[i] = inw(dx);
  //for (i = 0; i < 256; i++) 
  //  device.buf[i] = device.buf[i];//Print disk info
  
  return (1);
}

static int			ide_init(uint8_t	nb,
					 uint16_t	ctr)
{ 
  uint16_t			al, bl, dx;
  
  dx = ctr;
  al = inb(dx);
  bl = al;
  bl = bl & 0x10;
  bl = bl >> 4;
  if (bl == 0)
    //setslave
    al = al | 0x10;
  else
    //set master
    al = al & 0xef;
  outb(al, dx);
  
  al = inb(dx);
  al = al & 0x10;
  al = al >> 4;
  if (al == bl)
    {
      printf("|   -IDE %d @ 0x%x                  |\n", nb, ctr);
      return (ERROR_UNKNOWN);
    }
  printf("|   +IDE %d @ 0x%x                 |\n", nb, ctr);
  device.dev[nb * 2 + 0] = ide_check_disk(ctr, HD_M);
  if (device.dev[nb * 2 + 0])
    printf("|     --> Master disk detected     |\n");
  else
    printf("|     -->                          |\n");
  device.dev[nb * 2 + 1] = ide_check_disk(ctr, HD_S);  
  if (device.dev[nb * 2 + 1])
    printf("|     --> Slave disk detected      |\n");
  else
    printf("|     -->                          |\n");
  return (ERROR_NONE);
}

static int			ide_read(uint16_t	ctr,
					 uint8_t	dev,
					 uint32_t	start,
					 char		*buf)
{
    uint16_t			dx, al;
    uint32_t			i = 0;
    
    dx = ctr + 0x200;
    al = inb(dx);
    al = al | (1 << 2);
    outb(al, dx);
    for (i = 0; i < 100000; i++) //DON'T FORGE TTO CHECK
      ;				//IF INIT IS 
    al = al & ~(1 << 2);
    outb(al, dx);

    //size to read (sector)
    dx = ctr - 4;
    al = 1;
    outb(al, dx);
    
    //start point (sector first=0)
    dx = ctr - 3;
    al = 0;
    outb(al, dx);

    //Octet inférieur du numéro de cylindre du 1e secteur à transférer
    dx = ctr - 2;
    al = 0;
    outb(al, dx);

    //Octet supérieur du numéro de cylindre du 1e secteur à transférer
    dx = ctr - 1;
    al = 0;
    outb(al, dx);
    
    //Disque et numéro de tête du 1e secteur à transférer
    dx = ctr;
    al = dev + 0x40;
    outb(al, dx);

    //Commande
    dx = ctr + 1;
    al = 0x20;
    outb(al, dx);
    
    i = 5000;
    dx = ctr + 1;
    do {
      al = inb(dx);
      al = al & 0xd8;
      i--;
    } while ((0x58 != al) && (i > 0));
    if (i == 0)
      {
	printf("ide: 0x20 cmd error: Timeout\n");
	return (ERROR_UNKNOWN);
      }    

    //transfer
    dx = ctr - 6;
    short	*b;
    b = (short *)buf;
    for (i = 0; i < 256; i++)
	b[i] = inw(dx);
	
    return (ERROR_NONE);
}


static int			ide_serve()
{
  t_driver_ide_message		message;
  i_node			sender;
  t_vsize			size;
  uint16_t			ctr;
  uint8_t			dev;
  
  while (1)
    {
      if (message_receive(MESSAGE_TYPE_DRIVER_IDE,
			  (t_vaddr)&message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  if (message.u.req_read.ctr == 0)
	    ctr = PHD_1;
	  if (message.u.req_read.ctr == 1)
	    ctr = PHD_2;
	  if (message.u.req_read.ctr == 2)
	    ctr = PHD_3;
	  if (message.u.req_read.ctr == 3)
	    ctr = PHD_4;
	  if (message.u.req_read.dev == 0)
	    dev = HD_M;
	  if (message.u.req_read.dev == 1)
	    dev = HD_S;
	  switch (message.operation)
	    {
	    case IDE_DRIVER_REQ_READ:
	      if ((message.u.req_read.ctr > 3) || (message.u.req_read.dev > 1) || 
		  (!device.dev[message.u.req_read.ctr * 2 + message.u.req_read.dev]))
		{
		  message.operation = IDE_DRIVER_REP_ERR;
		  memmove(message.u.rep_err.text,
			  "error, bad disk",
			  17);
		  break;
		}
	      message.operation = IDE_DRIVER_REP_READ;
	      if (ide_read(ctr, dev, message.u.req_read.start,
			   message.u.rep_read.buf) != ERROR_NONE)
		{
		  message.operation = IDE_DRIVER_REP_ERR;
		  memmove(message.u.rep_err.text,
			  "error, reading fail\n",
			  21);
		  break;
		}
	      break;
	    default:
	      message.operation = IDE_DRIVER_REP_ERR;
	      memmove(message.u.rep_err.text,
		      "error, bad message",
		      20);		
	      break;
	    }
	  if (message_send(sender,
			   MESSAGE_TYPE_DRIVER_IDE,
			   (t_vaddr)&message,
			   sizeof (message)) != ERROR_NONE)
	    printf(" -- ide: error when sending ack\n");
	}
    } 
  return (0);
}

int		main(void)
{
  if (message_register(MESSAGE_TYPE_DRIVER_IDE,
		       MESSAGE_SIZE_DRIVER_IDE) != ERROR_NONE)
    return (-1);
  
  
  printf("\n====================================\n");
  printf("|             IDE Init             |\n");
  printf("====================================\n");
  memset(&device, sizeof(device), 0);
  ide_init(0, PHD_1); 
  ide_init(1, PHD_2); 
  ide_init(2, PHD_3); 
  ide_init(3, PHD_4); 
  printf("====================================\n\n");
  

  ide_serve();
  
  return (0);
}
