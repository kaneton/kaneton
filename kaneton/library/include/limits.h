/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/library/include/limits.h
 *
 * created       julien quintard   [sun jun 10 17:53:28 2007]
 * updated       julien quintard   [sun dec  5 15:58:53 2010]
 */

#ifndef LIBRARY_LIMITS_H
#define LIBRARY_LIMITS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <library/types.h>

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * integer types MAX and MIN values. thanks to alexandre becoulet.
 */

/*
 * return max integer value for a type
 */

#define __MINOF_TYPE(t)        ((__typeof__(t))(((__typeof__(t))-1) < 0 ?  (((__typeof__(t))1) << (sizeof (__typeof__(t)) * 8 - 1)) :  0))

/*
 * return min integer value for a type
 */

#define __MAXOF_TYPE(t)        ((__typeof__(t))(((__typeof__(t))-1) < 0 ? ~(((__typeof__(t))1) << (sizeof (__typeof__(t)) * 8 - 1)) : -1))

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * char type
 */

#define CHAR_BITS	(sizeof (char) * 8)

#define SCHAR_MIN	(__MINOF_TYPE(signed char))
#define SCHAR_MAX	(__MAXOF_TYPE(signed char))
#define UCHAR_MAX	(__MAXOF_TYPE(unsigned char))
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX

/*
 * short type
 */

#define SHORT_BITS	(sizeof (short) * 8)
#define SHRT_MIN	(__MINOF_TYPE(signed short))
#define SHRT_MAX	(__MAXOF_TYPE(signed short))
#define USHRT_MAX	(__MAXOF_TYPE(unsigned short))

/*
 * int type
 */

#define INT_BITS	(sizeof (int) * 8)
#define INT_MIN		(__MINOF_TYPE(signed int))
#define INT_MAX		(__MAXOF_TYPE(signed int))
#define UINT_MAX	(__MAXOF_TYPE(unsigned int))

/*
 * long type
 */

#define LONG_BITS	(sizeof (long) * 8)
#define SLONG_MIN	(__MINOF_TYPE(signed long))
#define SLONG_MAX	(__MAXOF_TYPE(signed long))
#define ULONG_MAX	(__MAXOF_TYPE(unsigned long))

/*
 * long long type
 */

#define LLONG_BITS	(sizeof (long long) * 8)
#define LLONG_MIN	(__MINOF_TYPE(signed long long))
#define LLONG_MAX	(__MAXOF_TYPE(signed long long))
#define ULLONG_MAX	(__MAXOF_TYPE(unsigned long long))

/*
 * quad type
 */

#define QUAD_BITS	LLONG_BITS
#define SQUAD_MIN	LLONG_MIN
#define SQUAD_MAX	LLONG_MAX
#define UQUAD_MAX	ULLONG_MAX

#endif
