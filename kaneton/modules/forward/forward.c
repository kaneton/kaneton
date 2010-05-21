/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/forward/forward.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [mon may 17 13:16:08 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements a module which forwards everything displayed text
 * to the serial line.
 *
 * this module is not portable as it relies on the ibm-pc platform.
 *
 * besides, this module also relies on the console module in order
 * to forward the call to console as well, such as console_message() etc.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module structure.
 */

m_forward		forward;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function sends some text.
 */

t_error			forward_send(char*			message,
				     t_uint32			length)
{
  t_uint32		magic;
  t_uint32		crc;

  forward_flush();

  ibmpc_serial_write(IBMPC_SERIAL_PRIMARY, (t_uint8*)message, length);

  return (ERROR_NONE);
}

/*
 * this function flushes the pending text by sending a text message
 * before reinitializing the buffer.
 */

t_error			forward_flush(void)
{
  t_uint32		size = forward.size;

  if (forward.size == 0)
    return (ERROR_NONE);

  forward.size = 0;

  if (forward_send(forward.buffer, size) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memset(forward.buffer, 0x0, sizeof(forward.buffer));

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

int			forward_write(char				c)
{
  forward.buffer[forward.size++] = c;

  if ((forward.size >= (sizeof(forward.buffer) - 1)) ||
      (c == '\n'))
    forward_flush();

  return (1);
}

/*
 * this function just initializes the forward.
 */

t_error			forward_initialize(void)
{
  module_call(console, console_message, '+', "forward module loaded\n");

  ibmpc_serial_init(IBMPC_SERIAL_PRIMARY,
		    IBMPC_SERIAL_BR57600,
		    IBMPC_SERIAL_8N1);

  printf_init(forward_write, NULL);

  return (ERROR_NONE);
}

/*
 * this function cleans everything.
 */

t_error			forward_clean(void)
{
  return (ERROR_NONE);
}
