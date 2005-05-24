/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * stdarg.h
 * 
 * path          /mnt/goinfre/projects/kaneton/src/include/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:24:25 2005   mycure
 * last update   Fri Feb 11 02:24:30 2005   mycure
 */

#ifndef IA32_STDARG_H
#define IA32_STDARG_H

typedef char *			va_list;

#define __va_rouded_size(type)						\
  (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, lastarg)						\
  (ap = ((char *) &(lastarg) + __va_rouded_size(lastarg)))

extern void va_end	(va_list);
#define va_end(ap)	/* nothing to do */

#define va_arg(ap, type)						\
  (ap += __va_rouded_size(type),					\
   *((type *) (ap - __va_rouded_size(type))))

#endif
