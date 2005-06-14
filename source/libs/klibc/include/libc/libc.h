/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * libc.h
 * 
 * path          /home/mycure/kaneton/libs/klibc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:39 2005   mycure
 * last update   Tue Jun 14 14:21:51 2005   mycure
 */

#ifndef LIBC_LIBC_H
#define LIBC_LIBC_H		1

/*
 * includes
 */

#include <arch/machdep/machdep.h>

#include "ctype.h"
#include "printf.h"

/*
 * prototypes:     ../../libc/_ctype.c           ../../libc/bcmp.c
 *                 ../../libc/bcopy.c            ../../libc/bzero.c
 *                 ../../libc/dump.c             ../../libc/strchr.c
 *                 ../../libc/rindex.c           ../../libc/index.c
 *                 ../../libc/memcmp.c           ../../libc/memset.c
 *                 ../../libc/strcat.c           ../../libc/strcmp.c
 *                 ../../libc/strcpy.c           ../../libc/memcpy.c
 *                 ../../libc/strlen.c           ../../libc/strncat.c
 *                 ../../libc/strncmp.c          ../../libc/strncpy.c
 *                 ../../libc/strtol.c           ../../libc/strtoul.c
 *                 ../../libc/printf.c           ../../libc/sum2.c
 */

/*
 * ../../libc/_ctype.c
 */

/*
 * ../../libc/bcmp.c
 */

int			bcmp(const void		*b1,
			     const void		*b2,
			     size_t		length);


/*
 * ../../libc/bcopy.c
 */

void			bcopy(const void	*src,
			      void		*dst,
			      size_t		n);


/*
 * ../../libc/bzero.c
 */

void			bzero(void		*dst,
			      size_t		length);


/*
 * ../../libc/dump.c
 */

void			dump_buf(u_int8_t	*buf,
				 u_int		address,
				 u_int		size);


void			dump(char		*data,
			     u_int		size);


/*
 * ../../libc/strchr.c
 */

char			*strchr(const char	*p,
				int		ch);


/*
 * ../../libc/rindex.c
 */

char			*rindex(const char	*p,
				int		ch);


/*
 * ../../libc/index.c
 */

char			*index(const char	*p,
			       int		ch);


/*
 * ../../libc/memcmp.c
 */

int			memcmp(const void	*s1,
			       const void	*s2,
			       size_t n);


/*
 * ../../libc/memset.c
 */

void			*memset(void		*s,
				int		c,
				size_t		n);


/*
 * ../../libc/strcat.c
 */

char			*strcat(char		*s,
				const char	*append);


/*
 * ../../libc/strcmp.c
 */

int			strcmp(const char	*s1,
			       const char	*s2);


/*
 * ../../libc/strcpy.c
 */

char			*strcpy(char		*to,
				const char	*from);


/*
 * ../../libc/memcpy.c
 */

void			*memcpy(void		*dest,
				const void	*src,
				size_t		n);


/*
 * ../../libc/strlen.c
 */

size_t			strlen(const char	*str);


/*
 * ../../libc/strncat.c
 */

char			*strncat(char		*dst,
				 char		*src,
				 size_t		n);


/*
 * ../../libc/strncmp.c
 */

int			strncmp(const char	*s1,
				const char	*s2,
				size_t		n);


/*
 * ../../libc/strncpy.c
 */

char			*strncpy(char		*to,
				 char		*from,
				 size_t		n);


/*
 * ../../libc/strtol.c
 */

long			strtol(const char	*nptr,
			       char		**endptr,
			       int		base);


/*
 * ../../libc/strtoul.c
 */

u_long			strtoul(const char	*nptr,
				char		**endptr,
				int		base);


/*
 * ../../libc/printf.c
 */

int			printf_string(char			*string,
				      unsigned int		flags,
				      int			len1,
				      int			len2);


int			printf_quad(quad_t			value,
				    int				base,
				    int				hdl_sign,
				    unsigned int		flags,
				    int				len1,
				    int				len2);


int			vprintf(const char			*fmt,
				va_list				args);


int			printf_init(t_printf_char_fn		pc,
				    t_printf_attr_fn		pa);


int			printf(char				*fmt,
			       ...);


/*
 * ../../libc/sum2.c
 */

u_int32_t		sum2(char		*buf,
			     int		size);

/*
 * eop
 */

#endif
