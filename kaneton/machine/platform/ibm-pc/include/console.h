/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/platform/ibm-pc/include/console.h
 *
 * created       julien quintard   [wed jun  6 15:53:35 2007]
 * updated       julien quintard   [sun dec  5 14:59:10 2010]
 */

#ifndef PLATFORM_CONSOLE_H
#define PLATFORM_CONSOLE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

#include <library/library.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PLATFORM_CONSOLE_ADDRESS	0xb8000
#define PLATFORM_CONSOLE_SIZE		0xfa0
#define PLATFORM_CONSOLE_LINES		25
#define PLATFORM_CONSOLE_COLUMNS	80
#define PLATFORM_CONSOLE_BPC		2	/* bytes per character */
#define PLATFORM_CONSOLE_TABULATION	8

#define PLATFORM_CONSOLE_FLICKER	(1 << 7)
#define PLATFORM_CONSOLE_BRIGHT		(1 << 3)

#define PLATFORM_CONSOLE_BLACK		0x0
#define PLATFORM_CONSOLE_BLUE		0x1
#define PLATFORM_CONSOLE_GREEN		0x2
#define PLATFORM_CONSOLE_CYAN		0x3
#define PLATFORM_CONSOLE_RED		0x4
#define PLATFORM_CONSOLE_MAGENTA	0x5
#define PLATFORM_CONSOLE_YELLOW		0x6
#define PLATFORM_CONSOLE_WHITE		0x7

/*
 * ---------- macro functions -------------------------------------------------
 */

#define PLATFORM_CONSOLE_FRONT(_color_)	(_color_)
#define PLATFORM_CONSOLE_BACK(_color_)	(_color_ << 4)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint16		line;
  t_uint16		column;
  t_uint8		attribute;
  char*			vga;
}			m_platform_console;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../console.c
 */

/*
 * ../console.c
 */

void			platform_console_clear(void);

void			platform_console_scroll(t_uint16	lines);

void			platform_console_attribute(t_uint8	attribute);

void			platform_console_character(char			c);

t_error			platform_console_initialize(void);

t_error			platform_console_clean(void);


/*
 * eop
 */

#endif
