/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/include/format.h
 *
 * created       julien quintard   [sun dec  5 15:04:58 2010]
 * updated       julien quintard   [mon dec 13 14:39:10 2010]
 */

#ifndef LIBRARY_FORMAT_H
#define LIBRARY_FORMAT_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_format_character_fn)(char		c);
typedef void			(*t_format_attribute_fn)(u_int8_t	attr);

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define FORMAT_VALUE(_sign_)						\
  if (quadflag)								\
    quadvalue = va_arg(args, quad_t);					\
  else									\
    {									\
      if ((_sign_))							\
	{								\
	  if (longflag)							\
	    quadvalue = (u_quad_t) va_arg(args, long);			\
	  else								\
	    quadvalue = (u_quad_t) va_arg(args, int);			\
	}								\
      else								\
	if (longflag)							\
	  quadvalue = (u_quad_t) va_arg(args, unsigned long);		\
	else								\
	  quadvalue = (u_quad_t) va_arg(args, unsigned int);		\
    }

#define FORMAT_SWAP(_i1_, _i2_)						\
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

#define FORMAT_LADJUST		0x1
#define FORMAT_ZPAD		0x2
#define FORMAT_DOT		0x4

#endif
