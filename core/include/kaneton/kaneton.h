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
 * last update   Tue Aug 23 23:08:02 2005   mycure
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
 * perms
 */

#define PERM_READ		0x1
#define PERM_WRITE		0x2
#define PERM_EXEC		0x4

/*
 * ---------- macros ----------------------------------------------------------
 */

#define kaneton_error(_fmt_...)						\
  {									\
    printf("%#[%#!%#]%# %s:%u: ",					\
           CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           CONS_FRONT(CONS_RED) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT,	\
           __FILE__,							\
           __LINE__);							\
    printf(_fmt_);							\
									\
    while (1)								\
      ;									\
  }

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton/as.h>
#include <kaneton/debug.h>
#include <kaneton/id.h>
#include <kaneton/init.h>
#include <kaneton/segment.h>
#include <kaneton/set.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/kaneton.c
 *      ../../kaneton/cons.c
 *      ../../kaneton/serial.c
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

int			cons_clean(void);


/*
 * ../../kaneton/serial.c
 */

int			serial_read(t_uint8*			data);

int			serial_write(t_uint8*			data);

int			serial_init(void);

int			serial_clean(void);


/*
 * eop
 */

#endif
