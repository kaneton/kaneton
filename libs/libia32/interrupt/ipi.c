/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/ipi.c
 *
 * created       matthieu bucchianeri   [tue jul 25 16:01:11 2006]
 * updated       matthieu bucchianeri   [tue jul 25 16:03:33 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage the InterProcessor Interrupts.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void			ipi_send_init(void)
{
  apic_write(APIC_REG_ICR_HI, 0);
  apic_write(APIC_REG_ICR_LOW, 0xC4500);
}

void			ipi_send_startup(void)
{
  apic_write(APIC_REG_ICR_HI, 0);
  apic_write(APIC_REG_ICR_LOW, 0xC4608);
}
