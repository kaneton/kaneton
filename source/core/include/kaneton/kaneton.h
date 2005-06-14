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
 * last update   Tue Jun 14 15:13:56 2005   mycure
 */

#ifndef KANETON_KANETON_H
#define KANETON_KANETON_H	1

/*
 * debug
 */

#define KANETON_DEBUG_PARAMS	0x1

#define KANETON_DEBUG							\
  (0 | KANETON_DEBUG_PARAMS)

/*
 * includes
 */

#include <kaneton/bootloader.h>
#include <kaneton/init.h>

#include <kaneton/errno.h>
#include <kaneton/suberrno.h>

#include <kaneton/segment.h>

/*
 * prototypes:     ../../kaneton/kaneton.c
 *                 ../../kaneton/cons.c
 */

/*
 * ../../kaneton/kaneton.c
 */

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
