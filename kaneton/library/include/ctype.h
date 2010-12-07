/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/library/include/ctype.h
 *
 * created       julien quintard   [sun jun 10 17:35:42 2007]
 * updated       julien quintard   [sun dec  5 15:55:58 2010]
 */

#ifndef LIBRARY_CTYPE_H
#define LIBRARY_CTYPE_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <library/types.h>

/*
 * ---------- macros ----------------------------------------------------------
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
 * ---------- macro functions -------------------------------------------------
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
