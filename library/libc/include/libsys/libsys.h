/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libsys/libsys.h
 *
 * created       julien quintard   [sun jun 10 17:36:54 2007]
 * updated       julien quintard   [sun jun 10 18:05:58 2007]
 */

#ifndef LIBC_LIBSYS_LIBSYS_H
#define LIBC_LIBSYS_LIBSYS_H		1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libsys/types.h>
#include <libsys/errno.h>
#include <libsys/limits.h>
#include <libsys/stdarg.h>
#include <libsys/suberrno.h>
#include <libsys/quad.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../libsys/perror.c
 *      ../../libsys/quad.c
 */

/*
 * ../../libsys/perror.c
 */


/*
 * ../../libsys/quad.c
 */

quad_t			__adddi3(quad_t				a,
				 quad_t				b);

quad_t			__anddi3(quad_t				a,
				 quad_t				b);

quad_t			__ashldi3(quad_t			a,
				  qshift_t			shift);

quad_t			__ashrdi3(quad_t			a,
				  qshift_t			shift);

int			__cmpdi2(quad_t				a,
				 quad_t				b);

quad_t			__divdi3(quad_t				a,
				 quad_t				b);

quad_t			__iordi3(quad_t				a,
				 quad_t				b);

quad_t			__lshldi3(quad_t			a,
				  qshift_t			shift);

quad_t			__lshrdi3(quad_t			a,
				  qshift_t			shift);

quad_t			__moddi3(quad_t				a,
				 quad_t				b);

quad_t			__muldi3(quad_t				a,
				 quad_t				b);

quad_t			__negdi2(quad_t				a);

quad_t			__one_cmpldi2(quad_t			a);

u_quad_t		__qdivrem(u_quad_t			uq,
				  u_quad_t			vq,
				  u_quad_t*			arq);

quad_t			__subdi3(quad_t				a,
				 quad_t				b);

int			__ucmpdi2(quad_t			a,
				  quad_t			b);

u_quad_t		__udivdi3(u_quad_t			a,
				  u_quad_t			b);

u_quad_t		__umoddi3(u_quad_t			a,
				  u_quad_t			b);

quad_t			__xordi3(quad_t				a,
				 quad_t				b);


/*
 * eop
 */

#endif
