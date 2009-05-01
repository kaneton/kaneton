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

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * needed to convert nanoseconds to APIC timer ticks.
 */

static t_uint64		timeref = 0;

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

asm (".text						\n"
     "bootloader_apic_calibrate_tick:			\n"
     "	pusha						\n"
     "	addl $5, ticks					\n"
     "	pushl $0					\n"
     "	call pic_acknowledge				\n"
     "	addl $4, %esp					\n"
     "	popa						\n"
     "	iret						");

void	bootloader_apic_calibrate_tick(void);

/*
 * this function calibrates the APIC timer.
 *
 * steps:
 *
 * 1) setup the timer for one shot with a divisor equal to 128.
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

  apic_write(APIC_REG_DIV, 10);
  apic_write(APIC_REG_TIMER_CONF, 1 << 16);

  /*
   * 2)
   */

  apic_write(APIC_REG_TIMER, 1000000000);

  /*
   * 3)
   */

  gate.offset = (t_uint32)bootloader_apic_calibrate_tick;
  gate.segsel = IA32_PMODE_BOOTLOADER_CS << 3;
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

  timeref = (t1 - t2);
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

  total = (timeref * delay) / 100000;

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

/*
 * write an apic register.
 */

void			apic_write(t_uint32		reg,
				   t_uint32		value)
{
  t_uint32*		addr = (t_uint32*)reg;

  *addr = value;
}

/*
 * read an apic register.
 */

t_uint32		apic_read(t_uint32			reg)
{
  t_uint32*		addr = (t_uint32*)reg;

  return (*addr);
}

/*
 * get the current cpu apic id.
 */

t_uint32		apic_id(void)
{
  return (apic_read(APIC_REG_APICID) >> 24);
}

/*
 * enable the local apic.
 */

void			apic_enable(void)
{
  apic_write(APIC_REG_SVR, apic_read(APIC_REG_SVR) | 0x100);
}

/*
 * send a INIT IPI.
 */

void			ipi_send_init(void)
{
  apic_write(APIC_REG_ICR_HI, 0);
  apic_write(APIC_REG_ICR_LOW, 0xC4500);
}

/*
 * send a SIPI.
 */

void			ipi_send_startup(void)
{
  apic_write(APIC_REG_ICR_HI, 0);
  apic_write(APIC_REG_ICR_LOW, 0xC4608);
}

/*
 * send a vector IPI.
 */

void			ipi_send_vector(t_uint8		vector,
					t_ipi_dest	dest,
					t_id		cpu)
{
  switch (dest)
    {
      case ipi_all:
	apic_write(APIC_REG_ICR_HI, 0);
	apic_write(APIC_REG_ICR_LOW, (1 << 19) | (1 << 14) | vector);
	break;
      case ipi_all_but_me:
	apic_write(APIC_REG_ICR_HI, 0);
	apic_write(APIC_REG_ICR_LOW, (1 << 19) | (1 << 18) | (1 << 14) |
		   vector);
	break;
      case ipi_cpu:
	apic_write(APIC_REG_ICR_HI, (t_uint8)cpu << 24);
	apic_write(APIC_REG_ICR_LOW, (1 << 14) | vector);
	break;
    }
}

/*
 * sends EOI.
 */

void			ipi_acknowledge(void)
{
  apic_write(APIC_REG_EOI, 0);
}
