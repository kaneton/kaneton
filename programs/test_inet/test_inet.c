/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/programs/test_inet/test_inet.c
 *
 * created       matthieu bucchianeri   [sat aug 25 18:07:46 2007]
 * updated       matthieu bucchianeri   [tue sep 11 15:18:12 2007]
 */

#include <stdio.h>
#include <stdint.h>
#include <crt.h>
#include <libkaneton.h>

#define MOD_SPAWN_INTERFACE
#include "../../services/mod/mod-service.h"
#define INET_SPAWN_INTERFACE
#include "../../services/inet/inet-service.h"

int		main()
{
  t_service_inet_addr	addr;
  t_service_inet_addr	mask;
  t_service_inet_addr	target;
  i_task		inet;

  inet_init();

  printf(" -- test inet --\n");

  if (mod_get_service(_crt_get_mod_id(), SERVICE_INET_NAME, &inet) !=
      ERROR_NONE)
    {
      printf(" -- cannot find inet service\n");
      while (1)
	;
    }

  while (inet_if_up(inet, "eth0") != ERROR_NONE)
    scheduler_yield(0);

  addr.type = e_service_inet_ipv4;
  addr.addr.ipv4 = 0xC0A80201;
  mask.type = e_service_inet_ipv4;
  mask.addr.ipv4 = 0xFFFFFF00;
  inet_if_config(inet, "eth0", INET_IF_CONFIG_SET, &addr, &mask);

  target.type = e_service_inet_ipv4;
  target.addr.ipv4 = 0xC0A80200;

  inet_route_add(inet, "eth0", &target, &mask, NULL);

  inet_if_dump(inet, "eth0");

  inet_route_dump(inet);

  task_state(_crt_get_task_id(), SCHEDULER_STATE_STOP);

  return 0;
}
