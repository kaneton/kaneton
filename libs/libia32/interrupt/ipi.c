/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/ipi.c
 *
 * created       matthieu bucchianeri   [tue jul 25 16:01:11 2006]
 * updated       matthieu bucchianeri   [mon aug 21 18:08:42 2006]
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

void			ipi_send_vector(t_uint8			vector,
					t_ia32_ipi_dest		dest)
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
    }
}

/*
 * sends EOI.
 */

void			ipi_acknowledge(void)
{
  apic_write(APIC_REG_EOI, 0);
}
