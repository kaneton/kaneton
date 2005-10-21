/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * libdata.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/libdata
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:39 2005   mycure
 * last update   Fri Oct 21 19:36:14 2005   mycure
 */

#ifndef LIBDATA_LIBDATA_H
#define LIBDATA_LIBDATA_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libdata/alloc.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../libdata/alloc.c
 *      ../../libdata/dump.c
 *      ../../libdata/sum2.c
 */

/*
 * ../../libdata/alloc.c
 */

void*			malloc(size_t				size);

void			free(void*				ptr);

u_int32_t		alloc_nalloc(void);

u_int32_t		alloc_nfree(void);

void			alloc_dump(void);

void*			realloc(void* 				ptr,
				size_t				size);

int			alloc_init(t_vaddr			addr,
				   t_size			size,
				   t_fit			fit);

void			alloc_test(void);


/*
 * ../../libdata/dump.c
 */

void			dump_buf(u_int8_t*			buf,
				 u_int				address,
				 u_int				size);

void			dump(char*				data,
			     u_int				size);


/*
 * ../../libdata/sum2.c
 */

u_int32_t		sum2(char				*buf,
			     int				size);


/*
 * eop
 */

#endif
