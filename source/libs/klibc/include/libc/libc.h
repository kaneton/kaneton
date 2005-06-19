/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * libc.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:39 2005   mycure
 * last update   Sun Jun 19 22:16:27 2005   mycure
 */

#ifndef LIBC_LIBC_H
#define LIBC_LIBC_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <klibc/include/libsys/types.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/libc/ctype.h>
#include <klibc/include/libc/printf.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../libc/ctype.c
 *      ../../libc/bcmp.c
 *      ../../libc/bcopy.c
 *      ../../libc/bzero.c
 *      ../../libc/dump.c
 *      ../../libc/index.c
 *      ../../libc/memcmp.c
 *      ../../libc/memcpy.c
 *      ../../libc/memset.c
 *      ../../libc/printf.c
 *      ../../libc/rindex.c
 *      ../../libc/strcat.c
 *      ../../libc/strchr.c
 *      ../../libc/strcmp.c
 *      ../../libc/strcpy.c
 *      ../../libc/strlen.c
 *      ../../libc/strncat.c
 *      ../../libc/strncmp.c
 *      ../../libc/strncpy.c
 *      ../../libc/strtol.c
 *      ../../libc/strtoul.c
 *      ../../libc/sum2.c
 */

/*
 * ../../libc/ctype.c
 */

/*
 * ../../libc/bcmp.c
 */

int			bcmp(const void*			b1,
			     const void*			b2,
			     size_t				length);


/*
 * ../../libc/bcopy.c
 */

void			bcopy(const void*			src,
			      void*				dst,
			      size_t				n);


/*
 * ../../libc/bzero.c
 */

void			bzero(void*				dst,
			      size_t				length);


/*
 * ../../libc/dump.c
 */

void			dump_buf(u_int8_t*			buf,
				 u_int				address,
				 u_int				size);


void			dump(char*				data,
			     u_int				size);


/*
 * ../../libc/index.c
 */

char*			index(const char*			p,
			      int				ch);


/*
 * ../../libc/memcmp.c
 */

int			memcmp(const void*			s1,
			       const void*			s2,
			       size_t				n);


/*
 * ../../libc/memcpy.c
 */

void*			memcpy(void*				dest,
			       const void*			src,
			       size_t				n);


/*
 * ../../libc/memset.c
 */

void*			memset(void*				s,
			       int				c,
			       size_t				n);


/*
 * ../../libc/printf.c
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
 * ../../libc/rindex.c
 */

char*			rindex(const char*			p,
			       int				ch);


/*
 * ../../libc/strcat.c
 */

char*			strcat(char*				s,
			       const char*			append);


/*
 * ../../libc/strchr.c
 */

char*			strchr(const char*			p,
			       int				ch);


/*
 * ../../libc/strcmp.c
 */

int			strcmp(const char*			s1,
			       const char*			s2);


/*
 * ../../libc/strcpy.c
 */

char*			strcpy(char*				to,
			       const char*			from);


/*
 * ../../libc/strlen.c
 */

size_t			strlen(const char*			str);


/*
 * ../../libc/strncat.c
 */

char*			strncat(char*				dst,
				char*				src,
				size_t				n);


/*
 * ../../libc/strncmp.c
 */

int			strncmp(const char*			s1,
				const char*			s2,
				size_t				n);


/*
 * ../../libc/strncpy.c
 */

char*			strncpy(char*				to,
				char*				from,
				size_t				n);


/*
 * ../../libc/strtol.c
 */

long			strtol(const char*			nptr,
			       char**				endptr,
			       int				base);


/*
 * ../../libc/strtoul.c
 */

unsigned long		strtoul(const char*			nptr,
				char**				endptr,
				int				base);


/*
 * ../../libc/sum2.c
 */

u_int32_t		sum2(char				*buf,
			     int				size);

/*
 * eop
 */

#endif
