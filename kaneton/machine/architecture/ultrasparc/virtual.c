/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/mmu/mmu.c
 *
 * created       matthieu bucchianeri   [mon aug 21 19:50:05 2006]
 * updated       matthieu bucchianeri   [mon aug 28 15:20:00 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function enables the MMU.
 */

void			mmu_enable(void)
{
  t_uint64		cr;

  cr = LDXA(ASI_LSU_CONTROL, REG_LSU_CONTROL);
  cr |= ((1 << 2) | (1 << 3));
  STXA(ASI_LSU_CONTROL, REG_LSU_CONTROL, cr);
}

/*
 * this function disables the MMU.
 */

void			mmu_disable(void)
{
  t_uint64		cr;

  cr = LDXA(ASI_LSU_CONTROL, REG_LSU_CONTROL);
  cr &= ~((1 << 2) | (1 << 3));
  STXA(ASI_LSU_CONTROL, REG_LSU_CONTROL, cr);
}

/*
 * this function sets the (primary) context register.
 */

void			mmu_context(t_sparc64_context		context)
{
  STXA(ASI_IMMU, REG_PRIMARY_CONTEXT, context);
  STXA(ASI_IMMU, REG_SECONDARY_CONTEXT, context);
}

/*
 * XXX following memcpy functions are not optimal.
 */

/*
 * this function copy some data bypassing the MMU.  the bypass is done
 * when reading, meaning that "src" is a physical address.
 */

void			mmu_bypass_memcpy_r(void*		dst,
					    const void*		src,
					    t_uint64		size)
{
  const t_uint8*	s = src;
  t_uint8*		d = dst;
  t_uint64		i;

  for (i = 0; i < size; i++)
    {
      d[i] = LDUBA(ASI_BYPASS_MMU, s + i);
    }
}

/*
 * this function copy some data bypassing the MMU.  the bypass is done
 * when writing, meaning that "dst" is a physical address.
 */

void			mmu_bypass_memcpy_w(void*		dst,
					    const void*		src,
					    t_uint64		size)
{
  const t_uint8*	s = src;
  t_uint8*		d = dst;
  t_uint64		i;

  for (i = 0; i < size; i++)
    {
      STBA(ASI_BYPASS_MMU, d + i, s[i]);
    }
}

/*
 * this function copy some data bypassing the MMU.  the bypass is done
 * both when  writing, and reading  meaning that both "src"  and "dst"
 * are physical addresses.
 */

void			mmu_bypass_memcpy_rw(void*		dst,
					    const void*		src,
					    t_uint64		size)
{
  const t_uint8*	s = src;
  t_uint8*		d = dst;
  t_uint64		i;

  for (i = 0; i < size; i++)
    {
      STBA(ASI_BYPASS_MMU, d + i, LDUBA(ASI_BYPASS_MMU, s + i));
    }
}
