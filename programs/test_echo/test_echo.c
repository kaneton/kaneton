
#include <stdio.h>
#include <crt.h>
#include <libkaneton.h>

#define MOD_SPAWN_INTERFACE
#include "../../services/mod/mod-service.h"
#define ECHO_SPAWN_INTERFACE
#include "../../services/echo/echo-service.h"

int		main(void)
{
  i_task	echo;
  char		buf[ECHO_SERVICE_MAX];

  if (echo_init() != ERROR_NONE)
    {
      printf(" -- test_echo: error echo_init()\n");
      return (-1);
    }

  if (mod_get_service(_crt_get_mod_id(), "echo", &echo) != ERROR_NONE)
    {
      printf(" -- test_echo: error mog_get_service()\n");
      return (-1);
    }

  if (!echo_echo(echo, "chiche donne nous tout", buf,
		 ECHO_SERVICE_MAX) != ERROR_NONE)
    printf(" -- test_echo: echo ok\n");

  printf(" -- test_echo: %s\n", buf);

  while (1)
    ;

  return (0);
}
