/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/virtual.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [fri jun 15 09:31:08 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just initialises the machine-dependent segment manager.
 *
 * steps:
 *
 * 1) insert code and data segments for kernel task.
 * 2) insert code and data segments for drivers.
 * 3) insert code and data segments for services.
 * 4) insert code and data segments for userland programs.
 * 5) update segment selector registers.
 * 6) setup the segment selector for interrupts.
 */

t_error			ia32_segmentation_init(void)
{
  t_ia32_segment	seg;
  t_uint16		kcs;
  t_uint16		kds;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_CORE_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_CORE_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_DRIVER_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_DRIVER_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_SERVICE_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_SERVICE_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_PROGRAM_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_user;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (ia32_gdt_add_segment(NULL, IA32_PMODE_GDT_PROGRAM_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_CS, ia32_prvl_supervisor, &kcs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_DS, ia32_prvl_supervisor, &kds);
  ia32_pmode_set_segment_registers(kcs, kds);

  /*
   * 6)
   */

  interrupt_ds = kds;

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

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

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

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
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy(buff, (void*)(t_vaddr)reg + offs, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
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

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

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
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)reg + offs, buff, sz);

  /*
   * 4)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
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

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(dst, &segd) != ERROR_NONE ||
      segment_get(src, &segs) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

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
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

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
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy((void*)(t_vaddr)regd + offsd, (void*)(t_vaddr)regs + offss, sz);

  /*
   * 4)
   */

  if (region_release(kasid, regs) != ERROR_NONE ||
      region_release(kasid, regd) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}
