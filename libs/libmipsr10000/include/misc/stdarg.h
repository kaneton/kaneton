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
 * updated       enguerrand raymond   [sat oct 21 02:01:13 2006]
 */

#ifndef LIBMIPSR10000_STDARG_H
#define LIBMIPSR10000_STDARG_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef char*	va_list;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ALIGN	8

/*
 * ---------- macro functions -------------------------------------------------
 */

#define va_start(_argpt_, _lastarg_)				\
(_argpt_ = ((char*)&_lastarg_ + ALIGN))

#define va_arg(_argpt_, _type_)					\
((_argpt_ += ALIGN),*((_type_*)(_argpt_ - ALIGN)))

#define va_end(_argpt_)

#endif
