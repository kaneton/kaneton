/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libc/include/stdarg.h
 *
 * created       julien quintard   [sun jun 10 18:04:06 2007]
 * updated       matthieu bucchianeri   [sat sep  1 13:35:36 2007]
 */

#ifndef LIBC_LIBSYS_STDARD_H
#define LIBC_LIBSYS_STDARD_H		1

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

#define va_start(v,l)   __builtin_stdarg_start((v),l)
#define va_copy(d,s)    __builtin_va_copy((d),(s))

#endif
