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

extern void bootloader_interrupt_dummy(void);

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
		1, &idt) != STATUS_OK)
    {
      bootloader_cons_msg('!', "interrupt: error creating idt.\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  if (idt_activate(&idt) != STATUS_OK)
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
      gate.segsel = IA32_PMODE_BOOTLOADER_CS << 3;
      gate.privilege = 0;
      gate.type = ia32_type_gate_interrupt;

      if (idt_add_gate(NULL, i, gate) != STATUS_OK)
	{
	  bootloader_cons_msg('!', "interrupt: error adding gate.\n");
	  bootloader_error();
	}
    }

  /*
   * 4)
   */

  if (pic_init() != STATUS_OK)
    {
      bootloader_cons_msg('!', "interrupt: error activating PIC.\n");
      bootloader_error();
    }

  asm volatile("sti");

  memcpy(&init->machine.idt, &idt, sizeof (t_ia32_idt));
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

  memcpy(&idt, &init->machine.idt, sizeof (t_ia32_idt));

  /*
   * 1)
   */

  if (idt_activate(&idt) != STATUS_OK)
    {
      bootloader_cons_msg('!', "interrupt: error activating idt.\n");
      bootloader_error();
    }

  asm volatile("sti");
}

/*
 * dummy interrupt handler.
 */

static const char* __attribute__((unused)) spurious_interrupt = "spurious interrupt on %d!\n";

asm (".text					\n"
     "bootloader_interrupt_dummy:		\n"
     "	pusha					\n"
     "	call apic_id				\n"
     "	pushl %eax				\n"
     "	pushl $spurious_interrupt		\n"
     "	call printf				\n"
     "	addl $8, %esp				\n"
     "	call ipi_acknowledge			\n"
     "	popa					\n"
     "	iret					");
