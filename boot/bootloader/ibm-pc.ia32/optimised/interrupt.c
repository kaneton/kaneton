/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ia32-smp/interrupt.c
 *
 * created       matthieu bucchianeri   [mon aug 21 14:44:03 2006]
 * updated       matthieu bucchianeri   [mon aug 21 18:29:28 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*			init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initialize interrupts for the boot processor.
 *
 * steps:
 *
 * 1) build an IDT.
 * 2) load it.
 * 3) setup default handlers.
 * 4) setup the PIC and activate interrupts.
 */

void			bootloader_interrupt_init(void)
{
  t_ia32_idt		idt;
  t_ia32_gate		gate;
  int			i;

  /*
   * 1)
   */

  if (idt_build(256,
		bootloader_init_alloc(256 *
				      sizeof(t_ia32_idte), NULL),
		1, &idt) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "interrupt: error creating idt.\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  if (idt_activate(&idt) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "interrupt: error activating idt.\n");
      bootloader_error();
    }

  /*
   * 3)
   */

  for (i = 0; i < 256; i++)
    {
      gate.offset = (t_uint32)bootloader_interrupt_dummy;
      gate.segsel = PMODE_GDT_CORE_CS << 3;
      gate.privilege = 0;
      gate.type = ia32_type_gate_interrupt;

      if (idt_add_gate(NULL, i, gate) != ERROR_NONE)
	{
	  bootloader_cons_msg('!', "interrupt: error adding gate.\n");
	  bootloader_error();
	}
    }

  /*
   * 4)
   */

  if (pic_init() != ERROR_NONE)
    {
      bootloader_cons_msg('!', "interrupt: error activating PIC.\n");
      bootloader_error();
    }

  STI();

  memcpy(&init->machdep.idt, &idt, sizeof (t_ia32_idt));
}

/*
 * this function init interrupts for an AP.
 *
 * steps:
 *
 * 1) load the IDT an activate interrupts.
 */

void			bootloader_interrupt_ap_init(void)
{
  t_ia32_idt		idt;

  memcpy(&idt, &init->machdep.idt, sizeof (t_ia32_idt));

  /*
   * 1)
   */

  if (idt_activate(&idt) != ERROR_NONE)
    {
      bootloader_cons_msg('!', "interrupt: error activating idt.\n");
      bootloader_error();
    }

  STI();
}

/*
 * dummy interrupt handler.
 */

void			bootloader_interrupt_dummy(void)
{
  printf("dummy on %d!\n", apic_id());

  ipi_acknowledge();

  LEAVE();
  IRET();
}
