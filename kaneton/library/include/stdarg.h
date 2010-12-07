/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/library/include/stdarg.h
 *
 * created       julien quintard   [sun jun 10 18:04:06 2007]
 * updated       julien quintard   [sun dec  5 15:59:11 2010]
 */

#ifndef LIBRARY_STDARG_H
#define LIBRARY_STDARG_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef __builtin_va_list va_list;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define va_end          __builtin_va_end
#define va_arg          __builtin_va_arg

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define va_start(v,l)   __builtin_va_start((v),l)
#define va_copy(d,s)    __builtin_va_copy((d),(s))

#endif
