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
 * updated       julien quintard   [thu jan 13 16:52:22 2011]
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

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region dispatcher.
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
    NULL

    /*							[endblock::dispatch] */
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*							  [block::functions] */

/*
 * this function resizes a region.
 *
 * note that the glue implementation is quite inefficient but extremely
 * simple as many scenarios could occur: larger, smaller, in-place, relocated
 * etc.
 *
 * the current implementation therefore consists in unmapping the pages
 * of the previous region before mapping the page corresponding to the
 * region's new size.
 */

t_error			glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  o_region*		reg;

  /*
   * XXX
   */

  if (region_get(as, old, &reg) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * XXX
   */

  if (architecture_paging_unmap(as, reg->address, reg->size) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the virtual memory corresponding to the "
		   "original region");

  /*
   * XXX
   */

  if (architecture_paging_map(as,
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
 * this function reserves a region and therefore calls the architecture
 * so that the region's page get mapped.
 */

t_error			glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_options		options,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid)
{
  /*
   * XXX
   */

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
 * this function releases a region. the pages corresponding to the region's
 * area of memory are thus unmapped.
 */

t_error			glue_region_release(i_as		asid,
					    i_region		regid)
{
  o_region*		reg;

  /*
   * XXX
   */

  if (region_get(asid, regid, &reg) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * XXX
   */

  if (architecture_paging_unmap(asid, reg->address, reg->size) != ERROR_OK)
    MACHINE_ESCAPE("unable to unmap the virtual memory corresponding to "
		   "the region");

  MACHINE_LEAVE();
}

/*
 * this function initializes the region manager's glue.
 */

t_error			glue_region_initialize(t_vaddr		base,
					       t_vsize		size)
{
  /*
   * XXX
   */

  if (architecture_paging_setup() != ERROR_OK)
    MACHINE_ESCAPE("unable to set up the paging");

  MACHINE_LEAVE();
}

/*						       [endblock::functions] */
