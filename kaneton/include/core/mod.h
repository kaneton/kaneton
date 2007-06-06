/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/mod.h
 *
 * created       julien quintard   [wed jun  6 13:39:48 2007]
 * updated       julien quintard   [wed jun  6 13:59:39 2007]
 */

#ifndef GUARD_CORE_MOD
#define GUARD_CORE_MOD			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/stats.h>
#include <core/error.h>

#include <sys/elf.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * mod manager
 */

typedef struct
{
  i_stats			stats;
}				m_mod;

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

t_error			mod_load_32(Elf32_Ehdr*		header,
				    char*		name);

t_error			mod_load_64(Elf64_Ehdr*		header,
				    char*		name);

t_error			mod_launch(void);

t_error			mod_init(void);

t_error			mod_clean(void);


/*
 * eop
 */

#endif
