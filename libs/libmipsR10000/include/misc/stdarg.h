/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/libs/liboctane/include/misc/stdarg.h
 *
 * created       Enguerrand RAYMOND   [mon oct 16 21:12:36 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 13:15:06 2006]
 */

#ifndef STDARG_H
#define STDARG_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef char*	va_list;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ALIGN	4

/*
 * ---------- macro functions -------------------------------------------------
 */

#define va_start(_argpt_, _lastarg_)				\
(_argpt_ = ((char*)&_lastarg_ + ALIGN))

#define va_arg(_argpt_, _type_)					\
((_argpt_ += ALIGN),*((_type_*)(_argpt_ - ALIGN)))

#define va_end(_argpt_)

#endif
