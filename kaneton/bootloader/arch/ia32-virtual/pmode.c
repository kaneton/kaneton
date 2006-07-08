/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/bootloader/arch/ia32-virtual/pmode.c
 *
 * created       julien quintard   [mon jul 19 20:43:14 2004]
 * updated       julien quintard   [sat jul  8 02:31:27 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
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

/*                                                                  [cut] k1 */

/*
 * this  function initialises  the global  offset table  inserting two
 * default segments for the bootloader and kernel initialisation.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc..
 *
 * steps:
 *
 * 1) create a new gdt and enable it.
 * 2) create a new idt and enable it.
 * 3) set two segments for the rest of the operations.
 * 4) update the segments registers.
 * 5) finally install the protected mode.
 * 6) update the init structure.
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
  /*  if (idt_build(PMODE_IDT_ENTRIES,
		bootloader_init_alloc(PMODE_IDT_ENTRIES *
				      sizeof(t_ia32_idte), NULL),
		&idt, 1) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error creating idt.\n");
      bootloader_error();
    }

    if (idt_activate(idt) != ERROR_NONE)
      {
	bootloader_cons_msg('!', "pmode: error activating idt.\n");
	bootloader_error();
      }
  */

  /*
   * 3)
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
   * 4)
   */

  gdt_build_selector(PMODE_BOOTLOADER_CS, ia32_prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_BOOTLOADER_DS, ia32_prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

  pmode_enable();

  /*
   * 5)
   */

  memcpy(&init->machdep.gdt, &gdt, sizeof (t_ia32_gdt));

  /*  memcpy(&init->machdep.idt, &idt, sizeof (t_ia32_idt));*/
}

/*                                                                 [cut] /k1 */
