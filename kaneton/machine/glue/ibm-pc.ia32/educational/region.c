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
 * updated       julien quintard   [fri apr  8 08:12:26 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the region manager's glue which basically consists
 * in updating the address space's page directory, tables etc. in order
 * to reflect the core operation.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region dispatcher.
 */

d_region		glue_region_dispatch =
  {
    NULL,
    NULL,
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
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function resizes a region.
 *
 * note that the glue implementation is quite inefficient but extremely
 * simple.
 *
 * the current implementation therefore consists in unmapping the pages
 * of the previous region before mapping the page corresponding to the
 * region's new size.
 *
 * steps:
 *
 * 1) retrieve the region object.
 * 2) destroy the old mapping.
 * 3) map the new region.
 */

t_error			glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  o_region*		o;

  /*
   * 1)
   */

  if (region_get(as, old, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 2)
   */

  if (architecture_paging_unmap(as,
				o->address,
				o->size) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the virtual memory corresponding to the "
		   "original region");

  /*
   * 3)
   */

  if (architecture_paging_map(as,
			      o->segment,
			      o->offset,
			      o->options,
			      o->address,
			      size) != ERROR_OK)
    MACHINE_ESCAPE("unable to map the virtual memory corresponding to the "
		   "future resized region");

  MACHINE_LEAVE();
}

/*
 * this function maps the freshly reserved region.
 */

t_error			glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_options		options,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid)
{
  if (architecture_paging_map(asid,
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
 * this function unmaps the pages associated with the given region.
 *
 * steps:
 *
 * 1) retrieve the region object.
 * 2) unmap the pages.
 */

t_error			glue_region_release(i_as		asid,
					    i_region		regid)
{
  o_region*		o;

  /*
   * 1)
   */

  if (region_get(asid, regid, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 2)
   */

  if (architecture_paging_unmap(asid,
				o->address,
				o->size) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the virtual memory corresponding to "
		   "the region");

  MACHINE_LEAVE();
}

/*
 * this function initializes the region manager's glue.
 *
 * steps:
 *
 * 1) set up the IA32 paging unit.
 */

t_error			glue_region_initialize(t_vaddr		base,
					       t_vsize		size)
{
  /*
   * 1)
   */

  if (architecture_paging_setup() != ERROR_OK)
    MACHINE_ESCAPE("unable to set up the paging");

  MACHINE_LEAVE();
}
