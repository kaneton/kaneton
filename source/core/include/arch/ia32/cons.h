/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * cons.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:24:13 2005   mycure
 * last update   Sat May 28 22:21:51 2005   mycure
 */

#ifndef IA32_CONS_H
#define IA32_CONS_H		1

/*
 * defines
 */

#define CONS_ADDR		0xb8000
#define CONS_SIZE		0xfa0
#define CONS_LINES		25
#define CONS_COLUMNS		80
#define CONS_BPC		2
#define CONS_TAB		8
#define CONS_ATTR		0x0f

typedef struct			s_cons
{
  t_uint16			line;
  t_uint16			column;
  t_uint8			attr;
  char*				vga;
}				t_cons;

/*
 * details about console format:
 *
 * flicker   :   0 = disable    1 = enable
 * back      :   0 = black      1 = blue       2 = green      3 = cyan
 *               4 = red        5 = magenta    6 = yellow     7 = white
 * intensity :   0 = disable    1 = enable
 * front     :   0 = black      1 = blue       2 = green      3 = cyan
 *               4 = red        5 = magenta    6 = yellow     7 = white
 */

#endif
