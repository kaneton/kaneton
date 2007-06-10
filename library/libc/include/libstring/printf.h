/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libstring/printf.h
 *
 * created       julien quintard   [sun jun 10 17:36:06 2007]
 * updated       julien quintard   [sun jun 10 17:36:23 2007]
 */

#ifndef LIBC_LIBSTRING_PRINTF_H
#define LIBC_LIBSTRING_PRINTF_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PRINTF_LADJUST		0x1
#define PRINTF_ZPAD		0x2
#define PRINTF_DOT		0x4

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

/* XXX */
#define fprintf(_stream_, _args_...) printf(_args_)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef int			(*t_printf_char_fn)(char	c);
typedef void			(*t_printf_attr_fn)(u_int8_t	attr);

#endif
