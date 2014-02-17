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
 * updated       julien quintard   [sun may  8 12:21:12 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the in-kernel test system that basically waits for
 * incoming calls and launches the according tests.
 *
 * this module is partially portable as it relies on the platform for serial
 * communication.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#if !defined(MODULE_console)
# error "the 'test' module depends upon the 'console' module"
#endif

#if defined(MODULE_forward)
# error "the 'forward' module cannot be activated with the 'test' module"
#endif

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

extern ms_test_function	_module_test_functions[];

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module manager.
 */
 
mm_test			_module_test;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function performs a very simple XOR-based checksum on the given data.
 */

t_uint32		module_test_checksum(void*		data,
					     t_uint32		size,
					     t_uint32*		crc)
{
  t_uint8*		d = data;
  t_uint32		i;

  for (*crc = 0, i = 0; i < size; i++)
    *crc ^= d[i];

  MODULE_LEAVE();
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

t_status		module_test_send(t_uint8		type,
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

  if (module_test_checksum(message, length, &crc) != STATUS_OK)
    MACHINE_ESCAPE("unable to compute the message's CRC");

  /*
   * 3)
   */

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&magic,
			sizeof (t_uint32));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&type,
			sizeof (t_uint8));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&length,
			sizeof (t_uint32));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)&crc,
			sizeof (t_uint32));

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)message,
			length);

  MODULE_LEAVE();
}

/*
 * this function receives a message, either a text or command.
 *
 * steps:
 *
 * 1) receive and check the magic number.
 * 2) receive the content.
 * 3) check the checksum.
 */

t_status		module_test_receive(t_uint8*		type,
					    char*		message)
{
  t_uint32		magic;
  t_uint32		length;
  t_uint32		crc;
  t_uint32		c;

  /*
   * 1)
   */

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)&magic,
		       sizeof (t_uint32));

  if (magic != MODULE_TEST_MAGIC)
    MODULE_ESCAPE("invalid magic number received");

  /*
   * 2)
   */

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)type,
		       sizeof (t_uint8));

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)&length,
		       sizeof (t_uint32));

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)&crc,
		       sizeof (t_uint32));

  platform_serial_read(PLATFORM_SERIAL_PRIMARY,
		       (t_uint8*)message,
		       length);

  /*
   * 3)
   */

  if (module_test_checksum(message, length, &c) != STATUS_OK)
    MODULE_ESCAPE("unable to compute the message's CRC");

  if (crc != c)
    MODULE_ESCAPE("invalid received message's CRC");

  MODULE_LEAVE();
}

/*
 * this function is a wrapper for sending commands.
 */

t_status		module_test_issue(char*			command)
{
  if (module_test_send(MODULE_TEST_TYPE_COMMAND,
		       command,
		       strlen(command)) != STATUS_OK)
    MODULE_ESCAPE("unable to send a command back to the client");

  MODULE_LEAVE();
}

/*
 * this function flushes the pending text by sending a text message
 * before reinitializing the buffer.
 *
 * steps:
 *
 * 1) if there is nothing in the buffer, just leave.
 * 2) send the buffered text to the serial line. note that the buffer size
 *    is set to zero to avoid infinite loops since send() will call flush().
 * 3) re-initialize the buffer.
 */

t_status		module_test_flush(void)
{
  t_uint32		size;

  /*
   * 1)
   */

  size = _module_test.size;

  if (_module_test.size == 0)
    MODULE_LEAVE();

  /*
   * 2)
   */

  _module_test.size = 0;

  if (module_test_send(MODULE_TEST_TYPE_TEXT,
		       _module_test.buffer,
		       size) != STATUS_OK)
    MODULE_ESCAPE("unable to send the buffer back to the client");

  /*
   * 3)
   */

  memset(_module_test.buffer,
	 0x0,
	 sizeof (_module_test.buffer));

  MODULE_LEAVE();
}

/*
 * this function adds a single character to the buffer. this function
 * is called by format().
 *
 * then, whenever the buffer is full or the character '\n' is received,
 * the buffer is flushed into a text message.
 *
 * steps:
 *
 * 1) record the character.
 * 2) flush if necessary.
 */

void			module_test_character(char		c)
{
  /*
   * 1)
   */

  _module_test.buffer[_module_test.size++] = c;

  /*
   * 2)
   */

  if ((_module_test.size >= (sizeof (_module_test.buffer) - 1)) ||
      (c == '\n'))
    module_test_flush();
}

/*
 * this function locates a test function according to its symbol name.
 */

t_status		module_test_locate(char*		symbol,
					   mf_test*		function)
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
 *
 * steps:
 *
 * 1) locate the symbol in the table.
 * 2) issue the enter command, specifying the beginning of the test.
 * 3) call the test function.
 * 4) issue the leave command.
 */

t_status		module_test_call(char*			symbol)
{
  mf_test		function;

  /*
   * 1)
   */

  if (module_test_locate(symbol, &function) != STATUS_OK)
    MODULE_ESCAPE("invalid test symbol");

  /*
   * 2)
   */

  module_test_issue("[enter]");

  /*
   * 3)
   */

  function();

  /*
   * 4)
   */

  module_test_issue("[leave]");

  MODULE_LEAVE();
}

/*
 * this function dumps the registered test functions.
 */

void			module_test_dump(void)
{
  unsigned int  i;

  module_call(console, message,
	      '#', "[tests]\n");

  for (i = 0; _module_test_functions[i].symbol != NULL; i++)
    module_call(console, message,
		'#', "  [%s] 0x%x\n",
		_module_test_functions[i].symbol,
		_module_test_functions[i].function);
}

/*
 * this function actually runs the test system by first initializing
 * the serial line, performing a handshake and waiting for commands.
 *
 * steps:
 *
 * 1) display a message.
 * 2) set up the serial port.
 * 3) tell the console module to forward everything to the test module.
 * 4) issues the ready command which the client must be waiting for.
 * 5) wait for commands and handle them.
 */

t_status		module_test_run(void)
{
  ms_test_command	commands[] =
    {
      { "[call] ", module_test_call },
      { NULL, NULL }
    };

  /*
   * 1)
   */

  module_call(console, message,
	      '+', "running the 'test' module\n");

  /*
   * 2)
   */

  platform_serial_initialize();

  platform_serial_setup(PLATFORM_SERIAL_PRIMARY,
			PLATFORM_SERIAL_BR57600,
			PLATFORM_SERIAL_8N1);

  /*
   * 3)
   */

  module_call(console, set,
	      module_test_character,
	      NULL);

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

      memset(message, 0x0, sizeof (message));

      if (module_test_receive(&type, message) != STATUS_OK)
	MODULE_ESCAPE("unable to received a test request");

      if (type != MODULE_TEST_TYPE_COMMAND)
	MODULE_ESCAPE("invalid command type");

      for (i = 0; commands[i].command != NULL; i++)
	{
	  if (strncmp(commands[i].command,
		      message,
		      strlen(commands[i].command)) == 0)
	    {
	      t_uint32		offset = strlen(commands[i].command);

	      if (commands[i].function(message + offset) != STATUS_OK)
		MODULE_ESCAPE("an error occured in the triggered "
			      "command");

	      break;
	    }
	}

      if (commands[i].command == NULL)
	MODULE_ESCAPE("unknown command name");
    }

  MODULE_LEAVE();
}

/*
 * this function loads the module.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the manager's structure.
 * 3) initialize the serial manager.
 */

t_status		module_test_load(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "loading the 'test' module\n");

  /*
   * 2)
   */

  memset(&_module_test, 0x0, sizeof (mm_test));

  /*
   * 3)
   */

  if (platform_serial_initialize() != STATUS_OK)
    MODULE_ESCAPE("unable to initialize the serial manager");

  MODULE_LEAVE();
}

/*
 * this function unloads the module.
 *
 * steps:
 *
 * 1) display a message.
 * 2) clean the serial manager.
 */

t_status		module_test_unload(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "unloading the 'test' module\n");

  /*
   * 2)
   */

  if (platform_serial_clean() != STATUS_OK)
    MODULE_ESCAPE("unable to clean the serial manager");

  MODULE_LEAVE();
}
