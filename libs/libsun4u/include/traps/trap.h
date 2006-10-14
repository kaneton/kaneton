/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/traps/trap.h
 *
 * created       matthieu bucchianeri   [mon aug 21 20:09:05 2006]
 * updated       matthieu bucchianeri   [mon aug 21 20:09:54 2006]
 */

#ifndef LIBSUN4U_TRAP_H
#define LIBSUN4U_TRAP_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * a trap handler function.
 */

typedef void	(*t_sparc64_handler)(t_uint64);

#endif
