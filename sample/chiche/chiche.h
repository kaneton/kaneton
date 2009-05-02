/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton.bak/drivers/cons-simple/cons-simple.h
 *
 * created       matthieu bucchianeri   [thu may 31 21:28:04 2007]
 * updated       matthieu bucchianeri   [sat jun  9 22:37:53 2007]
 */

#ifndef SAMPLE_CHICHE_CHICHE_H
#define SAMPLE_CHICHE_CHICHE_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "libkaneton/include/libkaneton.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define CONSOLE_ADDR		0xb8000
#define CONSOLE_SIZE		0xfa0
#define CONSOLE_LINES		25
#define CONSOLE_COLUMNS		80
#define CONSOLE_BPC		2
#define CONSOLE_TAB		8

#define CONSOLE_FLI		(1 << 7)
#define CONSOLE_INT		(1 << 3)

#define CONSOLE_BLACK		0x0
#define CONSOLE_BLUE		0x1
#define CONSOLE_GREEN		0x2
#define CONSOLE_CYAN		0x3
#define CONSOLE_RED		0x4
#define CONSOLE_MAGENTA		0x5
#define CONSOLE_YELLOW		0x6
#define CONSOLE_WHITE		0x7

/*
 * ---------- macro functions -------------------------------------------------
 */

#define CONSOLE_FRONT(_color_)	(_color_)
#define CONSOLE_BACK(_color_)	(_color_ << 4)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint16			line;
  t_uint16			column;
  t_uint8			attr;
  char*				vga;
}				t_simple_cons;

#endif
