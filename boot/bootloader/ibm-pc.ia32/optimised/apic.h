/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/apic/apic.h
 *
 * created       matthieu bucchianeri   [tue jul 25 16:05:01 2006]
 * updated       matthieu bucchianeri   [mon aug 21 15:40:33 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * apic macros.
 */

#ifndef BOOTLOADER_APIC_H
#define BOOTLOADER_APIC_H

/*
 * ---------- defines ---------------------------------------------------------
 */

#define APIC_REG_BASE		0xFEE00000

/*
 * APIC registers.
 */

#define APIC_REG_APICID		0xFEE00020
#define APIC_REG_VERSION	0xFEE00030
#define APIC_REG_EOI		0xFEE000B0
#define APIC_REG_SVR		0xFEE000F0
#define APIC_REG_ICR_LOW	0xFEE00300
#define APIC_REG_ICR_HI		0xFEE00310
#define APIC_REG_TIMER_CONF	0xFEE00320
#define APIC_REG_TIMER		0xFEE00380
#define APIC_REG_COUNT		0xFEE00390
#define APIC_REG_DIV		0xFEE003E0

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * enumerate ipi possible destinations.
 */

typedef enum
  {
	ipi_all,
	ipi_all_but_me,
	ipi_cpu
  }	t_ipi_dest;

#endif
