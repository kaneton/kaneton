/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...hine/architecture/ia32/generic/segment.c
 *
 * created       matthieu bucchianeri   [mon dec 24 19:25:22 2007]
 * updated       matthieu bucchianeri   [mon dec 24 19:31:21 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * segments operations on ia32.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * kernel address space identifier.
 */

extern i_as		kasid;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reads directly from a segment to a buffer.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) map the segment portion with a region.
 * 3) copy from segment to the buffer.
 * 4) unmap the region.
 */

t_error			ia32_segment_read(i_region		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz)
{
  o_segment*		o;
  i_region		reg;
  t_paddr		poffset;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs % PAGESZ)
  {
    poffset = offs & ~(PAGESZ - 1);
    offs -= poffset;
  }
  else
  {
    poffset = offs;
    offs = 0;
  }

  end = poffset + offs + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, segid, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy(buff, (void*)(t_vaddr)reg + offs, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function write directly to a segment from a buffer.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) map the segment portion with a region.
 * 3) copy from the buffer to the segment.
 * 4) unmap the region.
 */

t_error			ia32_segment_write(i_region		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz)
{
  o_segment*		o;
  i_region		reg;
  t_paddr		poffset;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs % PAGESZ)
  {
    poffset = offs & ~(PAGESZ - 1);
    offs -= poffset;
  }
  else
  {
    poffset = offs;
    offs = 0;
  }

  end = poffset + offs + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, segid, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)reg + offs, buff, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function copies a block of bytes from one segment to another.
 *
 * steps:
 *
 * 1) get the segment objects.
 * 2) map temporarily the two segments.
 * 3) do the copy.
 * 4) unmap the segments.
 */

t_error			ia32_segment_copy(i_region		dst,
					  t_paddr		offsd,
					  i_region		src,
					  t_paddr		offss,
					  t_psize		sz)
{
  i_region		regs;
  i_region		regd;
  o_segment*		segs;
  o_segment*		segd;
  t_paddr		poffset;
  t_paddr		end;

  /*
   * 1)
   */

  if (segment_get(dst, &segd) != ERROR_NONE ||
      segment_get(src, &segs) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offss % PAGESZ)
  {
    poffset = offss & ~(PAGESZ - 1);
    offss -= poffset;
  }
  else
  {
    poffset = offss;
    offss = 0;
  }

  end = poffset + offss + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, src, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &regs) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (offsd % PAGESZ)
  {
    poffset = offsd & ~(PAGESZ - 1);
    offsd -= poffset;
  }
  else
  {
    poffset = offsd;
    offsd = 0;
  }

  end = poffset + offsd + sz;
  if (end % PAGESZ)
    end = (end & ~(PAGESZ - 1)) + PAGESZ;

  if (region_reserve(kasid, dst, poffset, REGION_OPT_PRIVILEGED,
		     0, end - poffset, &regd) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)regd + offsd, (void*)(t_vaddr)regs + offss, sz);

  /*
   * 4)
   */

  if (region_release(kasid, regs) != ERROR_NONE ||
      region_release(kasid, regd) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}
