/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/mjpeg/memspace.c
 *
 * created       matthieu bucchianeri   [sat dec  2 01:56:45 2006]
 * updated       matthieu bucchianeri   [sat dec  2 02:43:26 2006]
 */

#include <klibc.h>

#include "dsx.h"

void	dsx_memspace_alloc(dsx_memspace_t*	memsp,
			   size_t		size)
{
  *memsp = malloc(sizeof(**memsp));
  (*memsp)->base = malloc(size);
  (*memsp)->size = size;
}
