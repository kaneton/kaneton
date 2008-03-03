
#ifndef IDE_DRIVER_H_
#define IDE_DRIVER_H_

/*
 * ---------- dependencies ----------------------------------------------------
 */

#define MOD_SPAWN_INTERFACE
#include "../../services/mod/mod-service.h"
#include <libkaneton.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>

#include "ide-share.h"

/*
 * ---------- macros ----------------------------------------------------------
 */


/*
 * ---------- interface -------------------------------------------------------
 */


t_error
ide_node_init(void);

/*
* This function send to ide driver a request to read size sectors
* It's begin from start sector to start + size
* It read on controlor ctr (0-3) on device dev (0=master, 1=slave)
* It read sectors and put it in buf
*/
t_error
ide_read(uint8_t	ctr,
	 uint8_t	dev,
	 uint32_t	start,
	 size_t		size,
	 char		*buf);


/*
* This function send to ide driver a request to write size sectors
* It's begin from start sector to start + size
* It writes on driver ctr (0-3) on device dev (0=master, 1=slave)
* It writes sectors from buf
*/
t_error
ide_write(uint8_t	ctr,
	  uint8_t	dev,
	  uint32_t	start,
	  size_t	size,
	  char		*buf);

i_task			ide;

t_error
ide_node_init(void)
{
  static uint8_t	is_init = 0;
  
  if (!is_init)
    {
      is_init = 1;
      if (message_register(MESSAGE_TYPE_DRIVER_IDE,
			   MESSAGE_SIZE_DRIVER_IDE) != ERROR_NONE)
	{
	  printf(" -- ide_init: error message_register\n");
	  return (ERROR_UNKNOWN);
	}
      if (mod_get_service(_crt_get_mod_id(), DRIVER_IDE_NAME, &ide) != ERROR_NONE)
	{
	  printf(" -- ide_init: error mog_get_service()\n");
	  return (ERROR_UNKNOWN);
	}
    }
  
  return (ERROR_NONE);
}

t_error	ide_read(uint8_t	ctr,
		 uint8_t	dev,
		 uint32_t	start,
		 size_t		size, //in sector (512bytes)
		 char		*buf)
{
  t_driver_ide_message		message;
  i_node			ide_driver;
  size_t			i = 0;
  size_t			nothing;
  
  if (ide_node_init() != ERROR_NONE)
    {
      printf(" -- test_ide: error ide_init()\n");
      return (ERROR_UNKNOWN);
    }
  ide_driver.machine = 0;
  ide_driver.task = ide;
  for (i = 0; size != 0; ++i, --size)
    {
      message.operation = IDE_DRIVER_REQ_READ;
      message.u.req_read.ctr = ctr;
      message.u.req_read.dev = dev;  
      message.u.req_read.start = start + i;      
      if (message_send(ide_driver,
		       MESSAGE_TYPE_DRIVER_IDE, (t_vaddr)&message,
		       sizeof (message)) != ERROR_NONE)
	{
	  printf(" -- ide: error when sending reading request\n");
	  return (ERROR_UNKNOWN);
	}
      if (message_receive(MESSAGE_TYPE_DRIVER_IDE, (t_vaddr)&message,
			  &nothing, &ide_driver) != ERROR_NONE)
	{
	  printf(" -- ide: error when recinving reading reply\n");
	  return (ERROR_UNKNOWN);
	}
      switch (message.operation)
	{
	case IDE_DRIVER_REP_ERR:
	  printf("ide driver send to you an error: '%s'\n", message.u.rep_err.text);
	  return (ERROR_UNKNOWN);
	  break;
	case IDE_DRIVER_REP_READ:
	  strncpy(buf + IDE_DRIVER_MAX * i, message.u.rep_read.buf, IDE_DRIVER_MAX);
	  break;
	default:
	  printf("error, bad message");
	  return (ERROR_UNKNOWN);
	  break;
	}
    }
  return (ERROR_NONE);
}

t_error	ide_write(uint8_t	ctr,
		  uint8_t	dev,
		  uint32_t	start,
		  size_t	size, //in sector (512bytes)
		  char		*buf)
{
  t_driver_ide_message		message;
  i_node			ide_driver;
  size_t			i = 0;
  size_t			nothing;
  
  if (ide_node_init() != ERROR_NONE)
    {
      printf(" -- ide: error ide_init()\n");
      return (ERROR_UNKNOWN);
    }
  ide_driver.machine = 0;
  ide_driver.task = ide;
  for (i = 0; size != 0; ++i, --size)
    {
      message.operation = IDE_DRIVER_REQ_WRITE;
      message.u.req_write.ctr = ctr;
      message.u.req_write.dev = dev;  
      message.u.req_write.start = start + i;   
      memmove(message.u.req_write.buf, buf + IDE_DRIVER_MAX * i, IDE_DRIVER_MAX);
      if (message_send(ide_driver,
		       MESSAGE_TYPE_DRIVER_IDE, (t_vaddr)&message,
		       sizeof (message)) != ERROR_NONE)
	{
	  printf(" -- ide: error when sending writting request\n");
	  return (ERROR_UNKNOWN);
	}
      if (message_receive(MESSAGE_TYPE_DRIVER_IDE, (t_vaddr)&message,
			  &nothing, &ide_driver) != ERROR_NONE)
	{
	  printf(" -- ide: error when recinving writting reply\n");
	  return (ERROR_UNKNOWN);
	}
      switch (message.operation)
	{
	case IDE_DRIVER_REP_ERR:
	  printf(" -- ide: driver send to you an error: '%s'\n", message.u.rep_err.text);
	  return (ERROR_UNKNOWN);
	  break;
	case IDE_DRIVER_REP_WRITE:
	  break;
	default:
	  printf(" -- ide: error, bad message");
	  return (ERROR_UNKNOWN);
	  break;
	}
    }
  return (ERROR_NONE);
}

#endif
