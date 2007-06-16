/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...aneton/machine/glue/ibm-pc.ia32/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       matthieu bucchianeri   [sat jun 16 18:37:18 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for region  manager on  ia32 with
 * paging architecture.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_region*	region;
extern i_as		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager dispatch.
 */

d_region		region_dispatch =
  {
/*                                                                  [cut] k1 */
    NULL,
    NULL,
    NULL,
    glue_region_resize,
    NULL,
    glue_region_reserve,
    glue_region_release,
    NULL,
    glue_region_init,
    glue_region_clean
/*                                                                 [cut] /k1 */
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function resizes a region.
 */

t_error			glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  // XXX
}

/*
 * reserves a region.
 */

t_error			glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid)
{
  REGION_ENTER(region);

  if (ia32_map_region(asid, segid, offset, opts, address, size) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function  releases a region.
 */

t_error			glue_region_release(i_as		asid,
					    i_region		regid)
{
  o_region*		reg;

  REGION_ENTER(region);

  if (region_get(asid, regid, &reg) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (ia32_unmap_region(asid, reg->address, reg->size) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function just initialises the machine-dependent region manager.
 */

t_error			glue_region_init(t_vaddr		start,
					 t_vsize		size)
{
  REGION_ENTER(region);

  if (ia32_paging_init() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent region manager.
 */

t_error			glue_region_clean(void)
{
  REGION_ENTER(region);

  if (ia32_paging_clean() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  REGION_LEAVE(region, ERROR_NONE);
}
