/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sun Jun 19 22:05:01 2005   mycure
 */

#ifndef KANETON_KANETON_H
#define KANETON_KANETON_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * endian
 */

#define ENDIAN_LITTLE		(1 << 0)
#define ENDIAN_BIG		(1 << 1)

/*
 * word size
 */

#define WORDSZ_32		(1 << 0)
#define WORDSZ_64		(1 << 1)
#define WORDSZ_128		(1 << 2)

/*
 * development mode
 */

#define MODE_DEVELOPMENT	(1 << 0)
#define MODE_RELEASE		(1 << 1)

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton/as.h>
#include <kaneton/debug.h>
#include <kaneton/init.h>
#include <kaneton/segment.h>
#include <kaneton/set.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/kaneton.c
 *      ../../kaneton/cons.c
 */

/*
 * ../../kaneton/kaneton.c
 */

void			kaneton_dump(void);


void			kaneton(t_init*				bootloader);


/*
 * ../../kaneton/cons.c
 */

void			cons_clear(void);


void			cons_scroll(t_uint16			lines);


void			cons_attr(t_uint8			attr);


int			cons_print_char(char			c);


void			cons_print_string(char*			string);


void			cons_msg(char				indicator,
				 char*				fmt,
				 ...);


int			cons_init(void);

/*
 * eop
 */

#endif
