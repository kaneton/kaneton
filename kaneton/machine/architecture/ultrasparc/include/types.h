/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/misc/types.h
 *
 * created       matthieu bucchianeri   [thu aug 10 15:13:39 2006]
 * updated       matthieu bucchianeri   [mon aug 28 15:20:25 2006]
 */

#ifndef LIBSUN4U_TYPES_H
#define LIBSUN4U_TYPES_H	1

/*
 * base types
 */

typedef unsigned char		t_uint8;
typedef signed char		t_sint8;
typedef unsigned short		t_uint16;
typedef signed short		t_sint16;
typedef unsigned int		t_uint32;
typedef signed int		t_sint32;
typedef unsigned long		t_uint64;
typedef signed long		t_sint64;

/*
 * specific.
 */

typedef t_uint64		t_sparc64_context;

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/types.h>

#endif
