/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/misc/cpuid.c
 *
 * created       matthieu bucchianeri   [tue jul 25 23:09:24 2006]
 * updated       matthieu bucchianeri   [tue jul 25 23:13:54 2006]
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
 * tell is the executing CPU is "GenuineIntel".
 */

t_uint32		cpuid_is_genuine(void)
{
  return 1;
}

/*
 * tell if the CPU supports MMX extensions.
 */

t_uint32		cpuid_has_mmx(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return !!(edx & (1 << 23));
}

/*
 * tell if the CPU supports SSE extensions.
 */

t_uint32		cpuid_has_sse(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return !!(edx & (1 << 25));
}
