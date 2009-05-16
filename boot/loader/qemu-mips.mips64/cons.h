/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/kaneton/boot/loader/qemu-mips.mips64/cons.h
 *
 * created       julien quintard   [wed jun  6 15:53:35 2007]
 * updated       enguerrand raymond   [wed may 13 08:38:58 2009]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

#ifndef BOOTLOADER_CONS_H
#define BOOTLOADER_CONS_H	1

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

void			bootloader_cons_clear(void);

void			bootloader_cons_scroll(t_uint16		lines);

void			bootloader_cons_attr(t_sint8		attr);

int			bootloader_cons_print_char(char		c);

void			bootloader_cons_print_string(char*	string);

void			bootloader_cons_msg(char		indicator,
					    char*		fmt,
					    ...);

int			bootloader_cons_init(void);


/*
 * eop
 */

#endif
