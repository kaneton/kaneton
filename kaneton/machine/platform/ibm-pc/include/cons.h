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

#ifndef PLATFORM_CONS_H
#define PLATFORM_CONS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IBMPC_CONS_ADDR		0xb8000
#define IBMPC_CONS_SIZE		0xfa0
#define IBMPC_CONS_LINES	25
#define IBMPC_CONS_COLUMNS	80
#define IBMPC_CONS_BPC		2
#define IBMPC_CONS_TAB		8

#define IBMPC_CONS_FLI		(1 << 7)
#define IBMPC_CONS_INT		(1 << 3)

#define IBMPC_CONS_BLACK	0x0
#define IBMPC_CONS_BLUE		0x1
#define IBMPC_CONS_GREEN	0x2
#define IBMPC_CONS_CYAN		0x3
#define IBMPC_CONS_RED		0x4
#define IBMPC_CONS_MAGENTA	0x5
#define IBMPC_CONS_YELLOW	0x6
#define IBMPC_CONS_WHITE	0x7

/*
 * ---------- macro functions -------------------------------------------------
 */

#define IBMPC_CONS_FRONT(_color_)	(_color_)
#define IBMPC_CONS_BACK(_color_)	(_color_ << 4)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint16			line;
  t_uint16			column;
  t_uint8			attr;
  char*				vga;
}				t_ibmpc_cons;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../cons-text.c
 */

/*
 * ../cons-text.c
 */

void			cons_clear(void);

void			cons_scroll(t_uint16			lines);

void			cons_attr(t_uint8			attr);

int			cons_print_char(char			c);

void			cons_print_string(char*			string);

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...);

t_error			cons_init(void);

t_error			cons_clean(void);


/*
 * eop
 */

#endif
