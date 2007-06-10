/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libsys/quad.h
 *
 * created       julien quintard   [sun jun 10 17:56:26 2007]
 * updated       julien quintard   [sun jun 10 17:56:37 2007]
 */

#ifndef LIBC_LIBSYS_QUAD_H
#define LIBC_LIBSYS_QUAD_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * depending on the desired operation, we view a `long long' (aka quad_t) in
 * one or more of the following formats.
 */

union		uu
{
  quad_t	q;			/* as a (signed) quad */
  u_quad_t	uq;			/* as an unsigned quad */
  long		sl[2];			/* as two signed longs */
  u_long	ul[2];			/* as two unsigned longs */
};

typedef unsigned int		qshift_t;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * define high and low longwords.
 */

#if (___endian == ENDIAN_LITTLE)
#define QUAD_HIGHWORD		1
#define QUAD_LOWWORD		0
#elif (___endian == ENDIAN_BIG)
#define QUAD_HIGHWORD		0
#define QUAD_LOWWORD		1
#endif

#define QUAD_H			QUAD_HIGHWORD
#define QUAD_L			QUAD_LOWWORD

/*
 * total number of bits in a quad_t and in the pieces that make it up.
 * these are used for shifting, and also below for halfword extraction
 * and assembly.
 */

#define QUAD_QUAD_BITS		(sizeof(quad_t) * CHAR_BITS)
#define QUAD_LONG_BITS		(sizeof(long) * CHAR_BITS)
#define QUAD_HALF_BITS		(sizeof(long) * CHAR_BITS / 2)

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * extract high and low shortwords from longword, and move low shortword of
 * longword to upper half of long, i.e., produce the upper longword of
 * ((quad_t)(x) << (number_of_bits_in_long/2)).  (`x' must actually be u_long.)
 *
 * these are used in the multiply code, to split a longword into upper
 * and lower halves, and to reassemble a product as a quad_t, shifted left
 * (sizeof(long)*CHAR_BITS/2).
 */

#define QUAD_HHALF(x)							\
  ((u_long)(x) >> QUAD_HALF_BITS)

#define QUAD_LHALF(x)							\
  ((u_long)(x) & (((long)1 << QUAD_HALF_BITS) - 1))

#define QUAD_LHUP(x)							\
  ((u_long)(x) << QUAD_HALF_BITS)

#endif
