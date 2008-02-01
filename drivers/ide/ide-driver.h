
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

#include "ide.h"

/*
 * ---------- macros ----------------------------------------------------------
 */


/*
 * ---------- interface -------------------------------------------------------
 */


t_error
ide_node_init(i_task	*ide);
t_error
ide_write(const char* text, char* buf, size_t max);


t_error
ide_node_init(i_task	*ide)
{
  static int	is_init = 0;
  
  if (!is_init)
    {
      is_init = 1;
      if (message_register(MESSAGE_TYPE_SERVICE_IDE,
			   MESSAGE_SIZE_SERVICE_IDE) != ERROR_NONE)
	return (ERROR_UNKNOWN);
      if (mod_get_service(_crt_get_mod_id(), SERVICE_IDE_NAME, ide) != ERROR_NONE)
	{
	  printf(" -- test_ide: error mog_get_service()\n");
	  return (ERROR_UNKNOWN);
	}
    }
  return (ERROR_NONE);
}

t_error
ide_write(const char* text, char* buf, size_t max)
{
  t_service_ide_message		message;
  i_node			ide_service;
  t_vsize			size;
  i_task			ide;
  
  if (ide_node_init(&ide) != ERROR_NONE)
    {
      printf(" -- test_ide: error ide_init()\n");
      return (ERROR_UNKNOWN);
    }
  ide_service.machine = 0;
  ide_service.task = ide;

  message.u.request.operation = IDE_SERVICE_IDE;
  strcpy(message.u.request.u.ide.text, text);

  if (message_send(ide_service,
		   MESSAGE_TYPE_SERVICE_IDE, (t_vaddr)&message,
		   sizeof (message)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (message_receive(MESSAGE_TYPE_SERVICE_IDE, (t_vaddr)&message,
		      &size, &ide_service) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  strncpy(buf, message.u.reply.u.ide.text, max);

  return (ERROR_NONE);
}

#endif
