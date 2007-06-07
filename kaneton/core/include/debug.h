/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/debug.h
 *
 * created       julien quintard   [wed jun  6 13:11:05 2007]
 * updated       julien quintard   [thu jun  7 12:40:37 2007]
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
#define DEBUG_STATS		(1 << 4)
#define DEBUG_REGION		(1 << 5)
#define DEBUG_TASK		(1 << 6)
#define DEBUG_THREAD		(1 << 7)
#define DEBUG_SCHEDULE		(1 << 8)
#define DEBUG_MESSAGE		(1 << 9)
#define DEBUG_CAPABILITY	(1 << 10)
#define DEBUG_INTERFACE		(1 << 11)
#define DEBUG_EVENT		(1 << 12)
#define DEBUG_TIME		(1 << 13)
#define DEBUG_CPU		(1 << 14)
#define DEBUG_IO		(1 << 15)
#define DEBUG_MAP		(1 << 16)

#define DEBUG								\
  (CONF_DEBUG)

/*
 * ---------- macro functions -------------------------------------------------
 */

/*                                                                [cut] k1   */

#define ASSERT(_test_)							\
  if (!(_test_))							\
    {									\
      printf("assert(%s) failed at %s:%d\n",				\
             #_test_, __FILE__, __LINE__);				\
									\
      while (1)								\
	;								\
    }

/*                                                                [cut] /k1  */

/*
 * ---------- types -----------------------------------------------------------
 */

/*                                                                [cut] k1   */

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

/*                                                               [cut] /k1   */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/debug/debug.c
 *      ../../core/debug/cons-text.c
 *      ../../core/debug/serial.c
 *      ../../core/debug/gdb.c
 */

/*
 * ../../core/debug/debug.c
 */

t_error			debug_init(void);

t_error			debug_clean(void);


/*
 * ../../core/debug/cons-text.c
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
 * ../../core/debug/serial.c
 */

void			serial_read(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32 			size);

void			serial_write(t_uint32			com_port,
				     t_uint8*			data,
				     t_uint32			size);

int			serial_send(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32			size);

int			serial_recv(t_uint32			com_port,
				    t_serial_data		*rdata);

int	serial_put(char c);

void			serial_init(t_uint32			com_port,
				    t_uint8			baud_rate,
				    t_uint8			bit_type,
				    t_uint8			fifo_type);


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

#endif
