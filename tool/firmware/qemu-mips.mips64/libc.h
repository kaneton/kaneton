/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...on/tool/firmware/qemu-mips.mips64/libc.h
 *
 * created       matthieu bucchianeri   [thu jun 28 14:47:59 2007]
 * updated       enguerrand raymond   [mon apr 27 12:25:30 2009]
 */

#ifndef FIRMWARE_LIBC_H
#define FIRMWARE_LIBC_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "cons.h"

/*
 * ---------- types -----------------------------------------------------------
 */

typedef char*			va_list;
typedef t_sint32		size_t;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define	NULL			((void*)0)
#define	PRINTF_LADJUST		0x1
#define PRINTF_ZPAD		0x2
#define PRINTF_DOT		0x4

#define CHAR_BITS		8

/*
 * ---------- macro functions -------------------------------------------------
 */

#define PRINTF_SWAP(_i1_, _i2_)						\
  {									\
    int		_tmp_;							\
									\
    _tmp_ = (_i1_);							\
    (_i1_) = (_i2_);							\
    (_i2_) = _tmp_;							\
  }

#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (long) - 1) / sizeof (long))		\
   * sizeof (long))

/* 
 * Alignment of an arg of given type (char,short promote to reg), - 1.
 */

#define __va_alignoff(TYPE)  						\
  (((__alignof(TYPE) <= __alignof(long)) 				\
    ?__alignof(long)							\
    : __alignof(TYPE)) - 1)

#define va_start(AP, LASTARG) 						\
 (AP = ((va_list) __builtin_next_arg(LASTARG)))

#define va_end(AP)	((void)0)

#define __va_copy(dest, src) (dest) = (src)

#define va_arg(AP, TYPE) 						\
 (AP = 									\
   (va_list) (((long)AP + __va_alignoff(TYPE)) & ~__va_alignoff(TYPE))  \
      + __va_rounded_size(TYPE), 					\
   *((TYPE *) (void *) ((char *)AP - __va_rounded_size(TYPE))))

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	firmware-libc.c
 */

/*
 * firmware-libc.c
 */

int			printf_string(char*			string,
				      unsigned int		flags,
				      int			len1,
				      int			len2);

int			printf_quad(t_sint64			value,
				    int				base,
				    int				hdl_sign,
				    unsigned int		flags,
				    int				len1,
				    int				len2);

int			vprintf(const char*			fmt,
				va_list				args);

int			printf(char*				fmt,
			       ...);

size_t			strlen(const char*			str);

void*			memcpy(void*				dest,
			       const void*			src,
			       size_t				n);

void*			memset(void*				s,
			       int				c,
			       size_t				n);

char*			strcpy(char*				to,
			       const char*			from);


/*
 * eop
 */

#endif
