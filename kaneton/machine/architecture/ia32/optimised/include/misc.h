/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...itecture/ia32/educational/include/misc.h
 *
 * created       matthieu bucchianeri   [sat jun 16 19:29:07 2007]
 * updated       matthieu bucchianeri   [sat jun 16 19:29:50 2007]
 */

#ifndef ARCHITECTURE_MISC_H
#define ARCHITECTURE_MISC_H

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../cpuid.c
 *	../sse.c
 */

/*
 * ../cpuid.c
 */

t_uint32		ia32_cpu_is_genuine(void);

char*			ia32_cpu_get_brand(char*		brand,
					   int			n);

t_uint32		ia32_cpu_has_mmx(void);

t_uint32		ia32_cpu_has_sse(void);

t_uint32		ia32_cpu_has_sse2(void);

t_uint32		ia32_cpu_has_sse3(void);

t_uint32		ia32_cpu_has_fxstate(void);

t_uint32		ia32_cpu_has_apic(void);


/*
 * ../sse.c
 */

void			ia32_enable_sse(void);


/*
 * eop
 */

#endif
