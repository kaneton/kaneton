/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/bootloader/arch/ia32-virtual/pmode.c
 *
 * created       julien quintard   [mon jul 19 20:43:14 2004]
 * updated       matthieu bucchianeri   [mon jan  9 18:23:21 2006]
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the student just has to setup the protected mode in this file,
 * nothing more.
 *
 * of course, it will be better to provide some debug functions etc..
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

#define PMODE_GDT_ENTRIES	256

#define PMODE_GDT_CORE_CS	0x1
#define PMODE_GDT_CORE_DS	0x2
#define PMODE_GDT_DRIVER_CS	0x3
#define PMODE_GDT_DRIVER_DS	0x4
#define PMODE_GDT_SERVICE_CS	0x5
#define PMODE_GDT_SERVICE_DS	0x6
#define PMODE_GDT_PROGRAM_CS	0x7
#define PMODE_GDT_PROGRAM_DS	0x8

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k1 */

/*
 * this function initialises the global offset table inserting
 * height entries for the core, drivers, services and programs.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc..
 *
 * steps:
 *
 * 1) creates a new table and enable it.
 * 2) sets the height segments for the core, driver, service and program.
 * 3) updates the segments registers.
 * 4) finally installs the protected mode.
 * 5) sets the global offset table address in the init variable.
 * 6) updates the init structure.
 */

void			bootloader_pmode_init(void)
{
  t_gdt			gdt;
  t_gdtr		gdtr;
  t_segment		seg;
  t_uint16		kcs;
  t_uint16		kds;

  /*
   * 1)
   */

  gdt_build(PMODE_GDT_ENTRIES, bootloader_init_alloc(PMODE_GDT_ENTRIES *
						     sizeof(t_gdte), NULL),
	    &gdt, 1);

  gdt_activate(gdt);

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_code;
  gdt_add_segment(NULL, PMODE_GDT_CORE_CS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_data;
  gdt_add_segment(NULL, PMODE_GDT_CORE_DS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_code;
  gdt_add_segment(NULL, PMODE_GDT_DRIVER_CS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_data;
  gdt_add_segment(NULL, PMODE_GDT_DRIVER_DS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_code;
  gdt_add_segment(NULL, PMODE_GDT_SERVICE_CS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_data;
  gdt_add_segment(NULL, PMODE_GDT_SERVICE_DS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_code;
  gdt_add_segment(NULL, PMODE_GDT_PROGRAM_CS, seg);

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_data;
  gdt_add_segment(NULL, PMODE_GDT_PROGRAM_DS, seg);

  /*
   * 3)
   */

  gdt_build_selector(PMODE_GDT_CORE_CS, prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_GDT_CORE_DS, prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

  /*
   * 4)
   */

  pmode_enable();

  /*
   * 5)
   */

  SGDT(gdtr); //XXX

  /*
   * 6)
   */

  memcpy(&init->machdep.gdt, &gdt, sizeof (t_gdt));
}

/*                                                                 [cut] /k1 */
