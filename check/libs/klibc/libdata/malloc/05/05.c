/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/05/05.c
 *
 * created       matthieu bucchianeri   [sun jun  4 19:37:30 2006]
 * updated       matthieu bucchianeri   [sun jun  4 20:03:53 2006]
 */

#include <kaneton.h>
#include <klibc.h>
#include "../common/common.h"

extern i_as		kasid;

/*
 * one big malloc.
 */

void		check_malloc_05(void)
{
  o_as*		kas;
  o_region*	reg;
  t_setsz	nb_regions;
  t_setsz	nb_regions_after;
  void		*p;
  size_t	big = 42 * PAGESZ + 1234;

  TEST_ENTER(0);

  MY_ASSERT(as_get(kasid, &kas) == ERROR_NONE,
	    "cannot get kernel as\n");

  MY_ASSERT(set_size(kas->regions, &nb_regions) == ERROR_NONE,
	    "cannot get region count\n");

  p = check_init_memory(malloc(big), big);
  check_memory(p, big);

  MY_ASSERT(set_size(kas->regions, &nb_regions_after) == ERROR_NONE,
	    "cannot get region count\n");

  MY_ASSERT(nb_regions < nb_regions_after,
	    "no region has been created\n");

  MY_ASSERT(region_get(kasid, (i_region)(t_uint32)p -
		       sizeof(t_chunk) - sizeof(t_area),
		       &reg) == ERROR_NONE,
	    "cannot get associated region\n");

  MY_ASSERT(reg->size > big, "region size does not match\n");

  free(p);

  MY_ASSERT(set_size(kas->regions, &nb_regions_after) == ERROR_NONE,
	    "cannot get region count\n");

  MY_ASSERT(nb_regions == nb_regions_after,
	    "region not released\n");

  TEST_LEAVE;
}