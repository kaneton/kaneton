/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/libs/libmipsr10000/include/misc/stdarg.h
 *
 * created       enguerrand raymond   [wed oct 18 14:04:03 2006]
 * updated       enguerrand raymond   [sun oct 22 21:33:26 2006]
 */

#ifndef LIBMIPSR10000_STDARG_H
#define LIBMIPSR10000_STDARG_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef __builtin_va_list	va_list;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define va_start(v,l)		__builtin_va_start(v,l)
#define va_end(v)		__builtin_va_end(v)
#define va_arg(v,l)		__builtin_va_arg(v,l)

#endif
