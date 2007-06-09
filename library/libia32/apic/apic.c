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

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * write an APIC register.
 */

void			apic_write(t_uint32			reg,
				   t_uint32			value)
{
  t_uint32*		addr = (t_uint32*)reg;

  *addr = value;
}

/*
 * read an APIC register.
 */

t_uint32		apic_read(t_uint32			reg)
{
  t_uint32*		addr = (t_uint32*)reg;

  return *addr;
}

/*
 * get the current CPU APIC ID.
 */

t_uint32		apic_id(void)
{
  return apic_read(APIC_REG_APICID) >> 24;
}

/*
 * enable the local APIC.
 */

void			apic_enable(void)
{
  apic_write(APIC_REG_SVR, apic_read(APIC_REG_SVR) | 0x100);
}
