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
 * ---------- globals ---------------------------------------------------------
 */

t_setid			buffers;

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
  printf_init(serial_put, 0); 
  set_reserve(ll, SET_OPT_ALLOC, sizeof(t_serial_buffer), &buffers);
  
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
  else if (!strcmp(recv_type.data, "loadfile"))
  	load_data();  
  else	  
	printf("debug receive unknown type\n");
  free(recv_type.data);
}


t_error			load_data()
{
  t_serial_data		cmd;
  t_serial_buffer	buffer;
  
  serial_recv(SERIAL_COM1, (t_serial_data  *) &cmd);
  buffer.name = cmd.data;
  free(cmd.data);
  serial_recv(SERIAL_COM1, (t_serial_data  *) &cmd);
  buffer.data = cmd.data;
  set_add(buffers, &buffer);
  free(cmd.data);
}

char*		get_data(char *name)
{
	/* set fonction par le id de set et cherche le bon data et le renvoei */
	/* voir segment/segment_dump*/	
  t_state		state;
  t_iterator		i;
  t_serial_buffer	buffer;
  t_id			id;
  
  set_foreach(SET_OPT_FORWARD, buffers, &i, state)
  {
	set_object(buffers, i,(void**)&buffer);
	printf("%s %s", buffer.name, name, strcmp(buffer.name, name));
  	if (!strcmp(buffer.name, name))
	{
		printf("here again\n");
		return buffer.data;
	}
  }
 return 0;
}

t_error			debug_recv_cmd()
{
  t_serial_data		cmd;	

  serial_recv(SERIAL_COM1, (t_serial_data  *) &cmd);
  debug_exec_cmd_tab(&cmd);
  free(cmd.data);
  serial_put(-1); 
  serial_send(SERIAL_COM1, "endprintf", 9);
}

t_error			debug_exec_cmd_tab(t_serial_data *cmd)
{ 
  int (*func)(void);
 
  func = (int (*)(void)) strtol(cmd->data, 0, 16);
  func();
}


t_error			debug_clean(void)
{
  /*
   * XXX
   */

  return (ERROR_NONE);
}
