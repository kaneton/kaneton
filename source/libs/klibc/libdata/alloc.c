/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * malloc.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:50:21 2005   mycure
 * last update   Sun Jul  3 22:39:32 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

t_alloc			alloc;

/*
 * ---------- functions -------------------------------------------------------
 */

void*			malloc(size_t				size)
{

}

int			alloc_init(t_vaddr			addr,
				   t_size			size)
{
  memset(&alloc, 0x0, sizeof(t_alloc));

  // XXX alloc.free = alloc_add(alloc.free, addr, size);
}
