/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libsys/stdarg.h
 *
 * created       julien quintard   [sun jun 10 18:04:06 2007]
 * updated       julien quintard   [sun jun 10 18:05:43 2007]
 */

#ifndef LIBC_LIBSYS_STDARD_H
#define LIBC_LIBSYS_STDARD_H		1

/*
 * XXX
 *
 * these macro highly rely on the underlying microprocessor architecture.
 */

/*
 * ---------- types -----------------------------------------------------------
 */

typedef char*			va_list;

/*
 * ---------- extern ----------------------------------------------------------
 */

extern void va_end	(va_list);

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		__va_rouded_size(type)					\
  (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define		va_start(ap, lastarg)					\
  (ap = ((char*) &(lastarg) + __va_rouded_size(lastarg)))

#define		va_end(ap)	/* nothing to do */

#define		va_arg(ap, type)					\
  (ap += __va_rouded_size(type),					\
   *((type*) (ap - __va_rouded_size(type))))

#endif
