/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/include/arch/machdep/core/stats.h
 *
 * created       julien quintard   [wed jun  6 16:25:31 2007]
 * updated       julien quintard   [wed jun  6 16:26:04 2007]
 */

#ifndef GUARD_IA32_CORE_STATS
#define GUARD_IA32_CORE_STATS		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_stats()

#define		machdep_call_stats(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_stats()

#endif
