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
 * updated       julien quintard   [fri jan 14 14:16:54 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the segment manager's glue.
 *
 * the machine behaviour consists in managing the IA32 segmentation model.
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
 * the segment dispatcher.
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
    NULL,
    glue_segment_read,
    glue_segment_write,
    glue_segment_copy,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_segment_initialize,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reads directly from a segment to a buffer.
 */

t_error			glue_segment_read(i_segment		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz)
{
  /*
   * XXX
   */

  if (architecture_paging_read(segid, offs, buff, sz) != ERROR_OK)
    MACHINE_ESCAPE("unable to read data from the segment");

  MACHINE_LEAVE();
}

/*
 * this function writes directly to a segment from a buffer.
 */

t_error			glue_segment_write(i_segment		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz)
{
  /*
   * XXX
   */

  if (architecture_paging_write(segid, offs, buff, sz) != ERROR_OK)
    MACHINE_ESCAPE("unable to write data to the segment");

  MACHINE_LEAVE();
}

/*
 * this function copies a chunk of data from one segment to another.
 */

t_error			glue_segment_copy(i_segment		dst,
					  t_paddr		offsd,
					  i_segment		src,
					  t_paddr		offss,
					  t_psize		sz)
{
  /*
   * XXX
   */

  if (architecture_paging_copy(dst, offsd, src, offss, sz) != ERROR_OK)
    MACHINE_ESCAPE("unable to copy data from a segment to another");

  MACHINE_LEAVE();
}

/*
 * this function initializes the segment manager's glue.
 */

t_error			glue_segment_initialize(void)
{
  /*
   * XXX
   */

  if (architecture_pmode_setup() != ERROR_OK)
    MACHINE_ESCAPE("unable to set up the protected mode");

  /*
   * XXX
   */

  if (architecture_segmentation_setup() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the IA32 segmentation");

  MACHINE_LEAVE();
}
