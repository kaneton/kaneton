/*
 * copyright quintard julien
 *
 * kaneton
 *
 * kaneton.h
 *
 * path          /home/mycure/kaneton/core/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 02:19:44 2005   mycure
** Last update Tue Sep  6 00:16:19 2005 Reboot Universe
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
 * perms
 */

#define PERM_READ		(1 << 0)
#define PERM_WRITE		(1 << 1)
#define PERM_EXEC		(1 << 2)

/*
 * fits
 */

#define ALLOC_FIRST_FIT         (1 << 0)

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

#include <kaneton/conf.h>

#include <kaneton/as.h>
#include <kaneton/debug.h>
#include <kaneton/error.h>

#include <kaneton/types.h>

#include <kaneton/id.h>
#include <kaneton/init.h>
#include <kaneton/segment.h>
#include <kaneton/set.h>
#include <kaneton/stats.h>

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

void			kaneton(t_init*				bootloader);

void			kaneton_dump(void);


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

t_error			cons_init(void);

t_error			cons_clean(void);


/*
 * ../../kaneton/serial.c
 */

int			serial_read(t_uint8*			data, int size, int* n);

int			serial_write(t_uint8*			data,	int		size, int* n);

int			serial_init(void);

int			serial_clean(void);


/*
 * eop
 */

#endif
