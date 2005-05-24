/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * ctype.h
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/include/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:25 2005   mycure
 * last update   Sat Apr 23 16:41:46 2005   mycure
 */

#ifndef LIBC_CTYPE_H
#define LIBC_CTYPE_H		1

#define _U		0x01
#define _L		0x02
#define _N		0x04
#define _S		0x08
#define _P		0x10
#define _C		0x20
#define _X		0x40
#define _B		0x80

extern const u_char	_ctype[];
extern const short	_ctype_toupper[];
extern const short	_ctype_tolower[];

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
