/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Fri Jun 17 16:07:26 2005   mycure
 */

#ifndef KANETON_KANETON_H
#define KANETON_KANETON_H	1

/*
 * development mode
 */

#define MODE_DEVELOPMENT	(1 << 0)
#define MODE_RELEASE		(1 << 1)

/*
 * debug
 */

#define KANETON_DEBUG_PARAMS	(1 << 0)
#define KANETON_DEBUG_AS	(1 << 1)
#define KANETON_DEBUG_SEGMENT	(1 << 2)

#define KANETON_DEBUG							\
  (0 | KANETON_DEBUG_PARAMS)

/*
 * includes
 */

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
