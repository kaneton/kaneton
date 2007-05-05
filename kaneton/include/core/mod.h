/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/mod.h
 *
 * created       matthieu bucchianeri   [sat may  5 18:46:33 2007]
 * updated       matthieu bucchianeri   [sat may  5 18:48:14 2007]
 */

#ifndef CORE_MOD_H
#define CORE_MOD_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>
#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * mod manager
 */

typedef struct
{
  i_stats	stats;
}		m_mod;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define MOD_CHECK(_mod_)						\
  {									\
    if ((_mod_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define MOD_ENTER(_mod_)						\
  {									\
    MOD_CHECK((_mod_));							\
    									\
    STATS_BEGIN((_mod_)->stats);					\
  }

/*
 * leave
 */

#define MOD_LEAVE(_mod_, _error_)					\
  {									\
    STATS_END((_mod_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/mod/mod.c
 */

/*
 * ../../core/mod/mod.c
 */

t_error			mod_init(void);

t_error			mod_clean(void);


/*
 * eop
 */

#endif
