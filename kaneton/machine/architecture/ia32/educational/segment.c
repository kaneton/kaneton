/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../architecture/ia32/educational/segment.c
 *
 * created       matthieu bucchianeri   [mon dec 24 19:25:22 2007]
 * updated       julien quintard   [mon nov 22 21:53:53 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * segments operations on ia32.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * kernel manager.
 */

extern m_kernel*	_kernel;

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

  if (segment_get(segid, &o) != ERROR_OK)
    return (ERROR_KO);

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

  if (region_reserve(_kernel->as,
		     segid,
		     poffset,
		     REGION_OPTION_PRIVILEGED,
		     0,
		     end - poffset,
		     &reg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  memcpy(buff, (void*)(t_vaddr)reg + offs, sz);

  /*
   * 4)
   */

  if (region_release(_kernel->as, reg) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
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

  if (segment_get(segid, &o) != ERROR_OK)
    return (ERROR_KO);

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

  if (region_reserve(_kernel->as,
		     segid,
		     poffset,
		     REGION_OPTION_PRIVILEGED,
		     0,
		     end - poffset,
		     &reg) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)reg + offs, buff, sz);

  /*
   * 4)
   */

  if (region_release(_kernel->as, reg) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
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

  if (segment_get(dst, &segd) != ERROR_OK ||
      segment_get(src, &segs) != ERROR_OK)
    return (ERROR_KO);

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

  if (region_reserve(_kernel->as,
		     src,
		     poffset,
		     REGION_OPTION_PRIVILEGED,
		     0,
		     end - poffset,
		     &regs) != ERROR_OK)
    return (ERROR_KO);

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

  if (region_reserve(_kernel->as,
		     dst,
		     poffset,
		     REGION_OPTION_PRIVILEGED,
		     0,
		     end - poffset,
		     &regd) != ERROR_OK)
    return (ERROR_KO);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)regd + offsd, (void*)(t_vaddr)regs + offss, sz);

  /*
   * 4)
   */

  if (region_release(_kernel->as, regs) != ERROR_OK ||
      region_release(_kernel->as, regd) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}
