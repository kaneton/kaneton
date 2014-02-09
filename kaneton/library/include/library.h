/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/include/library.h
 *
 * created       julien quintard   [sun jun 10 17:31:41 2007]
 * updated       julien quintard   [thu dec 16 11:41:59 2010]
 */

#ifndef LIBRARY_LIBRARY_H
#define LIBRARY_LIBRARY_H			1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define NULL			((void*)0)

#define EOF			-1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/types.h>
#include <library/stdarg.h>
#include <library/format.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../alloc.c
 *	../format.c
 *	../memcpy.c
 *	../memcmp.c
 *	../memset.c
 *      ../printf.c
 *      ../random.c
 *      ../snprintf.c
 *      ../sprintf.c
 *	../strchr.c
 *	../strcmp.c
 *	../strcpy.c
 *	../strdup.c
 *	../strlen.c
 *	../strncmp.c
 *	../strncpy.c
 *	../strrchr.c
 *	../strtol.c
 *	../strtoul.c
 *	../sum2.c
 */

/*
 * ../alloc.c
 */

void*			malloc(size_t				size);

void			free(void*				ptr);

u_int32_t		alloc_nalloc(void);

u_int32_t		alloc_nfree(void);

void			alloc_dump(void);

void			alloc_check_signatures(void);

void*			realloc(void* 				ptr,
				size_t				size);

void			alloc_setup(void);

int			alloc_init(paddr_t			addr,
				   psize_t			size);


/*
 * ../format.c
 */

int			format_string(t_format_character_fn	character,
				      t_format_attribute_fn	attribute,
				      char*			string,
				      unsigned int		flags,
				      int			len1,
				      int			len2);

int			format_quad(t_format_character_fn	character,
				    t_format_attribute_fn	attribute,
				    quad_t			value,
				    int				base,
				    int				hdl_sign,
				    unsigned int		flags,
				    int				len1,
				    int				len2);

int			format(t_format_character_fn		character,
			       t_format_attribute_fn		attribute,
			       const char*			fmt,
			       va_list				args);


/*
 * ../memcpy.c
 */

void*			memcpy(void*				dest,
			       const void*			src,
			       size_t				n);


/*
 * ../memcmp.c
 */

int			memcmp(const void*			s1,
			       const void*			s2,
			       size_t				n);


/*
 * ../memset.c
 */

void*			memset(void*				s,
			       int				c,
			       size_t				n);


/*
 * ../printf.c
 */

void			printf_character(char			c);

int			printf(char*				fmt,
			       ...);


/*
 * ../random.c
 */

void			random_seed(void);

unsigned long		random_generate(void);


/*
 * ../snprintf.c
 */

void			snprintf_character(char			c);

int			snprintf(char*				buffer,
				 int				size,
				 char*				fmt,
				 ...);


/*
 * ../sprintf.c
 */

void			sprintf_character(char			c);

int			sprintf(char*				buffer,
				char*				fmt,
			       ...);


/*
 * ../strchr.c
 */

char*			strchr(const char*			p,
			       int				ch);


/*
 * ../strcmp.c
 */

int			strcmp(const char*			s1,
			       const char*			s2);


/*
 * ../strcpy.c
 */

char*			strcpy(char*				to,
			       const char*			from);


/*
 * ../strdup.c
 */

char*			strdup(const char*			from);


/*
 * ../strlen.c
 */

size_t			strlen(const char*			str);


/*
 * ../strncmp.c
 */

int			strncmp(const char*			s1,
				const char*			s2,
				size_t				n);


/*
 * ../strncpy.c
 */

char*			strncpy(char*				to,
				char*				from,
				size_t				n);


/*
 * ../strrchr.c
 */

char*			strrchr(const char*			p,
				int				ch);


/*
 * ../strtol.c
 */

long			strtol(const char*			nptr,
			       char**				endptr,
			       int				base);


/*
 * ../strtoul.c
 */

unsigned long		strtoul(const char*			nptr,
				char**				endptr,
				int				base);


/*
 * ../sum2.c
 */

u_int32_t		sum2(char				*buf,
			     int				size);


/*
 * eop
 */

#endif
