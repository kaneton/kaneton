/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/kaneton/core/include/debug.h
 *
 * created       julien quintard   [wed jun  6 13:11:05 2007]
 * updated       matthieu bucchianeri   [wed jan  9 20:09:55 2008]
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
 * ---------- macro functions -------------------------------------------------
 */

#define ASSERT(_test_)							\
  if (!(_test_))							\
    {									\
      printf("assert(%s) failed at %s:%d\n",				\
             #_test_, __FILE__, __LINE__);				\
									\
      while (1)								\
	;								\
    }

/* [cut] */
/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint32			size;
  t_uint32			magic;
  t_uint32			crc;
  t_uint8*			data;
}				t_serial_data;

typedef struct
{
 char*				name;
 void*				data;
}				t_serial_buffer;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/debug/debug.c
 *      ../../core/debug/gdb.c
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
 * eop
 */
/* [/cut] */

#endif
