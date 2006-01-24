/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libstring/ctype.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:53:34 2006]
 */

#ifndef LIBSTRING_CTYPE_H
#define LIBSTRING_CTYPE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define _U			0x01
#define _L			0x02
#define _N			0x04
#define _S			0x08
#define _P			0x10
#define _C			0x20
#define _X			0x40
#define _B			0x80

/*
 * ---------- extern ----------------------------------------------------------
 */

extern const u_char	_ctype[];
extern const short	_ctype_toupper[];
extern const short	_ctype_tolower[];

/*
 * ---------- macros ----------------------------------------------------------
 */

#define isdigit(c)	((int) ((_ctype + 1)[(int) (c)] & _N))
#define islower(c)	((int) ((_ctype + 1)[(int) (c)] & _L))
#define isspace(c)	((int) ((_ctype + 1)[(int) (c)] & _S))
#define ispunct(c)	((int) ((_ctype + 1)[(int) (c)] & _P))
#define isupper(c)	((int) ((_ctype + 1)[(int) (c)] & _U))
#define isalpha(c)	((int) ((_ctype + 1)[(int) (c)] & (_U | _L)))
#define isxdigit(c)	((int) ((_ctype + 1)[(int) (c)] & (_N | _X)))
#define isalnum(c)	((int) ((_ctype + 1)[(int) (c)] & (_U | _L | _N)))
#define isprint(c)	((int) ((_ctype + 1)[(int) (c)] &		\
				(_P | _U | _L | _N | _B)))
#define isgraph(c)	((int) ((_ctype + 1)[(int) (c)] & (_P | _U | _L | _N)))
#define iscntrl(c)	((int) ((_ctype + 1)[(int) (c)] & _C))
#define tolower(c)	((int) ((_ctype_tolower + 1)[(int) (c)]))
#define toupper(c)	((int) ((_ctype_toupper + 1)[(int) (c)]))

#define isascii(c)	((unsigned) (c) <= 0177)
#define toascii(c)	((c) & 0177)

#endif
