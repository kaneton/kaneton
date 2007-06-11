/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...include/arch/ibm-pc.ia32-virtual/stats.h
 *
 * created       julien quintard   [wed jun  6 16:25:31 2007]
 * updated       julien quintard   [thu jun  7 15:29:45 2007]
 */

#ifndef ARCHITECTURE_STATS_H
#define ARCHITECTURE_STATS_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_stats()

#define		machine_call_stats(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      _r_;								\
    }									\
  )

#define		machine_data_stats()

#endif
