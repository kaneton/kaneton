/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...e/glue/ibm-pc.ia32/educational/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [wed jan  9 11:59:43 2008]
 */


/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for the segment manager on a ibm-pc.ia32
 * machine.
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

extern m_segment*	segment;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 segment manager dispatch.
 */

d_segment				segment_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_segment_read,
    glue_segment_write,
    glue_segment_copy,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_segment_initialize,
    glue_segment_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reads directly from a segment to a buffer.
 *
 */

t_status		glue_segment_read(i_segment		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz)
{
  SEGMENT_ENTER(segment);

  if (ia32_segment_read(segid, offs, buff, sz) != STATUS_OK)
    SEGMENT_LEAVE(segment, STATUS_UNKNOWN_ERROR);

  SEGMENT_LEAVE(segment, STATUS_OK);
}

/*
 * this function write directly to a segment from a buffer.
 *
 */

t_status		glue_segment_write(i_segment		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz)
{
  SEGMENT_ENTER(segment);

  if (ia32_segment_write(segid, offs, buff, sz) != STATUS_OK)
    SEGMENT_LEAVE(segment, STATUS_UNKNOWN_ERROR);

  SEGMENT_LEAVE(segment, STATUS_OK);
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 */

t_status		glue_segment_copy(i_segment		dst,
					  t_paddr		offsd,
					  i_segment		src,
					  t_paddr		offss,
					  t_psize		sz)
{
  SEGMENT_ENTER(segment);

  if (ia32_segment_copy(dst, offsd, src, offss, sz) != STATUS_OK)
    SEGMENT_LEAVE(segment, STATUS_UNKNOWN_ERROR);

  SEGMENT_LEAVE(segment, STATUS_OK);
}

/*
 * this function just initializes the machine-dependent segment manager.
 */

t_status		glue_segment_initialize(void)
{
  SEGMENT_ENTER(segment);

  if (ia32_pmode_init() != STATUS_OK)
    SEGMENT_LEAVE(segment, STATUS_UNKNOWN_ERROR);

  if (ia32_segmentation_init() != STATUS_OK)
    SEGMENT_LEAVE(segment, STATUS_UNKNOWN_ERROR);

  SEGMENT_LEAVE(segment, STATUS_OK);
}

/*
 * this function cleans the machine-dependent segment manager.
 */

t_status		glue_segment_clean(void)
{
  SEGMENT_ENTER(segment);

  if (ia32_pmode_clean() != STATUS_OK)
    SEGMENT_LEAVE(segment, STATUS_UNKNOWN_ERROR);

  SEGMENT_LEAVE(segment, STATUS_OK);
}
