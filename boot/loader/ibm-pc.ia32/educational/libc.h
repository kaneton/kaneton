/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...ootloader/ibm-pc.ia32/educational/libc.h
 *
 * created       matthieu bucchianeri   [thu jun 28 14:47:59 2007]
 * updated       matthieu bucchianeri   [wed jul 18 18:41:56 2007]
 */

#ifndef BOOTLOADER_LIBC_H
#define BOOTLOADER_LIBC_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
/*
 * ---------- types -----------------------------------------------------------
 */

typedef char*			va_list;
typedef int			(*t_printf_char_fn)(char	c);
typedef void			(*t_printf_attr_fn)(t_sint8	attr);
typedef t_sint32		size_t;
union		uu
{
  t_sint64	q;			/* as a (signed) quad */
  t_uint64	uq;			/* as an unsigned quad */
  t_sint32	sl[2];			/* as two signed longs */
  t_uint32	ul[2];			/* as two unsigned longs */
};

typedef unsigned int		qshift_t;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define	NULL			((void*)0)
#define	PRINTF_LADJUST		0x1
#define PRINTF_ZPAD		0x2
#define PRINTF_DOT		0x4

#define QUAD_HIGHWORD		1
#define QUAD_LOWWORD		0

#define QUAD_H			QUAD_HIGHWORD
#define QUAD_L			QUAD_LOWWORD

#define QUAD_QUAD_BITS		(sizeof(quad_t) * CHAR_BITS)
#define QUAD_LONG_BITS		(sizeof(long) * CHAR_BITS)
#define QUAD_HALF_BITS		(sizeof(long) * CHAR_BITS / 2)

#define CHAR_BITS		8

/*
 * ---------- macro functions -------------------------------------------------
 */

#define QUAD_HHALF(x)							\
  ((t_uint32)(x) >> QUAD_HALF_BITS)

#define QUAD_LHALF(x)							\
  ((t_uint32)(x) & (((long)1 << QUAD_HALF_BITS) - 1))

#define QUAD_LHUP(x)							\
  ((t_uint32)(x) << QUAD_HALF_BITS)

#define PRINTF_SWAP(_i1_, _i2_)						\
  {									\
    int		_tmp_;							\
									\
    _tmp_ = (_i1_);							\
    (_i1_) = (_i2_);							\
    (_i2_) = _tmp_;							\
  }

#define		__va_rouded_size(type)					\
  (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define		va_start(ap, lastarg)					\
  (ap = ((char*) &(lastarg) + __va_rouded_size(lastarg)))

#define		va_end(ap)	/* nothing to do */

#define		va_arg(ap, type)					\
  (ap += __va_rouded_size(type),					\
   *((type*) (ap - __va_rouded_size(type))))

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

int			printf_init(t_printf_char_fn		pc,
				    t_printf_attr_fn		pa);

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

t_uint64		__umoddi3(t_uint64			a,
				  t_uint64			b);

t_uint64		__udivdi3(t_uint64			a,
				  t_uint64			b);

t_uint64		__qdivrem(t_uint64			uq,
				  t_uint64			vq,
				  t_uint64*			arq);


/*
 * eop
 */

#endif
