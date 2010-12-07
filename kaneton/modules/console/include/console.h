/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/modules/console/include/console.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [sun dec  5 16:00:28 2010]
 */

#ifndef MODULES_CONSOLE_CONSOLE_H
#define MODULES_CONSOLE_CONSOLE_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the print function pointer.
 */

typedef void			(*f_module_console_character)(char);
typedef void			(*f_module_console_attribute)(t_uint8);

/*
 * the module structure
 */

typedef struct
{
  f_module_console_character	character;
  f_module_console_attribute	attribute;
}				m_module_console;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * since the console module relies on the platform when it comes to the
 * default behaviour, the following macros map the platform ones.
 */

#define MODULE_CONSOLE_FLICKER		PLATFORM_CONSOLE_FLICKER
#define MODULE_CONSOLE_BRIGHT		PLATFORM_CONSOLE_BRIGHT

#define MODULE_CONSOLE_BLACK		PLATFORM_CONSOLE_BLACK
#define MODULE_CONSOLE_BLUE		PLATFORM_CONSOLE_BLUE
#define MODULE_CONSOLE_GREEN		PLATFORM_CONSOLE_GREEN
#define MODULE_CONSOLE_CYAN		PLATFORM_CONSOLE_CYAN
#define MODULE_CONSOLE_RED		PLATFORM_CONSOLE_RED
#define MODULE_CONSOLE_MAGENTA		PLATFORM_CONSOLE_MAGENTA
#define MODULE_CONSOLE_YELLOW		PLATFORM_CONSOLE_YELLOW
#define MODULE_CONSOLE_WHITE		PLATFORM_CONSOLE_WHITE

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * since the console module relies on the platform when it comes to the
 * default behaviour, the following macro-functions map the platform ones.
 */

#define MODULE_CONSOLE_FRONT(_color_)	PLATFORM_CONSOLE_FRONT(_color_)
#define MODULE_CONSOLE_BACK(_color_)	PLATFORM_CONSOLE_BACK(_color_)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../console.c
 */

/*
 * ../console.c
 */

void			module_console_character(char			c);

void			module_console_attribute(t_uint8	attribute);

void			module_console_message(char		indicator,
					       char*		fmt,
					       ...);

void			module_console_print(char*		fmt,
					     ...);

t_error			module_console_set(f_module_console_character	character,
					   f_module_console_attribute	attribute);

t_error			module_console_get(f_module_console_character*	character,
					   f_module_console_attribute*	attribute);

t_error			module_console_load(void);

t_error			module_console_unload(void);


/*
 * eop
 */

#endif
