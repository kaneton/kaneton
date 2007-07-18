/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...ton/library/libc/include/libsys/endian.h
 *
 * created       matthieu bucchianeri   [wed jul 18 17:17:33 2007]
 * updated       matthieu bucchianeri   [wed jul 18 17:19:19 2007]
 */

#ifndef LIBC_LIBSYS_ENDIAN_H
#define LIBC_LIBSYS_ENDIAN_H

#if (___kaneton$endian == 0)                   /* little endian */

# ifndef _QUAD_HIGHWORD
#  define _QUAD_HIGHWORD 1
# endif
# ifndef _QUAD_LOWWORD
#  define _QUAD_LOWWORD 0
# endif

#elif (___kaneton$endian == 1)                 /* big endian */

# ifndef _QUAD_HIGHWORD
#  define _QUAD_HIGHWORD 0
# endif
# ifndef _QUAD_LOWWORD
#  define _QUAD_LOWWORD 1
# endif

#endif

#endif
