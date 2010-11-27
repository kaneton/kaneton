/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/glue/ibm-pc.ia32/educational/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sat nov 27 05:08:36 2010]
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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 segment manager dispatch.
 */

d_segment		glue_segment_dispatch =
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

t_error			glue_segment_read(i_segment		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz)
{
  if (ia32_segment_read(segid, offs, buff, sz) != ERROR_OK)
    MACHINE_ESCAPE("unable to read data from the segment");

  MACHINE_LEAVE();
}

/*
 * this function write directly to a segment from a buffer.
 *
 */

t_error			glue_segment_write(i_segment		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz)
{
  if (ia32_segment_write(segid, offs, buff, sz) != ERROR_OK)
    MACHINE_ESCAPE("unable to write data to the segment");

  MACHINE_LEAVE();
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 */

t_error			glue_segment_copy(i_segment		dst,
					  t_paddr		offsd,
					  i_segment		src,
					  t_paddr		offss,
					  t_psize		sz)
{
  if (ia32_segment_copy(dst, offsd, src, offss, sz) != ERROR_OK)
    MACHINE_ESCAPE("unable to copy data from a segment to another");

  MACHINE_LEAVE();
}

/*
 * this function just initializes the machine-dependent segment manager.
 */

t_error			glue_segment_initialize(void)
{
  if (ia32_pmode_init() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the protected mode");

  if (ia32_segmentation_init() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the IA32 segmentation");

  MACHINE_LEAVE();
}

/*
 * this function cleans the machine-dependent segment manager.
 */

t_error			glue_segment_clean(void)
{
  // XXX clean segmentation.

  if (ia32_pmode_clean() != ERROR_OK)
    MACHINE_ESCAPE("unable to clean the protected mode");

  MACHINE_LEAVE();
}
