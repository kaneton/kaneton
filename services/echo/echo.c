
#include <stdio.h>
#include <crt.h>
#include <libkaneton.h>

#include "echo-service.h"

int				echo_serve(void)
{
  t_service_echo_message*	message;
  i_node			sender;
  t_vsize			size;

  printf("server echo OK\n");
  if ((message = malloc(sizeof (*message))) == NULL)
    {
      printf(" -- echo: memory exhausted\n");
      return (-1);
    }

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_SERVICE_ECHO,
			  (t_vaddr)message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  switch (message->u.request.operation)
	    {
	      case ECHO_SERVICE_ECHO:
		printf(" -- echo: %s\n", message->u.request.u.echo.text);

		memmove(message->u.reply.u.echo.text,
			message->u.request.u.echo.text,
			strlen(message->u.request.u.echo.text) + 1);

		if (message_send(sender,
				 MESSAGE_TYPE_SERVICE_ECHO,
				 (t_vaddr)message,
				 sizeof (*message)) != ERROR_NONE)
		  {
		    printf(" -- echo: error in request\n");
		  }

		break;
	      default:
		printf(" -- echo: error, bad message id\n");
		break;
	    }
	}
    }

  return (0);
}

int main(void)
{
  if (message_register(MESSAGE_TYPE_SERVICE_ECHO,
		       MESSAGE_SIZE_SERVICE_ECHO) != ERROR_NONE)
    return (-1);

  printf(" -- echo: echo service started.\n");

  echo_serve();

  return (0);
}
