/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.TETON/kaneton/modules/test/test.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [fri nov 26 10:56:24 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the in-kernel test system that basically waits for
 * incoming calls and launches the according tests.
 *
 * this module is not portable as it relies on the ibm-pc platform for
 * serial communication.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * this array of mappings between function names and function symbols is
 * automatically generated.
 */

extern s_module_test_function	_module_test_functions[];

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module structure.
 */
 
m_module_test*		_module_test = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function performs a very simple XOR-based checksum on the given data.
 */

t_uint32		module_test_checksum(void*		data,
					     t_uint32		size)
{
  t_uint8*		d = data;
  t_uint32		crc = 0;
  t_uint32		i;

  for (i = 0; i < size; i++)
    crc ^= d[i];

  return (crc);
}

/*
 * this function sends a message, either a text or command.
 *
 * steps:
 *
 * 1) flushes the buffer to make sure there is no pending text messages.
 * 2) construct a message by computing the CRC and setting the magic.
 * 3) send the message by issuing multiple serial_write()
 */

t_error			module_test_send(t_uint8		type,
					 char*			message,
					 t_uint32		length)
{
  t_uint32		magic;
  t_uint32		crc;

  /*
   * 1)
   */

  module_test_flush();

  /*
   * 2)
   */

  magic = MODULE_TEST_MAGIC;
  crc = module_test_checksum(message, length);

  /*
   * 3)
   */

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&magic,
			sizeof(t_uint32));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&type,
			sizeof(t_uint8));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&length,
			sizeof(t_uint32));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&crc,
			sizeof(t_uint32));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)message,
			length);

  MODULE_LEAVE();
}

/*
 * this function receives a message, either a text or command.
 */

t_error			module_test_receive(t_uint8*		type,
					    char*		message)
{
  t_uint32		magic;
  t_uint32		length;
  t_uint32		crc;

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)&magic,
		       sizeof(t_uint32));

  if (magic != MODULE_TEST_MAGIC)
    MODULE_ESCAPE("invalid magic number received");

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)type,
		       sizeof(t_uint8));

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)&length,
		       sizeof(t_uint32));

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)&crc,
		       sizeof(t_uint32));

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)message,
		       length);

  if (crc != module_test_checksum(message, length))
    MODULE_ESCAPE("invalid received message's CRC");

  MODULE_LEAVE();
}

/*
 * this function is a wrapper for sending commands.
 */

t_error			module_test_issue(char*			command)
{
  if (module_test_send(MODULE_TEST_TYPE_COMMAND,
		       command,
		       strlen(command)) != ERROR_OK)
    MODULE_ESCAPE("unable to send a command back to the client");

  MODULE_LEAVE();
}

/*
 * this function flushes the pending text by sending a text message
 * before reinitializing the buffer.
 */

t_error			module_test_flush(void)
{
  t_uint32		size = _module_test->size;

  if (_module_test->size == 0)
    MODULE_LEAVE();

  _module_test->size = 0;

  if (module_test_send(MODULE_TEST_TYPE_TEXT,
		       _module_test->buffer,
		       size) != ERROR_OK)
    MODULE_ESCAPE("unable to send the buffer back to the client");

  memset(_module_test->buffer,
	 0x0,
	 sizeof(_module_test->buffer));

  MODULE_LEAVE();
}

/*
 * this function adds a single character to the buffer. this function
 * is used by printf_init() so that printf() uses it every time
 * it needs to print a character, that this function actually buffers.
 *
 * then, whenever the buffer is full or the character '\n' is received,
 * the buffer is flushed into a text message.
 *
 * steps:
 *
 * 1) check that the module has been initialized. indeed this module
 *    could be called directly from the platform's console. this check
 *    has been put in order to ignore any message that preceeds the
 *    test module set up.
 * 2) write the character.
 * 3) flush if necessary.
 */

int			module_test_write(char			c)
{
  /*
   * 1)
   */

  if (_module_test == NULL)
    return (1);

  /*
   * 2)
   */

  _module_test->buffer[_module_test->size++] = c;

  /*
   * 3)
   */

  if ((_module_test->size >= (sizeof(_module_test->buffer) - 1)) ||
      (c == '\n'))
    module_test_flush();

  return (1);
}

/*
 * this function locates a test function according to its symbol name.
 */

t_error			module_test_locate(char*		symbol,
					   f_module_test*	function)
{
  unsigned int  i;

  for (i = 0; _module_test_functions[i].symbol != NULL; i++)
    if (strcmp(_module_test_functions[i].symbol, symbol) == 0)
      {
	*function = _module_test_functions[i].function;

	MODULE_LEAVE();
      }

  MODULE_ESCAPE("unable to locate the test symbol");
}

/*
 * this function takes a test function symbol and calls it.
 */

t_error			module_test_call(char*			symbol)
{
  f_module_test		test;

  if (module_test_locate(symbol, &test) != ERROR_OK)
    MODULE_ESCAPE("invalid test symbol");

  module_test_issue("[enter]");

  test();

  module_test_issue("[leave]");

  MODULE_LEAVE();
}

/*
 * this function dumps the registered test functions.
 */

void			module_test_dump(void)
{
  unsigned int  i;

  module_call(console, console_message,
	      '#', "[tests]\n");

  for (i = 0; _module_test_functions[i].symbol != NULL; i++)
    module_call(console, console_message,
		'#', "  [%s] 0x%x\n",
		_module_test_functions[i].symbol,
		_module_test_functions[i].function);
}

/*
 * this function actually runs the test system by first initializing
 * the serial line, switching printf() before performing a handshake
 * and waiting for commands.
 *
 * steps:
 *
 * 1) allocate memory for the module's structure.
 * 2) set up the serial port.
 * 3) initialize printf() so that every displayed text is forward onto the
 *    serial port.
 * 4) issues the enter command which the client must be waiting for.
 * 5) wait for commands and treat them.
 */

t_error			module_test_run(void)
{
  s_module_test_command	commands[] =
    {
      { "[call] ", module_test_call },
      { NULL, NULL }
    };

  module_call(console, console_message,
	      '+', "test module loaded\n");

  /*
   * 1)
   */

  if ((_module_test = malloc(sizeof(m_module_test))) == NULL)
    MODULE_ESCAPE("unable to allocate memory for the test module's structure");

  memset(_module_test, 0x0, sizeof(m_module_test));

  /*
   * 2)
   */

  platform_serial_setup(PLATFORM_SERIAL_PRIMARY,
			PLATFORM_SERIAL_BR57600,
			PLATFORM_SERIAL_8N1);

  /*
   * 3)
   */

  printf_init(module_test_write, NULL);

  /*
   * 4)
   */

  module_test_issue("[ready]");

  /*
   * 5)
   */

  while (1)
    {
      char		message[512];
      t_uint8		type;
      unsigned int	i;

      memset(message, 0x0, sizeof(message));

      if (module_test_receive(&type, message) != ERROR_OK)
	MODULE_ESCAPE("unable to received a test request\n");

      if (type != MODULE_TEST_TYPE_COMMAND)
	MODULE_ESCAPE("invalid command type\n");

      for (i = 0; commands[i].command != NULL; i++)
	{
	  if (strncmp(commands[i].command,
		      message,
		      strlen(commands[i].command)) == 0)
	    {
	      t_uint32		offset = strlen(commands[i].command);

	      if (commands[i].function(message + offset) != ERROR_OK)
		MODULE_ESCAPE("an error occured in the triggered "
			      "command\n");
	    }
	}
    }

  MODULE_LEAVE();
}
