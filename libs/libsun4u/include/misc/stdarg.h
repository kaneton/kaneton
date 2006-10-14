/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/misc/stdarg.h
 *
 * created       matthieu bucchianeri   [thu aug 10 15:16:06 2006]
 * updated       matthieu bucchianeri   [mon aug 28 18:27:04 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * Taken from Solaris Operating System.
 */

#ifndef LIBSUN4U_STDARG_H
#define LIBSUN4U_STDARG_H	1

/*
 * ---------- types -----------------------------------------------------------
 */
#if 0
typedef char *__va_list;
typedef __va_list va_list;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define VA_ALIGN        8

/*
 * ---------- macro functions -------------------------------------------------
 */

#define _ARGSIZEOF(t)							\
  ((sizeof (t) + VA_ALIGN - 1) & ~(VA_ALIGN - 1))


#define va_start(list, name)						\
  (list = (__va_list)&name + _ARGSIZEOF(name))

#define va_arg(list, t)							\
  (*(t *)((list += _ARGSIZEOF(t)) - _ARGSIZEOF(t)))

#define va_end(list)							\
  (list = (__va_list)0)

#endif

typedef __builtin_va_list va_list;

# define va_start(ap, last)   __builtin_va_start(ap, last)
# define va_end(ap)     __builtin_va_end(ap)
# define va_arg(ap, type)  __builtin_va_arg(ap, type)


#endif
