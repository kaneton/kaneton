/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/glue/ibm-pc.ia32/optimised/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       julien quintard   [fri may  1 19:44:25 2009]
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
    NULL,
    NULL,
    NULL,
    glue_region_resize,
    NULL,
    glue_region_reserve,
    glue_region_release,
    NULL,
    glue_region_initialize,
    glue_region_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function resizes a region.
 */

t_status		glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  // XXX
}

/*
 * reserves a region.
 */

t_status		glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid)
{
  REGION_ENTER(region);

  /* XXX flush AP */
  if (ia32_map_region(asid, segid, offset, opts, address, size) != STATUS_OK)
    REGION_LEAVE(region, STATUS_UNKNOWN_ERROR);

  REGION_LEAVE(region, STATUS_OK);
}

/*
 * this function  releases a region.
 */

t_status		glue_region_release(i_as		asid,
					    i_region		regid)
{
  o_region*		reg;

  REGION_ENTER(region);

  if (region_get(asid, regid, &reg) != STATUS_OK)
    REGION_LEAVE(region, STATUS_UNKNOWN_ERROR);

  /* XXX flush AP */
  if (ia32_unmap_region(asid, reg->address, reg->size) != STATUS_OK)
    REGION_LEAVE(region, STATUS_UNKNOWN_ERROR);

  REGION_LEAVE(region, STATUS_OK);
}

/*
 * this function just initializes the machine-dependent region manager.
 */

t_status		glue_region_initialize(t_vaddr		start,
					       t_vsize		size)
{
  REGION_ENTER(region);

  if (ia32_paging_init() != STATUS_OK)
    REGION_LEAVE(region, STATUS_UNKNOWN_ERROR);

  REGION_LEAVE(region, STATUS_OK);
}

/*
 * this function cleans the machine-dependent region manager.
 */

t_status		glue_region_clean(void)
{
  REGION_ENTER(region);

  if (ia32_paging_clean() != STATUS_OK)
    REGION_LEAVE(region, STATUS_UNKNOWN_ERROR);

  REGION_LEAVE(region, STATUS_OK);
}
