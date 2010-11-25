/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/console/include/console.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [sat may  2 00:49:25 2009]
 */

#ifndef MODULES_CONSOLE_CONSOLE_H
#define MODULES_CONSOLE_CONSOLE_H	1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../console.c
 */

/*
 * ../console.c
 */

void			module_console_clear(void);

void			module_console_scroll(t_uint16		lines);

void			module_console_message(char		indicator,
					       char*		fmt,
					       ...);

t_error			module_console_initialize(void);

t_error			module_console_clean(void);


/*
 * eop
 */

#endif
