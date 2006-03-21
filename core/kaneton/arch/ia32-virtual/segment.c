/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue mar 21 12:02:26 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  file implements dependent  code for  segment manager  on ia32
 * with paging architecture.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students will have to develop this entire part of the project. take
 * a look at the interface  structure declaration to fill it correctly
 * and to write needed functions.
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
extern t_asid		kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager interface.
 */

i_segment		segment_interface =
  {

/*                                                                  [cut] k2 */

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_segment_read,
    ia32_segment_write,
    ia32_segment_copy,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_segment_init,
    ia32_segment_clean

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function reads directly from a segment to a buffer.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) check offset and size.
 * 3) map the segment portion with a region.
 * 4) copy from segment to the buffer.
 * 5) unmap the region.
 */

t_error			ia32_segment_read(t_segid		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz)
{
  o_segment*		o;
  t_regid		reg;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs + sz > o->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (region_reserve(kasid, segid, offs, REGION_OPT_NONE, 0, sz, &reg) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  memcpy(buff, (void*)(t_vaddr)reg, sz);

  /*
   * 5)
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
 * 2) check offset and size.
 * 3) map the segment portion with a region.
 * 4) copy from the buffer to the segment.
 * 5) unmap the region.
 */

t_error			ia32_segment_write(t_segid		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz)
{
  o_segment*		o;
  t_regid		reg;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (segment_get(segid, &o) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (offs + sz > o->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (region_reserve(kasid, segid, offs, REGION_OPT_NONE, 0, sz, &reg) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  memcpy((void*)(t_vaddr)reg, buff, sz);

  /*
   * 5)
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
 * 2) check for offsets and size.
 * 3) map temporarily the two segments.
 * 4) do the copy.
 * 5) unmap the segments.
 */

t_error			ia32_segment_copy(t_segid		dst,
					  t_paddr		offsd,
					  t_segid		src,
					  t_paddr		offss,
					  t_psize		sz)
{
  t_regid		regs;
  t_regid		regd;
  o_segment*		segs;
  o_segment*		segd;

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

  if (offsd + sz > segd->size || offss + sz > segs->size)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (region_reserve(kasid, src, offss, REGION_OPT_NONE, 0, sz, &regs) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
  if (region_reserve(kasid, dst, offsd, REGION_OPT_NONE, 0, sz, &regd) !=
      ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  memcpy((void*)(t_vaddr)regd, (void*)(t_vaddr)regs, sz);

  /*
   * 5)
   */

  if (region_release(kasid, regs) != ERROR_NONE ||
      region_release(kasid, regd) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function just initialises the machine-dependent segment manager.
 *
 * steps:
 *
 * 1) initialise protected-mode.
 * 2) insert code and data segments for kernel task.
 * 3) insert code and data segments for drivers.
 * 4) insert code and data segments for services.
 * 5) insert code and data segments for userland programs.
 * 6) update segment selector registers.
 */

t_error			ia32_segment_init(t_fit			fit)
{
  t_segment		seg;
  t_uint16		kcs;
  t_uint16		kds;

  SEGMENT_ENTER(segment);

  /*
   * 1)
   */

  if (pmode_init() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_CORE_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_CORE_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_DRIVER_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_DRIVER_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 4)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_SERVICE_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_privileged;
  seg.is_system = 0;
  seg.type.usr = type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_SERVICE_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 5)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_user;
  seg.is_system = 0;
  seg.type.usr = type_code;
  if (gdt_add_segment(NULL, PMODE_GDT_PROGRAM_CS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_user;
  seg.is_system = 0;
  seg.type.usr = type_data;
  if (gdt_add_segment(NULL, PMODE_GDT_PROGRAM_DS, seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 6)
   */

  gdt_build_selector(PMODE_GDT_CORE_CS, prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_GDT_CORE_DS, prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

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

/*                                                                 [cut] /k2 */
