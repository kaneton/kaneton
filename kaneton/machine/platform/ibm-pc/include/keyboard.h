/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...chine/platform/ibm-pc/include/keyboard.h
 *
 * created       matthieu bucchianeri   [sun jun 17 19:07:08 2007]
 * updated       matthieu bucchianeri   [sun jun 17 19:09:05 2007]
 */

#ifndef PLATFORM_KEYBOARD_H
#define PLATFORM_KEYBOARD_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define	IBMPC_KEYBOARD_IRQ	33

/*
 * ---------- macro functions -------------------------------------------------
 */

#define IBMPC_KEYBOARD_MAPPING(_name_)					\
  static const char       (_name_)[] =					\
  {									\
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',			\
    '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r',			\
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,			\
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';',			\
    '\'', '`', '-', '\\', 'z', 'x', 'c', 'v', 'b', 'n',			\
    'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0,				\
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0					\
  };

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../keyboard.c
 */

/*
 * ../keyboard.c
 */

t_error			ibmpc_keyboard_init(void);


/*
 * eop
 */

#endif
