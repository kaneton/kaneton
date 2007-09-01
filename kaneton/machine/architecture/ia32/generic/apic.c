/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/apic/apic.c
 *
 * created       matthieu bucchianeri   [tue jul 25 15:56:15 2006]
 * updated       matthieu bucchianeri   [tue jul 25 23:10:20 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage the Advanced Programmable Interrupt Controlle (APIC).
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * write an apic register.
 */

void			ia32_apic_write(t_uint32		reg,
					t_uint32		value)
{
  t_uint32*		addr = (t_uint32*)reg;

  *addr = value;
}

/*
 * read an apic register.
 */

t_uint32		ia32_apic_read(t_uint32			reg)
{
  t_uint32*		addr = (t_uint32*)reg;

  return (*addr);
}

/*
 * get the current cpu apic id.
 */

t_uint32		ia32_apic_id(void)
{
  return (ia32_apic_read(IA32_APIC_REG_APICID) >> 24);
}

/*
 * enable the local apic.
 */

void			ia32_apic_enable(void)
{
  ia32_apic_write(IA32_APIC_REG_SVR,
		  ia32_apic_read(IA32_APIC_REG_SVR) | 0x100);
}
