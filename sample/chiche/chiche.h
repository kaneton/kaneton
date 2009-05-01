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

#ifndef DRIVERS_CONS_SIMPLE_H
#define DRIVERS_CONS_SIMPLE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libkaneton.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define CONS_ADDR		0xb8000
#define CONS_SIZE		0xfa0
#define CONS_LINES		25
#define CONS_COLUMNS		80
#define CONS_BPC		2
#define CONS_TAB		8

#define CONS_FLI		(1 << 7)
#define CONS_INT		(1 << 3)

#define CONS_BLACK		0x0
#define CONS_BLUE		0x1
#define CONS_GREEN		0x2
#define CONS_CYAN		0x3
#define CONS_RED		0x4
#define CONS_MAGENTA		0x5
#define CONS_YELLOW		0x6
#define CONS_WHITE		0x7

/*
 * ---------- macro functions -------------------------------------------------
 */

#define CONS_FRONT(_color_)	(_color_)
#define CONS_BACK(_color_)	(_color_ << 4)

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

// XXX
extern t_uint64 __task_id;
extern t_uint64 __as_id;

#endif
