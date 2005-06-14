/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * libsys.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/libsys
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:36:32 2005   mycure
 * last update   Mon Jun 13 17:41:43 2005   mycure
 */

#ifndef LIBSYS_LIBSYS_H
#define LIBSYS_LIBSYS_H		1

/*
 * includes
 */

#include "types.h"

#include "errno.h"
#include "limits.h"
#include "services.h"
#include "suberrno.h"
#include "quad.h"

/*
 * prototypes:     ../../libsys/perror.c
 *                 ../../libsys/quad.c
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
