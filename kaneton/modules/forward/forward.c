/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/kaneton/modules/forward/forward.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [wed nov 24 15:45:52 2010]
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

m_module_forward	_module_forward;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function sends some text.
 */

t_error			module_forward_send(char*		message,
					    t_uint32		length)
{
  module_forward_flush();

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)message,
			length);

  MODULE_LEAVE();
}

/*
 * this function flushes the pending text by sending a text message
 * before reinitializing the buffer.
 */

t_error			module_forward_flush(void)
{
  t_uint32		size = _module_forward.size;

  if (_module_forward.size == 0)
    MODULE_LEAVE();

  _module_forward.size = 0;

  if (module_forward_send(_module_forward.buffer,
			  size) != ERROR_OK)
    MODULE_ESCAPE("unable to send the buffer to the serial line");

  memset(_module_forward.buffer,
	 0x0,
	 sizeof(_module_forward.buffer));

  MODULE_LEAVE();
}

/*
 * this function adds a single character to the buffer. this function
 * is used by printf_init() so that printf() uses it every time
 * it needs to print a character, that this function actually buffers.
 *
 * then, whenever the buffer is full or the character '\n' is received,
 * the buffer is flushed into a text message.
 */

int			module_forward_write(char			c)
{
  _module_forward.buffer[_module_forward.size++] = c;

  if ((_module_forward.size >= (sizeof(_module_forward.buffer) - 1)) ||
      (c == '\n'))
    module_forward_flush();

  return (1);
}

/*
 * this function just initializes the forward.
 */

t_error			module_forward_initialize(void)
{
  module_call(console, console_message,
	      '+', "forward module loaded\n");

  platform_serial_setup(PLATFORM_SERIAL_PRIMARY,
			PLATFORM_SERIAL_BR57600,
			PLATFORM_SERIAL_8N1);

  printf_init(module_forward_write, NULL);

  MODULE_LEAVE();
}

/*
 * this function cleans everything.
 */

t_error			module_forward_clean(void)
{
  MODULE_LEAVE();
}
