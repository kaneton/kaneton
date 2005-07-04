/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * malloc.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:39 2005   mycure
 * last update   Mon Jul  4 14:28:40 2005   mycure
 */

#ifndef LIBDATA_ALLOC_H
#define LIBDATA_ALLOC_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <klibc/include/libsys/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * chunck
 */

typedef struct			s_chunck
{
  size_t			size;

  struct s_chunck*		prv;
  struct s_chunck*		nxt;
}				t_chunck;

/*
 * alloc global information
 */

typedef struct
{
  t_chunck*			alloc;
  t_chunck*			free;
}				t_alloc;

#endif
