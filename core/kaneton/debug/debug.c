/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * debug.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:23:13 2005   mycure
 * last update   Fri Oct 21 19:56:27 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just initialises the debug manager.
 */

t_error			debug_init(void)
{
  /*
   * XXX
   */
  serial_init(SERIAL_COM1, SERIAL_BR57600, SERIAL_8N1, SERIAL_FIFO_8);
  printf("serial port initialized\n");	
  printf_init(serial_put, 0); /* pb du flush !  */
  
  while(1) 	debug_recv();
  
  return (ERROR_NONE);
}

/*
 * this function cleans the debug manager.
 */

t_error			debug_recv()
{
  t_serial_data		recv_type;
  int i;

  serial_recv(SERIAL_COM1, &recv_type);
  
  if (!strcmp(recv_type.data, "command"))
	  debug_recv_cmd();
  else	  
	printf("debug receive unknown type\n");
}

t_error			debug_recv_cmd()
{
  t_serial_data 	cmd_tab_len;
  t_serial_data		*cmd_tab;
  t_serial_data		*cmd_tab_tmp;
  int 			len, i;

  serial_recv(SERIAL_COM1, &cmd_tab_len);	
  len = strtol(cmd_tab_len.data, 0, 10);
  cmd_tab = malloc((len + 1) * sizeof(t_serial_data));
 
  for (i = 0; i < len; i++)
   serial_recv(SERIAL_COM1,(t_serial_data *) (cmd_tab + i));
    
  debug_exec_cmd_tab(cmd_tab);
  serial_send(SERIAL_COM1, "endprintf", 9);

  
  for (i = 0; (cmd_tab + i)->data; i++)
	  free ((cmd_tab + i)->data);
  free(cmd_tab);
}


t_error			debug_exec_cmd_tab(t_serial_data *cmd_tab)
{ 
  int i , ret;
  static debug_function_t	function_list[] = 
	{
		{"printf", printf},/*debug printf*/
		{"alloc_test", alloc_test},
		{"set_test", set_test},
		{0, 0}
	};

   for (i = 0; function_list[i].f_name; i++)
	if (!strcmp(function_list[i].f_name, (char *) (cmd_tab + 0)->data))	
   	  return (function_list[i].f((char *)(cmd_tab + 1)->data));

  printf("Bad function called");
  /*   printf("\n"); Mystique printf*/
}


t_error			debug_clean(void)
{
  /*
   * XXX
   */

  return (ERROR_NONE);
}
