/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * libdata.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/include/libdata
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:39 2005   mycure
 * last update   Sun Jul  3 14:49:36 2005   mycure
 */

#ifndef LIBDATA_LIBDATA_H
#define LIBDATA_LIBDATA_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <klibc/include/libsys/types.h>

/*
 * ---------- includes --------------------------------------------------------
 */

// XXX #include <klibc/include/libdata/malloc.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../libstring/dump.c
 *      ../../libstring/sum2.c
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
 * ../../libstring/dump.c
 */

void			dump_buf(u_int8_t*			buf,
				 u_int				address,
				 u_int				size);

void			dump(char*				data,
			     u_int				size);


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
 * ../../libstring/sum2.c
 */

u_int32_t		sum2(char				*buf,
			     int				size);


/*
 * eop
 */

#endif
