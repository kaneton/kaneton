/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...neton/machine/platform/ibm-pc/keyboard.c
 *
 * created       matthieu bucchianeri   [sun jun 17 19:04:00 2007]
 * updated       matthieu bucchianeri   [sun jun 17 22:10:36 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>
#include <libc.h>

#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

IBMPC_KEYBOARD_MAPPING(scancodes);

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * keyboard irq handler.
 */

static void		ibmpc_kbd_handler(t_id			id)
{
  t_uint8               scancode;

  INB(0x60, scancode);

  if (scancode < 70)
    {
      switch (scancodes[scancode])
	{
	  case 's':
	    printf("\n");
	    scheduler_dump();
	    printf("\n");
	    break;
	  default:
	    printf("%c", scancodes[scancode]);
	}
    }
}

/*
 * initialize the keyboard.
 */

t_error			ibmpc_keyboard_init(void)
{
  return (event_reserve(33, EVENT_FUNCTION, EVENT_HANDLER(ibmpc_kbd_handler)));
}
