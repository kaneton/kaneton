/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...achine/platform/qemu-mips/include/cons.h
 *
 * created       enguerrand raymond   [sun jan 25 14:29:58 2009]
 * updated       enguerrand raymond   [sun jan 25 14:36:27 2009]
 */

#ifndef PLATFORM_CONS_H
#define PLATFORM_CONS_H		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../cons-text.c
 */

/*
 * ../cons-text.c
 */

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...);

t_error			cons_init(void);


/*
 * eop
 */

#endif
