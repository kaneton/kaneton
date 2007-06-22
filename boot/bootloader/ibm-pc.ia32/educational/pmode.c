/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/pmode.c
 *
 * created       julien quintard   [mon jul 19 20:43:14 2004]
 * updated       matthieu bucchianeri   [tue feb  6 19:17:11 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include "bootloader.h"

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

/*
 * this  function initializes  the global  offset table  inserting two
 * default segments for the bootloader and kernel initialization.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc.
 *
 * steps:
 *
 * 1) create a new gdt and enable it.
 * 2) set two segments for the rest of the operations.
 * 3) update the segments registers.
 * 4) enable caching and write-back (since we are in uniprocessor mode).
 * 5) update the init structure.
 */

void			bootloader_pmode_init(void)
{
  t_ia32_gdt		gdt;
  t_ia32_segment	seg;
  t_uint16		kcs;
  t_uint16		kds;

  /*
   * 1)
   */

  if (gdt_build(PMODE_GDT_ENTRIES,
		bootloader_init_alloc(PMODE_GDT_ENTRIES *
				      sizeof(t_ia32_gdte), NULL),
		&gdt, 1) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error creating gdt.\n");
      bootloader_error();
    }

  if (gdt_activate(gdt) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error activating gdt.\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_code;
  if (gdt_add_segment(NULL, PMODE_BOOTLOADER_CS, seg) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error creating main code segment.\n");
      bootloader_error();
    }

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = ia32_type_data;
  if (gdt_add_segment(NULL, PMODE_BOOTLOADER_DS, seg) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error creating main data segment.\n");
      bootloader_error();
    }

  /*
   * 3)
   */

  gdt_build_selector(PMODE_BOOTLOADER_CS, ia32_prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_BOOTLOADER_DS, ia32_prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

  pmode_enable();

  /*
   * 4)
   */

  WBINVD();

  asm volatile("movl %cr0, %eax\n\t"
	       "andl $0x3FFFFFFF, %eax\n\t"
	       "movl %eax, %cr0");

  /*
   * 5)
   */

  memcpy(&init->machdep.gdt, &gdt, sizeof (t_ia32_gdt));
}
