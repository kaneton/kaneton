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
 * updated       julien quintard   [sat may  2 11:10:44 2009]
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

/*
 * this array of mappings between function names and function symbols is
 * automatically generated.
 */

extern s_test_function	test_functions[];

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module structure.
 */

m_test			test;

/*
 * the test commands.
 */

s_test_command		test_commands[] =
  {
    { "[call] ", test_call },
    { NULL, NULL }
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function performs a very simple XOR-based checksum on the given data.
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
 * this function sends a message, either a text or command.
 *
 * steps:
 *
 * 1) flushes the buffer to make sure there is no pending text messages.
 * 2) construct a message by computing the CRC and setting the magic.
 * 3) send the message by issuing multiple serial_write()
 */

t_error			test_send(t_uint8			type,
				  char*				message,
				  t_uint32			length)
{
  t_uint32		magic;
  t_uint32		crc;

  /*
   * 1)
   */
  test_flush();

  /*
   * 2)
   */
  magic = TEST_MAGIC;
  crc = chk_sum(message, length);

  /*
   * 3)
   */
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&magic, sizeof(t_uint32));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&type, sizeof(t_uint8));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&length, sizeof(t_uint32));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)&crc, sizeof(t_uint32));
  ibmpc_serial_write(SERIAL_PRIMARY, (t_uint8*)message, length);

  return (ERROR_NONE);
}

/*
 * this function receives a message, either a text or command.
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
      cons_msg('!', "wrong magic\n");

      return (ERROR_UNKNOWN);
    }

  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)type, sizeof(t_uint8));
  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)&length, sizeof(t_uint32));
  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)&crc, sizeof(t_uint32));

  ibmpc_serial_read(SERIAL_PRIMARY, (t_uint8*)message, length);

  if (crc != chk_sum(message, length))
    {
      cons_msg('!', "wrong CRC\n");

      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * this function is a wrapper for sending commands.
 */

t_error			test_issue(char*			command)
{
  return (test_send(TEST_TYPE_COMMAND, command, strlen(command)));
}

/*
 * this function flushes the pending text by sending a text message
 * before reinitializing the buffer.
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

/*
 * this function adds a single character to the buffer. this function
 * is used by printf_init() so that printf() uses it every time
 * it needs to print a character, that this function actually buffers.
 *
 * then, whenever the buffer is full or the character '\n' is received,
 * the buffer is flushed into a text message.
 */

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
 * this function takes a test function symbol and calls it.
 */

void			test_call(char*				symbol)
{
  f_test		test;

  if ((test = test_locate(symbol)) == NULL)
    {
      cons_msg('!', "unknown test '%s'", symbol);

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

  cons_msg('#', "[tests]\n");

  for (i = 0; test_functions[i].symbol != NULL; i++)
    cons_msg('#', "  [%s] 0x%x\n", test_functions[i].symbol, test_functions[i].function);
}

/*
 * this function actually runs the test system by first initializing
 * the serial line, switching printf() before performing a handshake
 * and waiting for commands.
 */

t_error			test_run(void)
{
  cons_msg('+', "test module loaded\n");

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

/* XXX

#ifdef TESTSUITE_MANUAL_ENABLE
  extern i_thread kthread;

  kthread = ID_UNUSED;

  //  STI();

  cons_msg('+', "running manual tests\n");
  check_tests();

  CLI();

#ifdef TESTSUITE_FAST_REBOOT
  // disable paging to remap the bootloader
  asm volatile("movl %%cr0, %%eax\n\t"
	       "andl $0x7FFFFFFF, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");

// back to the bootloader
  return;
#else
  while (1)
    ;
#endif

  kthread = 0;
#endif

#if TESTSUITE_DEBUG_ENABLE
  extern i_thread kthread;

  cons_msg('+', "starting debug manager\n");
  kthread = ID_UNUSED;

#ifdef IA32_DEPENDENT
  STI();
#endif
  debug_initialize();
#ifdef IA32_DEPENDENT
  CLI();
#endif

#ifdef TESTSUITE_FAST_REBOOT
// disable paging to remap the bootloader
  asm volatile("movl %%cr0, %%eax\n\t"
	       "andl $0x7FFFFFFF, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");

// back to the bootloader
  return;
#else
  while (1)
    ;
#endif

  kthread = 0;
#endif

*/
