/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/kaneton/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue dec 20 14:40:48 2005]
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
    NULL,
    NULL,
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
