/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/debug.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [thu aug 10 16:33:42 2006]
 */

/*
 *  this file must not be used by students.
 *
 */

/*                                                                  [cut] k1 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>
#include <core/debug.h>

/*
 * ---------- globals ---------------------------------------------------------
 */
#ifdef SERIAL
i_set			buffers;

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
  set_reserve(ll, SET_OPT_ALLOC, sizeof(t_serial_buffer), &buffers);

  while(1) 	debug_recv();

  return (ERROR_NONE);
}

/*
 * this function cleans the debug manager.
 */

t_error			debug_recv(void)
{
  t_serial_data		recv_type;

  serial_recv(SERIAL_COM1, &recv_type);

  if (!strcmp((char*)recv_type.data, "command"))
	debug_recv_cmd();
  else if (!strcmp((char*)recv_type.data, "loadfile"))
  	load_data();
  else
	printf("debug receive unknown type\n");
  /* rajouter un unload file pour cleaner ! */
  free(recv_type.data);

  return (ERROR_NONE);
}


t_error			load_data(void)
{
  t_serial_data		cmd;
  t_serial_buffer	buffer;

  serial_recv(SERIAL_COM1, (t_serial_data  *) &cmd);
  buffer.name = (char*)cmd.data;
/*  free(cmd.data);
*/

  serial_recv(SERIAL_COM1, (t_serial_data  *) &cmd);
  buffer.data = cmd.data;
  set_add(buffers, &buffer);

  /*free(cmd.data);*/

  return (ERROR_NONE);
}

char*		get_data(char *name)
{
  t_state		state;
  t_iterator		i;
  t_serial_buffer*	buffer;

  set_foreach(SET_OPT_FORWARD, buffers, &i, state)
  {
	set_object(buffers, i,(void**)&buffer);
  	if (!strcmp(buffer->name, name))
		return (buffer->data);
  }
 return (0);
}

t_error			debug_recv_cmd(void)
{
  t_serial_data		cmd;

  serial_recv(SERIAL_COM1, (t_serial_data  *) &cmd);
  debug_exec_cmd_tab(&cmd);
  free(cmd.data);
  serial_put(-1);
  serial_send(SERIAL_COM1, (t_uint8*)"endprintf", 9);

  return (ERROR_NONE);
}

t_error			debug_exec_cmd_tab(t_serial_data *cmd)
{
  int (*func)(void);

  *strchr((char*)cmd->data, '/') = 0;
  func = (int (*)(void)) strtol((char*)cmd->data + strlen((char*)cmd->data) + 1,
				0, 16);
  cons_msg('+', "running test %s @ 0x%p\n", (char*)cmd->data, func);
  printf_init(serial_put, 0);
  func();
  printf_init(cons_print_char, cons_attr);
  return (ERROR_NONE);
}


t_error			debug_clean(void)
{
  /*
   * XXX
   */

  return (ERROR_NONE);
}
#endif

/*                                                                 [cut] /k1 */
