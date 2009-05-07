/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...n/machine/platform/ibm-pc/include/cons.h
 *
 * created       julien quintard   [wed jun  6 15:53:35 2007]
 * updated       matthieu bucchianeri   [sun jun 17 17:38:58 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

#ifndef PLATFORM_CONSOLE_H
#define PLATFORM_CONSOLE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

#include <library/stdarg.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IBMPC_CONSOLE_ADDR		0xb8000
#define IBMPC_CONSOLE_SIZE		0xfa0
#define IBMPC_CONSOLE_LINES		25
#define IBMPC_CONSOLE_COLUMNS		80
#define IBMPC_CONSOLE_BPC		2
#define IBMPC_CONSOLE_TAB		8

#define IBMPC_CONSOLE_FLI		(1 << 7)
#define IBMPC_CONSOLE_INT		(1 << 3)

#define IBMPC_CONSOLE_BLACK		0x0
#define IBMPC_CONSOLE_BLUE		0x1
#define IBMPC_CONSOLE_GREEN		0x2
#define IBMPC_CONSOLE_CYAN		0x3
#define IBMPC_CONSOLE_RED		0x4
#define IBMPC_CONSOLE_MAGENTA		0x5
#define IBMPC_CONSOLE_YELLOW		0x6
#define IBMPC_CONSOLE_WHITE		0x7

/*
 * ---------- macro functions -------------------------------------------------
 */

#define IBMPC_CONSOLE_FRONT(_color_)	(_color_)
#define IBMPC_CONSOLE_BACK(_color_)	(_color_ << 4)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint16			line;
  t_uint16			column;
  t_uint8			attribute;
  char*				vga;
}				t_ibmpc_console;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../console.c
 */

/*
 * ../console.c
 */

void			ibmpc_console_clear(void);

void			ibmpc_console_scroll(t_uint16		lines);

void			ibmpc_console_attribute(t_uint8		attribute);

int			ibmpc_console_print_char(char		c);

void			ibmpc_console_print_string(char*	string);

void			ibmpc_console_message(char		indicator,
					      char*		fmt,
					      va_list		args);

t_error			ibmpc_console_initialize(void);

t_error			ibmpc_console_clean(void);


/*
 * eop
 */

#endif
