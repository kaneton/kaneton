/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue jan 10 00:30:50 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_segment*	segment;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager interface.
 */

i_segment		segment_interface =
  {
    NULL,
    ia32_segment_reserve,
    ia32_segment_release,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_segment_init,
    ia32_segment_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just initialises the machine-dependent segment manager.
 *
 * for the intel 32-bit architecture we just initialise the protected mode.
 */

t_error			ia32_segment_init(t_fit			fit)
{
  SEGMENT_ENTER(segment);

  if (pmode_init() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent segment manager.
 */

t_error			ia32_segment_clean(void)
{
  SEGMENT_ENTER(segment);

  if (pmode_clean() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function reserves a segment given the desired size.
 */

t_error			ia32_segment_reserve(t_asid		asid,
					     t_psize		size,
					     t_perms		perms,
					     t_segid*		segid)
{
  SEGMENT_ENTER(segment);

  /* XXX */

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}

/*
 * this function releases a segment.
 */

t_error			ia32_segment_release(t_segid			segid)
{
  SEGMENT_ENTER(segment);

  /* XXX */

  SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
}
