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

#ifndef ARCHITECTURE_APIC_H
#define ARCHITECTURE_APIC_H

/*
 * ---------- defines ---------------------------------------------------------
 */

#define IA32_APIC_REG_BASE		0xFEE00000
#define IA32_IOAPIC_REG_SEL		0xFEC00000
#define IA32_IOAPIC_REG_DATA		0xFEC00010

/*
 * APIC registers.
 */

#define IA32_APIC_REG_APICID		0xFEE00020
#define IA32_APIC_REG_VERSION		0xFEE00030
#define IA32_APIC_REG_EOI		0xFEE000B0
#define IA32_APIC_REG_SVR		0xFEE000F0
#define IA32_APIC_REG_ICR_LOW		0xFEE00300
#define IA32_APIC_REG_ICR_HI		0xFEE00310
#define IA32_APIC_REG_TIMER_CONF	0xFEE00320
#define IA32_APIC_REG_TIMER		0xFEE00380
#define IA32_APIC_REG_COUNT		0xFEE00390
#define IA32_APIC_REG_DIV		0xFEE003E0

/*
 * IO-APIC registers.
 */

#define IA32_IOAPIC_REG_APICID		0x0
#define IA32_IOAPIC_REG_VERSION		0x1
#define IA32_IOAPIC_REG_ARBITRATION	0x2
#define IA32_IOAPIC_REG_TABLE		0x10

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../apic.c
 */

/*
 * ../apic.c
 */

void			ia32_apic_write(t_uint32		reg,
					t_uint32		value);

t_uint32		ia32_apic_read(t_uint32			reg);

t_uint32		ia32_apic_id(void);

void			ia32_apic_enable(void);

void			ia32_ioapic_write(t_uint32		reg,
					  t_uint32		value);

t_uint32		ia32_ioapic_read(t_uint32		reg);

void			ia32_ioapic_enable(void);


/*
 * eop
 */

#endif
