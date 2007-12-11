/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...rchitecture/ia32/generic/include/local.h
 *
 * created       matthieu bucchianeri   [mon dec 10 13:51:18 2007]
 * updated       matthieu bucchianeri   [mon dec 10 14:06:16 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * offers cpu-local global variables.
 */

#ifndef ARCHITECTURE_LOCAL_H
#define ARCHITECTURE_LOCAL_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * number of variables.
 */

#define	IA32_CPU_LOCAL_SIZE	10

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * cpu-local variable reference.
 */

typedef t_vaddr	t_ia32_cpu_local;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../local.c
 */

/*
 * ../local.c
 */

void			ia32_cpu_local_reserve(t_ia32_cpu_local*	var);

void			ia32_cpu_local_set(t_ia32_cpu_local*	var,
					   t_uint32		value);

t_uint32		ia32_cpu_local_get(t_ia32_cpu_local	var);

void	ia32_cpu_local_set(t_ia32_cpu_local*	var,
						   t_uint32		value);

t_uint32	ia32_cpu_local_get(t_ia32_cpu_local	var);


/*
 * eop
 */

#endif
