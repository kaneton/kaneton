/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/educational/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       julien quintard   [mon nov 22 22:31:01 2010]
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
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the region manager.
 */

extern m_region*	_region;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager dispatch.
 */

d_region		region_dispatch =
  {
    /*							   [block::dispatch] */
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
    /*							[endblock::dispatch] */
  };

/*
 * ---------- functions -------------------------------------------------------
 */


/*							  [block::functions] */

/*
 * this function resizes a region.
 *
 * this is an extremely simple implementation...
 */

t_error			glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  o_region*		reg;
  o_as*			o;

  if (region_get(as, old, &reg) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  if (ia32_unmap_region(as, reg->address, reg->size) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  if (ia32_map_region(as,
		      reg->segment,
		      reg->offset,
		      reg->options,
		      reg->address,
		      size) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  REGION_LEAVE(_region, ERROR_OK);
}

/*
 * reserves a region.
 *
 */

t_error			glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_options		options,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid)
{
  REGION_ENTER(_region);

  if (ia32_map_region(asid,
		      segid,
		      offset,
		      options,
		      address,
		      size) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  REGION_LEAVE(_region, ERROR_OK);
}

/*
 * this function  releases a region.
 *
 */

t_error			glue_region_release(i_as		asid,
					    i_region		regid)
{
  o_region*		reg;

  REGION_ENTER(_region);

  if (region_get(asid, regid, &reg) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  if (ia32_unmap_region(asid, reg->address, reg->size) != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  REGION_LEAVE(_region, ERROR_OK);
}

/*
 * this function just initializes the machine-dependent region manager.
 *
 */

t_error			glue_region_initialize(t_vaddr		start,
					       t_vsize		size)
{
  REGION_ENTER(_region);

  if (ia32_paging_init() != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  REGION_LEAVE(_region, ERROR_OK);
}

/*
 * this function cleans the machine-dependent region manager.
 *
 */

t_error			glue_region_clean(void)
{
  REGION_ENTER(_region);

  if (ia32_paging_clean() != ERROR_OK)
    REGION_LEAVE(_region, ERROR_KO);

  REGION_LEAVE(_region, ERROR_OK);
}

/*						       [endblock::functions] */
