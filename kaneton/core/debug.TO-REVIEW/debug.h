/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/debug.TO-REVIEW/debug.h
 *
 * created       julien quintard   [wed jun  6 13:11:05 2007]
 * updated       julien quintard   [sat may  2 00:15:28 2009]
 */

/*
 * XXX
 *
 * this file should be reworkd to keep the DEBUG_* macros. however the
 * serial stuff should be moved in a test manager dedicated to the
 * communication with the kaneton test suite.
 */

#ifndef CORE_DEBUG_H
#define CORE_DEBUG_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define DEBUG_PARAMS		(1 << 0)
#define DEBUG_AS		(1 << 1)
#define DEBUG_SEGMENT		(1 << 2)
#define DEBUG_SET		(1 << 3)
#define DEBUG_REGION		(1 << 4)
#define DEBUG_TASK		(1 << 5)
#define DEBUG_THREAD		(1 << 6)
#define DEBUG_SCHEDULE		(1 << 7)
#define DEBUG_MESSAGE		(1 << 8)
#define DEBUG_CAPABILITY	(1 << 9)
#define DEBUG_INTERFACE		(1 << 10)
#define DEBUG_EVENT		(1 << 11)
#define DEBUG_TIME		(1 << 12)
#define DEBUG_CPU		(1 << 13)
#define DEBUG_IO		(1 << 14)
#define DEBUG_MAP		(1 << 15)

#define DEBUG								\
  (0)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/debug/debug.c
 *      ../../core/debug/gdb.c
 *      ../../core/debug/shell.c
 *      ../../core/debug/callbacks.c
 */

/*
 * ../../core/debug/debug.c
 */

t_error			debug_initialize(void);

t_error			debug_clean(void);


/*
 * ../../core/debug/gdb.c
 */

void			gdb_handler(t_uint32 needless);

t_error		gdb_command(void);

t_uint32		gdb_extract(t_uint8*	buffer,
				    t_uint32	sz);

int			gdb_read_reg(t_uint8*		buffer);

int		gdb_write_reg(t_uint8*		buffer);

int		gdb_read_mem(t_uint8*		buffer);

int		gdb_write_mem(t_uint8*		buffer);

int			gdb_step(t_uint8*		buffer);

int		gdb_continue(t_uint8*		buffer);

int		gdb_unset_break(t_uint8*	buffer);

int		gdb_set_break(t_uint8*		buffer);

int			gdb_status(t_uint8*		buffer);

t_error		gdb_init(void);

t_error		gdb_clean(void);


/*
 * ../../core/debug/shell.c
 */

void debug_shell(char   (*i_read)(void));

void debug_shell_echo(const char *arg);

void debug_shell_call(const char *arg);

void debug_shell_config_set(const char *arg, int val);

int debug_shell_config_isset(const char *arg);

void debug_shell_set(const char *arg);

void debug_shell_unset(const char *arg);


/*
 * ../../core/debug/callbacks.c
 */

void    debug_callbacks_register(const char *name, void (*fn)(void));

void    debug_callbacks_call(const char *name);


/*
 * eop
 */

#endif
