/*
 * copyright quintard julien
 *
 * kaneton
 *
 * init.h
 *
 * path          /home/vertrex/kaneton/libs/klibc/libdata
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Mon Nov 28 16:32:00 2005   
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

#define DEBUG								\
  (CONF_DEBUG)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef	struct		s_debug_function
{
	char		*f_name;
	int		(*f)(char *str);
}			debug_function_t;



/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/debug/debug.c
 */

/*
 * ../../kaneton/debug/debug.c
 */

t_error			debug_init(void);

t_error			debug_recv();

t_error			debug_recv_cmd();

t_error			debug_clean(void);


/*
 * eop
 */

#endif
