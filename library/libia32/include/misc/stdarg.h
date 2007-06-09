/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/libs/libia32/include/misc/stdarg.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [fri mar 10 04:13:22 2006]
 */

#ifndef IA32_IA32_STDARG_H
#define IA32_IA32_STDARG_H	1

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
