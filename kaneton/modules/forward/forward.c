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
 * updated       julien quintard   [thu apr  7 15:13:11 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements a module which forwards everything text displayed
 * through the console module to the serial line.
 *
 * this module is only partially portable as it relies on the platform
 * serial functionality.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#if !defined(MODULE_console)
# error "the 'forward' module depends upon the 'console' module"
#endif

#if defined(MODULE_test)
# error "the 'test' module cannot be activated with the 'forward' module"
#endif

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module manager.
 */

mm_forward		_module_forward;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function forces the module to send the buffered and given text to
 * the serial line.
 *
 * steps:
 *
 * 1) flush the buffer hence forcing it to be sent to the serial line.
 * 2) send the given message to the serial line.
 */

t_status		module_forward_send(char*		message,
					    t_uint32		length)
{
  /*
   * 1)
   */

  module_forward_flush();

  /*
   * 2)
   */

  platform_serial_write(PLATFORM_SERIAL_PRIMARY,
			(t_uint8*)message,
			length);

  MODULE_LEAVE();
}

/*
 * this function flushes the pending text by sending the buffered text before
 * reinitializing the buffer.
 *
 * steps:
 *
 * 1) if there is nothing in the buffer, leave.
 * 2) send the buffered text. note that the size if first set to zero
 *    in order to avoid infinite loops since the send() function will
 *    call the flush() function
 * 3) reset the buffer.
 */

t_status		module_forward_flush(void)
{
  t_uint32		size;

  /*
   * 1)
   */

  size = _module_forward.size;

  if (_module_forward.size == 0)
    MODULE_LEAVE();

  /*
   * 2)
   */

  _module_forward.size = 0;

  if (module_forward_send(_module_forward.buffer,
			  size) != STATUS_OK)
    MODULE_ESCAPE("unable to send the buffer to the serial line");

  /*
   * 3)
   */

  memset(_module_forward.buffer,
	 0x0,
	 sizeof (_module_forward.buffer));

  MODULE_LEAVE();
}

/*
 * this function adds a single character to the buffer.
 *
 * steps:
 *
 * 1) add the character to the buffer.
 * 2) whenever the buffer is full or the character '\n' is received, the
 *    buffer is flushed into a text message.
 */

void			module_forward_character(char		c)
{
  /*
   * 1)
   */

  _module_forward.buffer[_module_forward.size++] = c;

  /*
   * 2)
   */

  if ((_module_forward.size >= (sizeof (_module_forward.buffer) - 1)) ||
      (c == '\n'))
    module_forward_flush();
}

/*
 * this function loads the module.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the manager's structure.
 * 3) initialize the platform serial line.
 * 4) initialize the console module in order to receive everything.
 */

t_status		module_forward_load(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "loading the 'forward' module\n");

  /*
   * 2)
   */

  memset(&_module_forward, 0x0, sizeof (mm_forward));

  /*
   * 3)
   */

  platform_serial_setup(PLATFORM_SERIAL_PRIMARY,
			PLATFORM_SERIAL_BR57600,
			PLATFORM_SERIAL_8N1);

  /*
   * 4)
   */

  module_call(console, set,
	      module_forward_character,
	      NULL);

  MODULE_LEAVE();
}

/*
 * this function unloads the module.
 */

t_status		module_forward_unload(void)
{
  module_call(console, message,
	      '+', "unloading the 'forward' module\n");

  MODULE_LEAVE();
}
