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
  uint32_t			ecx, i;

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
  uint16_t			dx;
  uint8_t			al, bl;
  
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
  uint16_t			dx;
  uint8_t			al;
  uint32_t			i = 0;
  
  /* dx = ctr + 0x200; */
  /*   al = inb(dx); */
  /*   al = al | (1 << 2); */
  /*   outb(al, dx); */
  /*   for (i = 0; i < 100000; i++) //Init is not necesary */
  /*     ; */
  /*   al = al & ~(1 << 2); */
  /*   outb(al, dx); */
  
  //Port definition for CHS    //// for LBA
  
  //size to read (sector)
  dx = ctr - 4;
  al = 1;
  outb(al, dx);    
  //start point (sector first=0)/// bit 0 to 7 of LBA adresse
  dx = ctr - 3;
  al = 0xFF & start;
  outb(al, dx);
  //lower bytes of start sector /// bit 8 to 15 of LBA adresse
  dx = ctr - 2;
  al = 0xFF & (start >> 8);
  outb(al, dx);
  //highter bytes of start sector// bit 16 to 23 of LBA adresse
  dx = ctr - 1;
  al = 0xFF & (start >> 16);
  outb(al, dx);
  //bit 4 (0=master,1=slave) /// bit 5 and 7 = 1 /// bit 6 (0=CHS,1=LBA)  /// bit 0 to 3 are bit 24 to 27 of LBA adresse
  dx = ctr;
  al = (dev + 0x40) | (0xF & (start >> 24));
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


static int			ide_write(uint16_t	ctr,
					  uint8_t	dev,
					  uint32_t	start,
					  char		*buf)
{
  uint16_t			dx;
  uint8_t			al;
  uint32_t			i = 0;
  
  dx = ctr - 4;
  al = 1;
  outb(al, dx);    
  //start point (sector first=0)/// bit 0 to 7 of LBA adresse
  dx = ctr - 3;
  al = 0xFF & start;
  outb(al, dx);
  //lower bytes of start sector /// bit 8 to 15 of LBA adresse
  dx = ctr - 2;
  al = 0xFF & (start >> 8);
  outb(al, dx);
  //highter bytes of start sector// bit 16 to 23 of LBA adresse
  dx = ctr - 1;
  al = 0xFF & (start >> 16);
  outb(al, dx);
  //bit 4 (0=master,1=slave) /// bit 5 and 7 = 1 /// bit 6 (0=CHS,1=LBA)  /// bit 0 to 3 are bit 24 to 27 of LBA adresse
  dx = ctr;
  al = (dev + 0x40) | (0xF & (start >> 24));
  outb(al, dx);
  //Commande
  dx = ctr + 1;
  al = 0x30;
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
      printf("ide: 0x30 cmd error: Timeout\n");
      return (ERROR_UNKNOWN);
    }    
  
  //transfer
  dx = ctr - 6;
  short	*b;
  b = (short *)buf;
  for (i = 0; i < 256; i++)
    outw(b[i], dx);
  
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
	    case IDE_DRIVER_REQ_WRITE:
	      if ((message.u.req_write.ctr > 3) || (message.u.req_write.dev > 1) || 
		  (!device.dev[message.u.req_write.ctr * 2 + message.u.req_write.dev]))
		{
		  message.operation = IDE_DRIVER_REP_ERR;
		  memmove(message.u.rep_err.text,
			  "error, bad disk",
			  17);
		  break;
		}
	      message.operation = IDE_DRIVER_REP_WRITE;
	      if (ide_write(ctr, dev, message.u.req_write.start,
			    message.u.req_write.buf) != ERROR_NONE)
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
