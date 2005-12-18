/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/debug.h
 *
 * created       julien quintard   [mon nov 28 19:37:01 2005]
 * updated       julien quintard   [sun dec 18 18:57:15 2005]
 */

#ifndef KANETON_DEBUG_H
#define KANETON_DEBUG_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
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

#define DEBUG								\
  (CONF_DEBUG)

/*
 * ---------- types -----------------------------------------------------------
 */


typedef struct			s_serial_data
{
  t_uint32			size;
  t_uint32			magic;
  t_uint32			crc;
  t_uint8*			data;
}				t_serial_data;

typedef	struct			s_serial_buffer
{
 char*				name;
 void*				data;
}				t_serial_buffer;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/debug/debug.c
 *      ../../kaneton/debug/cons.c
 */

/*
 * ../../kaneton/debug/debug.c
 */

t_error			debug_init(void);

t_error			debug_recv();

t_error			load_data();

char*		get_data(char *name);

t_error			debug_recv_cmd();

t_error			debug_exec_cmd_tab(t_serial_data *cmd);

t_error			debug_clean(void);


/*
 * ../../kaneton/debug/cons.c
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
 * eop
 */

#endif
