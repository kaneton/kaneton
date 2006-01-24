/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libsys/limits.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:53:04 2006]
 */

#ifndef LIBSYS_LIMITS_H
#define LIBSYS_LIMITS_H		1

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * char size
 */

#define CHAR_BITS		8
#define SCHAR_MIN		(-0x7f - 1)
#define SCHAR_MAX		0x7f
#define UCHAR_MIN		0x00U
#define UCHAR_MAX		0xffU

/*
 * short size
 */

#define SHORT_BITS		16
#define SSHORT_MIN		(-0x7fff - 1)
#define SSHORT_MAX		0x7fff
#define USHORT_MIN		0x0000U
#define USHORT_MAX		0xffffU

/*
 * int size
 */

#define INT_BITS		32
#define SINT_MIN		(-0x7fffffff - 1)
#define SINT_MAX		0x7fffffff
#define UINT_MIN		0x00000000U
#define UINT_MAX		0xffffffffU

/*
 * long size
 */

#define LONG_BITS		32
#define SLONG_MIN		(-0x7fffffff - 1)
#define SLONG_MAX		0x7fffffff
#define ULONG_MIN		0x00000000U
#define ULONG_MAX		0xffffffffU

/*
 * long long size
 */

#define LLONG_BITS		64
#define SLLONG_MIN		(-0x7fffffffffffffffLL - 1)
#define SLLONG_MAX		0x7fffffffffffffffLL
#define ULLONG_MIN		0x0000000000000000ULL
#define ULLONG_MAX		0xffffffffffffffffULL

/*
 * quad size
 */

#define QUAD_BITS		64
#define SQUAD_MIN		(-0x7fffffffffffffffLL - 1)
#define SQUAD_MAX		0x7fffffffffffffffLL
#define UQUAD_MIN		0x0000000000000000ULL
#define UQUAD_MAX		0xffffffffffffffffULL

#endif
