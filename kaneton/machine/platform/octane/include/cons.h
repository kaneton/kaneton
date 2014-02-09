/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kanet...n/machine/platform/octane/include/cons.h
 *
 * created       laurent lec   [fri mar 28 17:11:15 2008]
 * updated       laurent lec   [fri mar 28 17:17:29 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 */

#ifndef PLATFORM_CONS_H
#define PLATFORM_CONS_H		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../cons.c
 */

/*
 * ../cons.c
 */

void			cons_clear(void);

void			cons_scroll(t_uint16			lines);

void			cons_attr(t_uint8			attr);

int			cons_print_char(char			c);

void			cons_print_string(char*			string);

void			module_call(console, console_message, char				indicator,
				 char*				fmt,
				 ...);

t_status		cons_init(void);

t_status		cons_clean(void);


/*
 * eop
 */

#endif
