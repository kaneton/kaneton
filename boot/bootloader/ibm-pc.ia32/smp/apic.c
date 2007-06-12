/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-smp/apic.c
 *
 * created       matthieu bucchianeri   [tue jul 25 11:22:27 2006]
 * updated       matthieu bucchianeri   [wed mar 21 22:23:46 2007]
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
 * needed to convert nanoseconds to APIC timer ticks.
 */

static t_uint32		timeref = 0;

/*
 * counter used to calibrate.
 */

static volatile t_uint32 ticks;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * This is the PIT IRQ handler. Used to calibrate the APIC timer frequency.
 */

static void		bootloader_apic_calibrate_tick(void)
{
  asm volatile("push	%eax				\n"
	       "push	%ebx				\n"
	       "push	%ecx				\n"
	       "push	%edx				\n"
	       "push	%esi				\n"
	       "push	%edi				\n"
	       );

  ticks += 5;

  pic_acknowledge(0);

  asm volatile("pop	%edi				\n"
	       "pop	%esi				\n"
	       "pop	%edx				\n"
	       "pop	%ecx		  		\n"
	       "pop	%ebx				\n"
	       "pop	%eax				\n"
	       );

  LEAVE();
  IRET();
}

/*
 * this function calibrates the APIC timer.
 *
 * steps:
 *
 * 1) setup the timer for one shot with a divisor equal to 1.
 * 2) setup the initial count to a big number.
 * 3) wait for the sample amout of time.
 * 4) compute the time reference.
 */

void			bootloader_apic_calibrate_timer(void)
{
  t_ia32_gate		gate;
  t_sint32		t1;
  t_sint32		t2;

  /*
   * 1)
   */

  apic_write(APIC_REG_DIV, 11);
  apic_write(APIC_REG_TIMER_CONF, 1 << 16);

  /*
   * 2)
   */

  apic_write(APIC_REG_TIMER, 1000000000);

  /*
   * 3)
   */

  gate.offset = (t_uint32)bootloader_apic_calibrate_tick;
  gate.segsel = PMODE_GDT_CORE_CS << 3;
  gate.privilege = 0;
  gate.type = ia32_type_gate_interrupt;

  idt_add_gate(NULL, 32, gate);

  pit_init(200);

  pic_enable_irq(0);

  ticks = 0;
  t1 = apic_read(APIC_REG_COUNT);

  while (ticks < 100)
    ;

  pic_disable_irq(0);

  t2 = apic_read(APIC_REG_COUNT);

  /*
   * 4)
   */

  timeref = (t1 - t2) / (ticks * 1000);
}

/*
 * this function waits for the given amount of microseconds.
 *
 * steps:
 *
 * 1) compute the tick count.
 * 2) setup the timer.
 * 3) wait until the delay expires.
 */

void			bootloader_apic_wait(t_uint32			delay)
{
  t_uint64		total;

  /*
   * 1)
   */

  total = (delay * timeref) / 128;

  /*
   * 2)
   */

  apic_write(APIC_REG_DIV, 10);
  apic_write(APIC_REG_TIMER, (t_uint32)total);

  /*
   * 3)
   */

  while (apic_read(APIC_REG_COUNT))
    ;
}
