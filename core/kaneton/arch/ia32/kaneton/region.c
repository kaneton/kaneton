/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/kaneton/arch/ia32/kaneton/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       julien quintard   [thu dec 15 11:50:39 2005]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_region*	region;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager interface.
 */

i_region		region_interface =
  {
    NULL,
    NULL,
    NULL,
    ia32_region_init,
    ia32_region_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just initialises the machine-dependent region manager.
 *
 * for the intel 32-bit architecture we just initialise the paging mode.
 */

t_error			ia32_region_init(t_fit			fit,
					 t_vaddr		start,
					 t_vsize		size)
{
  REGION_ENTER(region);

  /* XXX ia32_paging_init ?
  if (paging_init() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_NONE);
  */

  REGION_LEAVE(region, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent region manager.
 */

t_error			ia32_region_clean(void)
{
  REGION_ENTER(region);

  /* XXX
  if (paging_clean() != ERROR_NONE)
    REGION_LEAVE(region, ERROR_NONE);
  */

  REGION_LEAVE(region, ERROR_NONE);
}
