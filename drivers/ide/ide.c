
#include <stdio.h>
#include <crt.h>
#include <libkaneton.h>

#include "ide.h"

static int			ide_init(void)
{
  
  printf("init\n");

  return (0);
}

static int			ide_serve()
{
  t_service_ide_message		message;
  i_node			sender;
  t_vsize			size;

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_SERVICE_IDE,
			  (t_vaddr)&message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  switch (message.u.request.operation)
	    {
	    case IDE_SERVICE_IDE:
	      printf(" -- ide: %s\n", message.u.request.u.ide.text);
	      ide_init();
	      break;
	    default:
	      printf(" -- ide: error, bad message id\n");
	      break;
	    }
	  memmove(message.u.reply.u.ide.text,
		  "Bien recu\n",
		  11);
	  if (message_send(sender,
			   MESSAGE_TYPE_SERVICE_IDE,
			   (t_vaddr)&message,
			   sizeof (message)) != ERROR_NONE)
	    printf(" -- ide: error in request\n");
	}
    } 
  return (0);
}

int main(void)
{
  if (message_register(MESSAGE_TYPE_SERVICE_IDE,
		       MESSAGE_SIZE_SERVICE_IDE) != ERROR_NONE)
    return (-1);

  printf(" -- ide: ide service started.\n");

  ide_serve();

  return (0);
}
