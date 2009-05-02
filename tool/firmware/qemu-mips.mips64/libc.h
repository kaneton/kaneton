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
 * updated       enguerrand raymond   [sat may  2 12:32:32 2009]
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

typedef __builtin_va_list	va_list;
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

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	libc.c
 */

/*
 * libc.c
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
