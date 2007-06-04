/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/mod.h
 *
 * created       matthieu bucchianeri   [sat may  5 18:46:33 2007]
 * updated       matthieu bucchianeri   [sat may  5 20:40:09 2007]
 */

#ifndef CORE_MOD_H
#define CORE_MOD_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct sm_mod		m_mod;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <sys/elf.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * mod manager
 */

struct				sm_mod
{
  i_stats			stats;
};

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
