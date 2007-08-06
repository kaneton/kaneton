/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...brary/libc/include/libstring/libstring.h
 *
 * created       julien quintard   [sun jun 10 17:35:02 2007]
 * updated       matthieu bucchianeri   [mon aug  6 21:55:08 2007]
 */

#ifndef LIBC_LIBSTRING_LIBSTRING_H
#define LIBC_LIBSTRING_LIBSTRING_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>
#include <libsys/stdarg.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libstring/ctype.h>
#include <libstring/printf.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../libstring/ctype.c
 *      ../../libstring/bcmp.c
 *      ../../libstring/bcopy.c
 *      ../../libstring/bzero.c
 *      ../../libstring/index.c
 *      ../../libstring/memcmp.c
 *      ../../libstring/memcpy.c
 *      ../../libstring/memset.c
 *      ../../libstring/printf.c
 *      ../../libstring/rindex.c
 *      ../../libstring/strcat.c
 *      ../../libstring/strchr.c
 *      ../../libstring/strcmp.c
 *      ../../libstring/strcpy.c
 *      ../../libstring/strdup.c
 *      ../../libstring/strlen.c
 *      ../../libstring/strncat.c
 *      ../../libstring/strncmp.c
 *      ../../libstring/strncpy.c
 *	../../libstring/strrchr.c
 *      ../../libstring/strtol.c
 *      ../../libstring/strtoul.c
 */

/*
 * ../../libstring/ctype.c
 */


/*
 * ../../libstring/bcmp.c
 */

int			bcmp(const void*			b1,
			     const void*			b2,
			     size_t				length);


/*
 * ../../libstring/bcopy.c
 */

void			bcopy(const void*			src,
			      void*				dst,
			      size_t				n);


/*
 * ../../libstring/bzero.c
 */

void			bzero(void*				dst,
			      size_t				length);


/*
 * ../../libstring/index.c
 */

char*			index(const char*			p,
			      int				ch);


/*
 * ../../libstring/memcmp.c
 */

int			memcmp(const void*			s1,
			       const void*			s2,
			       size_t				n);


/*
 * ../../libstring/memcpy.c
 */

void*			memcpy(void*				dest,
			       const void*			src,
			       size_t				n);


/*
 * ../../libstring/memset.c
 */

void*			memset(void*				s,
			       int				c,
			       size_t				n);


/*
 * ../../libstring/printf.c
 */

int			printf_string(char*			string,
				      unsigned int		flags,
				      int			len1,
				      int			len2);

int			printf_quad(quad_t			value,
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


/*
 * ../../libstring/rindex.c
 */

char*			rindex(const char*			p,
			       int				ch);


/*
 * ../../libstring/strcat.c
 */

char*			strcat(char*				s,
			       const char*			append);


/*
 * ../../libstring/strchr.c
 */

char*			strchr(const char*			p,
			       int				ch);


/*
 * ../../libstring/strcmp.c
 */

int			strcmp(const char*			s1,
			       const char*			s2);


/*
 * ../../libstring/strcpy.c
 */

char*			strcpy(char*				to,
			       const char*			from);


/*
 * ../../libstring/strdup.c
 */

char*			strdup(const char*			from);


/*
 * ../../libstring/strlen.c
 */

size_t			strlen(const char*			str);


/*
 * ../../libstring/strncat.c
 */

char*			strncat(char*				dst,
				char*				src,
				size_t				n);


/*
 * ../../libstring/strncmp.c
 */

int			strncmp(const char*			s1,
				const char*			s2,
				size_t				n);


/*
 * ../../libstring/strncpy.c
 */

char*			strncpy(char*				to,
				char*				from,
				size_t				n);


/*
 * ../../libstring/strrchr.c
 */

char*			strrchr(const char*			p,
				int				ch);


/*
 * ../../libstring/strtol.c
 */

long			strtol(const char*			nptr,
			       char**				endptr,
			       int				base);


/*
 * ../../libstring/strtoul.c
 */

unsigned long		strtoul(const char*			nptr,
				char**				endptr,
				int				base);


/*
 * eop
 */

#endif
