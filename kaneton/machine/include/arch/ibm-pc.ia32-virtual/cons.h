/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../include/arch/ibm-pc.ia32-virtual/cons.h
 *
 * created       julien quintard   [wed jun  6 15:53:35 2007]
 * updated       julien quintard   [thu jun  7 14:35:23 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * details about console format:
 *
 * flicker   :   0 = disable    1 = enable
 * back      :   0 = black      1 = blue       2 = green      3 = cyan
 *               4 = red        5 = magenta    6 = yellow     7 = white
 * intensity :   0 = disable    1 = enable
 * front     :   0 = black      1 = blue       2 = green      3 = cyan
 *               4 = red        5 = magenta    6 = yellow     7 = white
 */

#ifndef ARCHITECTURE_CONS_H
#define ARCHITECTURE_CONS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

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
}				t_cons;

#endif
