/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/test/test.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [fri may  1 19:18:16 2009]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the in-kernel test system that basically waits for
 * incoming calls and launches the according tests.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern s_test		test_functions[];

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the manager structure.
 */

m_test			test;

/*
 * the test commands.
 */

struct
{
  char*			command;
  void			(*function)(char*);
} test_commands[] =
  {
    { "[call] ", test_call },
    { NULL, NULL }
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */
t_uint32		test_checksum(void*			data,
				      t_uint32			size)
{
  t_uint8*		d = data;
  t_uint32		crc = 0;
  t_uint32		i;

  for (i = 0; i < size; i++)
    crc ^= d[i];

  return (crc);
}

/*
 * XXX
 */

t_error			test_send(t_uint8			type,
				  char*				message,
				  t_uint32			length)
{
  t_uint32		magic;
  t_uint32		crc;

  test_flush();

  magic = TEST_MAGIC;
  crc = chk_sum(message, length);

  // XXX printf("[XXX][send] 0x%x %u %u 0x%x '%s'\n", magic, type, length, crc, message);

  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&magic, sizeof(t_uint32));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&type, sizeof(t_uint8));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&length, sizeof(t_uint32));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&crc, sizeof(t_uint32));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)message, length);

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			test_receive(t_uint8*			type,
				     char*			message)
{
  t_uint32		magic;
  t_uint32		length;
  t_uint32		crc;

  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)&magic, sizeof(t_uint32));

  if (magic != TEST_MAGIC)
    {
      printf("[error] invalid magic\n");

      return (ERROR_UNKNOWN);
    }

  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)type, sizeof(t_uint8));
  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)&length, sizeof(t_uint32));
  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)&crc, sizeof(t_uint32));

  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)message, length);

  if (crc != chk_sum(message, length))
    {
      printf("[error] invalid CRC\n");

      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			test_issue(char*			command)
{
  return (test_send(TEST_TYPE_COMMAND, command, strlen(command)));
}

/*
 * XXX
 */

t_error			test_flush(void)
{
  t_uint32		size = test.size;

  if (test.size == 0)
    return (ERROR_NONE);

  test.size = 0;

  if (test_send(TEST_TYPE_TEXT, test.buffer, size) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memset(test.buffer, 0x0, sizeof(test.buffer));

  return (ERROR_NONE);
}

int			test_write(char				c)
{
  test.buffer[test.size++] = c;

  if ((test.size >= (sizeof(test.buffer) - 1)) ||
      (c == '\n'))
      test_flush();

  return (1);
}

/*
 * this function locates a test function according to its symbol name.
 */

f_test			test_locate(char*			symbol)
{
  unsigned int  i;

  for (i = 0; test_functions[i].symbol != NULL; i++)
    if (strcmp(test_functions[i].symbol, symbol) == 0)
      return (test_functions[i].function);

  return (NULL);
}

/*
 * this function takes a test function symbol and executes it.
 */

void			test_call(char*				symbol)
{
  f_test		test;

  if ((test = test_locate(symbol)) == NULL)
    {
      printf("[error] unknown test '%s'\n", symbol);

      return;
    }

  test_issue("[enter]");

  test();

  test_issue("[leave]");
}

/*
 * this function dumps the registered test functions.
 */

void			test_dump(void)
{
  unsigned int  i;

  printf("[tests]\n");

  for (i = 0; test_functions[i].symbol != NULL; i++)
    printf("  [%s] 0x%x\n", test_functions[i].symbol, test_functions[i].function);
}

/*
 * this function actually runs the test system.
 */

t_error			test_run(void)
{
  cons_msg('+', "starting the test module\n");

  /*
   * 1)
   */
  ibmpc_serial_init(SERIAL_PRIMARY, SERIAL_BR57600, SERIAL_8N1);

  /*
   * 2)
   */
  printf_init(test_write, NULL);

  /*
   * 3)
   */
  test_issue("[ready]");

  /*
   * 4)
   */
  while (1)
    {
      char		message[512];
      t_uint8		type;
      unsigned int	i;

      memset(message, 0x0, sizeof(message));

      if (test_receive(&type, message) != ERROR_NONE)
	continue;

      if (type != TEST_TYPE_COMMAND)
	continue;

      for (i = 0; test_commands[i].command != NULL; i++)
	{
	  if (strncmp(test_commands[i].command,
		      message,
		      strlen(test_commands[i].command)) == 0)
	    test_commands[i].function(message +
				      strlen(test_commands[i].command));
	}
    }

  return (ERROR_NONE);
}
