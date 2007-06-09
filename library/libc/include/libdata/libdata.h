/*
 * licence kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libdata/libdata.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:53:43 2006]
 */

#ifndef LIBDATA_LIBDATA_H
#define LIBDATA_LIBDATA_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>
// XXX #include <arch/machdep/machdep.h>

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

void			alloc_check_signatures(void);

void*			realloc(void* 				ptr,
				size_t				size);

void			alloc_setup(t_pfn_map_reserve		freserve,
				    t_pfn_map_release		frelease,
				    i_as			asid);

int			alloc_init(t_vaddr			addr,
				   t_size			size);


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
