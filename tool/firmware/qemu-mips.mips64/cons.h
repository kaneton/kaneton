/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...on/tool/firmware/qemu-mips.mips64/cons.h
 *
 * created       julien quintard   [wed jun  6 15:53:35 2007]
 * updated       enguerrand raymond   [wed may 13 08:42:28 2009]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

#ifndef FIRMWARE_CONS_H
#define FIRMWARE_CONS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "uart.h"
#include "libc.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	cons.c
 */

/*
 * cons.c
 */

void			firmware_cons_clear(void);

void			firmware_cons_scroll(t_uint16		lines);

void			firmware_cons_attr(t_sint8		attr);

int			firmware_cons_print_char(char		c);

void			firmware_cons_print_string(char*	string);

void			firmware_cons_msg(char		indicator,
					    char*		fmt,
					    ...);


/*
 * eop
 */

#endif
