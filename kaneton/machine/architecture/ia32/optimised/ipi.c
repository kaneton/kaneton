/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry.../machine/architecture/ia32/generic/ipi.c
 *
 * created       matthieu bucchianeri   [sat dec 22 21:53:19 2007]
 * updated       matthieu bucchianeri   [wed jan  9 11:23:21 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * send interprocessor-interrupts.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * send a INIT IPI.
 */

void			ia32_ipi_send_init(void)
{
  ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
  ia32_apic_write(IA32_APIC_REG_ICR_LOW, 0xC4500);
}

/*
 * send a SIPI.
 */

void			ia32_ipi_send_startup(void)
{
  ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
  ia32_apic_write(IA32_APIC_REG_ICR_LOW, 0xC4608);
}

/*
 * send a vector IPI.
 */

void			ia32_ipi_send_vector(t_uint8		vector,
					     t_ia32_ipi_dest	dest,
					     i_cpu		cpu)
{
  switch (dest)
    {
      case ipi_all:
	ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
	ia32_apic_write(IA32_APIC_REG_ICR_LOW, (1 << 19) | (1 << 14) | vector);
	break;
      case ipi_all_but_me:
	ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
	ia32_apic_write(IA32_APIC_REG_ICR_LOW, (1 << 19) | (1 << 18) | (1 << 14) |
		   vector);
	break;
      case ipi_cpu:
	ia32_apic_write(IA32_APIC_REG_ICR_HI, (t_uint8)cpu << 24);
	ia32_apic_write(IA32_APIC_REG_ICR_LOW, (1 << 14) | vector);
	break;
    }
}

/*
 * sends EOI.
 */

void			ia32_ipi_acknowledge(void)
{
  ia32_apic_write(IA32_APIC_REG_EOI, 0);
}
