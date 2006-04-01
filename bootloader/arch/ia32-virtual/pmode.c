/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/bootloader/arch/ia32-virtual/pmode.c
 *
 * created       julien quintard   [mon jul 19 20:43:14 2004]
 * updated       julien quintard   [fri mar 10 03:43:45 2006]
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

/*                                                                  [cut] k1 */

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PMODE_GDT_ENTRIES	256
#define PMODE_IDT_ENTRIES	256

#define PMODE_BOOTLOADER_CS	0x1
#define PMODE_BOOTLOADER_DS	0x2

/*                                                                 [cut] /k1 */

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
  t_gdt			gdt;
/*                                                                 [cut] /k1 */
/*                                                                  [cut] k3 */
  t_idt			idt;
/*                                                                 [cut] /k3 */
/*                                                                  [cut] k1 */
  t_segment		seg;
  t_uint16		kcs;
  t_uint16		kds;

  /*
   * 1)
   */

  if (gdt_build(PMODE_GDT_ENTRIES,
		bootloader_init_alloc(PMODE_GDT_ENTRIES *
				      sizeof(t_gdte), NULL),
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

/*                                                                 [cut] /k1 */
/*                                                                  [cut] k3 */
  /*
   * 2)
   */
  if (idt_build(PMODE_IDT_ENTRIES,
		bootloader_init_alloc(PMODE_IDT_ENTRIES *
				      sizeof(t_idte), NULL),
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

/*                                                                 [cut] /k3 */
/*                                                                  [cut] k1 */


  /*
   * 3)
   */

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_code;
  if (gdt_add_segment(NULL, PMODE_BOOTLOADER_CS, seg) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error creating main code segment.\n");
      bootloader_error();
    }

  seg.base = 0;
  seg.limit = 0xffffffff;
  seg.privilege = prvl_supervisor;
  seg.is_system = 0;
  seg.type.usr = type_data;
  if (gdt_add_segment(NULL, PMODE_BOOTLOADER_DS, seg) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "pmode: error creating main data segment.\n");
      bootloader_error();
    }

  /*
   * 4)
   */

  gdt_build_selector(PMODE_BOOTLOADER_CS, prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_BOOTLOADER_DS, prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

/*                                                                 [cut] /k1 */
/*                                                                  [cut] k3 */

  /*
   * 5)
   */

  pic_init();

/*                                                                 [cut] /k3 */
/*                                                                  [cut] k1 */

  pmode_enable();

  /*
   * 6)
   */

  memcpy(&init->machdep.gdt, &gdt, sizeof (t_gdt));
/*                                                                 [cut] /k1 */
/*                                                                  [cut] k3 */
  memcpy(&init->machdep.idt, &idt, sizeof (t_idt));
/*                                                                 [cut] /k3 */
/*                                                                  [cut] k1 */
}

/*                                                                 [cut] /k1 */
