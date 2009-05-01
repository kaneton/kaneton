/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kanet...achine/architecture/ia32/generic/local.c
 *
 * created       matthieu bucchianeri   [mon dec 10 13:54:28 2007]
 * updated       laurent lec   [sat mar 29 11:53:32 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * offers cpu-local global variables.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/* XXX: not used
static IA32_HANDLER_DATA_SECTION t_ia32_cpu_local*	cpu_local_area[IA32_CPU_MAX * IA32_CPU_LOCAL_SIZE];
static t_vaddr		cpu_local_current = 1;
*/

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		init;

/*
 * ---------- functions -------------------------------------------------------
 */

#if 0
void			ia32_cpu_local_reserve(t_ia32_cpu_local*	var)
{
  assert(cpu_local_current < IA32_CPU_LOCAL_SIZE);

  *var = cpu_local_current++;
}

void			ia32_cpu_local_set(t_ia32_cpu_local*	var,
					   t_uint32		value)
{
  t_uint32		id;

  if (*var == 0)
    ia32_cpu_local_reserve(var);

  id = ia32_apic_id();

  cpu_local_area[id * IA32_CPU_LOCAL_SIZE + *var] = value;
}

t_uint32		ia32_cpu_local_get(t_ia32_cpu_local	var)
{
  t_uint32		id;

  assert(var != 0 && var <= IA32_CPU_LOCAL_SIZE);
  assert(cpu_local_area != 0);

  id = ia32_apic_id();

  return (cpu_local_area[id * IA32_CPU_LOCAL_SIZE + *var]);
}
#endif

IA32_HANDLER_SECTION
void	ia32_cpu_local_set(t_ia32_cpu_local*	var,
						   t_uint32		value)
{
  *var = value;
}

IA32_HANDLER_SECTION
t_uint32	ia32_cpu_local_get(t_ia32_cpu_local	var)
{
  return (var);
}
