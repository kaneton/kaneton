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
 * last update   Sun Jul  3 14:54:01 2005   mycure
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

#include <klibc/include/libdata/malloc.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../libdata/dump.c
 *      ../../libdata/sum2.c
 */

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
