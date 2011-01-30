/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/misc/cpuid.c
 *
 * created       matthieu bucchianeri   [tue jul 25 23:09:24 2006]
 * updated       matthieu bucchianeri   [wed mar 21 22:45:19 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * CPUID instruction.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * tell is the executing cpu is "GenuineIntel".
 */

t_uint32		ia32_cpu_is_genuine(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;
  t_uint32		buff[4];

  memset(buff, 0, sizeof (buff));
  CPUID(0x0, eax, ebx, ecx, edx);

  buff[0] = ebx;
  buff[1] = edx;
  buff[2] = ecx;

  return (!strcmp((char*)buff, "GenuineIntel"));
}

/*
 * return the brand string.
 */

char*			ia32_cpu_get_brand(char*		brand,
					   int			n)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;
  t_uint32		buff[12];
  t_uint32		i;

  CPUID(0x80000000, eax, ebx, ecx, edx);

  if (!(eax & 0x80000000) || eax < 0x80000004)
    return (strncpy(brand, "Unknown", n));

  memset(buff, 0, sizeof (buff));

  for (i = 0; i < 3; i++)
    {
      CPUID(0x80000002 + i, eax, ebx, ecx, edx);

      buff[i * 4 + 0] = eax;
      buff[i * 4 + 1] = ebx;
      buff[i * 4 + 2] = ecx;
      buff[i * 4 + 3] = edx;
    }
  return (strncpy(brand, (char*)buff, n));
}

/*
 * tell if the CPU supports MMX extensions.
 */

t_uint32		ia32_cpu_has_mmx(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(edx & (1 << 23)));
}

/*
 * tell if the CPU supports SSE extensions.
 */

t_uint32		ia32_cpu_has_sse(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(edx & (1 << 25)));
}

/*
 * tell if the CPU supports SSE2 extensions.
 */

t_uint32		ia32_cpu_has_sse2(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(edx & (1 << 26)));
}

/*
 * tell if the CPU supports SSE3 extensions.
 */

t_uint32		ia32_cpu_has_sse3(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(ecx & (1 << 0)));
}

/*
 * tell if the CPU supports FXSAVE & FXRSTOR instructions.
 */

t_uint32		ia32_cpu_has_fxstate(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(edx & (1 << 24)));
}

/*
 * tell if the CPU embed a local APIC
 */

t_uint32		ia32_cpu_has_apic(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(edx & (1 << 9)));
}
