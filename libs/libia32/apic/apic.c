/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/apic/apic.c
 *
 * created       matthieu bucchianeri   [tue jul 25 15:56:15 2006]
 * updated       matthieu bucchianeri   [tue jul 25 16:00:00 2006]
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

void			apic_write(t_uint32			reg,
				   t_uint32			value)
{
  t_uint32*		addr = (t_uint32*)reg;

  *addr = value;
}

t_uint32		apic_read(t_uint32			reg)
{
  t_uint32*		addr = (t_uint32*)reg;

  return *addr;
}

t_uint32		apic_id(void)
{
  return apic_read(APIC_REG_APICID) >> 24;
}

void			apic_enable(void)
{
  apic_write(APIC_REG_SVR, apic_read(APIC_REG_SVR) | 0x100);
}
