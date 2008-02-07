
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
ide_node_init(i_task	*ide);
t_error
ide_read(uint8_t ctr, uint8_t dev, char* buf, size_t size);


t_error
ide_node_init(i_task	*ide)
{
  static int	is_init = 0;
  
  if (!is_init)
    {
      is_init = 1;
      if (message_register(MESSAGE_TYPE_DRIVER_IDE,
			   MESSAGE_SIZE_DRIVER_IDE) != ERROR_NONE)
	return (ERROR_UNKNOWN);
      if (mod_get_service(_crt_get_mod_id(), DRIVER_IDE_NAME, ide) != ERROR_NONE)
	{
	  printf(" -- test_ide: error mog_get_service()\n");
	  return (ERROR_UNKNOWN);
	}
    }
  return (ERROR_NONE);
}

t_error
ide_read(uint8_t ctr, uint8_t dev, char* buf, size_t size)
{
  t_driver_ide_message		message;
  i_node			ide_driver;
  i_task			ide;
  
  if (ide_node_init(&ide) != ERROR_NONE)
    {
      printf(" -- test_ide: error ide_init()\n");
      return (ERROR_UNKNOWN);
    }
  ide_driver.machine = 0;
  ide_driver.task = ide;

  message.u.req_read.ctr = ctr;
  message.u.req_read.dev = dev;
  message.u.req_read.size = size;
  message.operation = IDE_DRIVER_REQ_READ;
  if (message_send(ide_driver,
		   MESSAGE_TYPE_DRIVER_IDE, (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  if (message_receive(MESSAGE_TYPE_DRIVER_IDE, (t_vaddr)&message,
		      &size, &ide_driver) != ERROR_NONE)
    return (ERROR_UNKNOWN);
  switch (message.operation)
    {
    case IDE_DRIVER_REP_ERR:
      printf("ide driver send to you an error: '%s'\n", message.u.rep_err.text);
      return (ERROR_UNKNOWN);
      break;
    case IDE_DRIVER_REP_READ:
      strncpy(buf, message.u.rep_read.buf, size);
      break;
    default:
      printf("error, bad message");
      return (ERROR_UNKNOWN);
      break;
    }
  return (ERROR_NONE);
}

#endif
