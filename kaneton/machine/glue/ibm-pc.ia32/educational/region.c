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
 * updated       julien quintard   [fri dec 10 21:27:31 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for region  manager on  ia32 with
 * paging architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager dispatch.
 */

d_region		glue_region_dispatch =
  {
    /*							   [block::dispatch] */
    NULL,
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
 * this is an extremely simple and inefficient implementation... but
 * it does work.
 */

t_error			glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  o_region*		reg;

  if (region_get(as, old, &reg) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  if (ia32_unmap_region(as, reg->address, reg->size) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the virtual memory corresponding to the "
		   "original region");

  if (ia32_map_region(as,
		      reg->segment,
		      reg->offset,
		      reg->options,
		      reg->address,
		      size) != ERROR_OK)
    MACHINE_ESCAPE("unable to map the virtual memory corresponding to the "
		   "future resized region");

  MACHINE_LEAVE();
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
  if (ia32_map_region(asid,
		      segid,
		      offset,
		      options,
		      address,
		      size) != ERROR_OK)
    MACHINE_ESCAPE("unable to map the virtual memory corresponding to the "
		   "reserved region");

  MACHINE_LEAVE();
}

/*
 * this function  releases a region.
 *
 */

t_error			glue_region_release(i_as		asid,
					    i_region		regid)
{
  o_region*		reg;

  if (region_get(asid, regid, &reg) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  if (ia32_unmap_region(asid, reg->address, reg->size) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the virtual memory corresponding to "
		   "the region");

  MACHINE_LEAVE();
}

/*
 * this function just initializes the machine-dependent region manager.
 *
 */

t_error			glue_region_initialize(t_vaddr		base,
					       t_vsize		size)
{
  if (ia32_paging_init() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the paging");

  MACHINE_LEAVE();
}

/*
 * this function cleans the machine-dependent region manager.
 *
 */

t_error			glue_region_clean(void)
{
  if (ia32_paging_clean() != ERROR_OK)
    MACHINE_ESCAPE("unable to clean the paging");

  MACHINE_LEAVE();
}

/*						       [endblock::functions] */
