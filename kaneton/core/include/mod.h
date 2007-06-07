/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/mod.h
 *
 * created       julien quintard   [wed jun  6 13:39:48 2007]
 * updated       julien quintard   [thu jun  7 12:39:17 2007]
 */

/*
 * XXX
 *
 * this file is temporary. instead the bootloader should provide core
 * an already mapped and ready mod executable so that the core just creates
 * a task, set PC and SP and runs it.
 */

#ifndef CORE_MOD_H
#define CORE_MOD_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <core/elf.h>

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
