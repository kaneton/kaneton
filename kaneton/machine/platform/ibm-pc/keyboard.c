/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...neton/machine/platform/ibm-pc/keyboard.c
 *
 * created       matthieu bucchianeri   [sun jun 17 19:04:00 2007]
 * updated       matthieu bucchianeri   [wed jan  9 11:44:33 2008]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>
#include <libc/libc.h>

#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

IBMPC_KEYBOARD_MAPPING(scancodes);

/*
 * ---------- functions -------------------------------------------------------
 */

/* [cut k2] */
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
	  case 'd':
#ifdef VIEW_ENABLE
	    view_download();
#else
	    cons_msg('!', "view manager support not enabled\n");
#endif
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
  return (event_reserve(33, EVENT_FUNCTION, EVENT_HANDLER(ibmpc_kbd_handler), 0));
}
/* [/cut] */
