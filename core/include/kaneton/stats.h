/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/stats.h
 *
 * created       julien quintard   [sun jan 19 14:51:33 2005]
 * updated       julien quintard   [fri mar 10 04:00:26 2006]
 */

#ifndef KANETON_STATS_H
#define KANETON_STATS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>
#include <kaneton/types.h>
#include <kaneton/conf.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define STATS_CONTAINER_INITSZ	0x8
#define STATS_FUNCTIONS_INITSZ	0x1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * stats function
 */

typedef struct
{
  char*				name;

  t_uint64			calls;
  t_uint64			errors;

  /* XXX timer */
}				t_stats_func;

/*
 * stats object
 */

typedef struct
{
  char*				name;

  t_stats_func*			functions;
  t_sint64			functionssz;
}				t_stats;

/*
 * stats manager
 */

typedef struct
{
  t_stats*			container;
  t_sint64			containersz;
}				m_stats;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define STATS_CHECK(_stats_)						\
  {									\
    if ((_stats_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define STATS_ENTER(_stats_)						\
  {									\
    STATS_CHECK((_stats_));						\
  }

/*
 * leave
 */

#define STATS_LEAVE(_stats_, _error_)					\
  {									\
    return (_error_);							\
  }

/*
 * traps
 */

#ifdef CONF_STATS

# define STATS_DUMP()							\
  stats_dump()

# define STATS_BEGIN(_stats_)						\
  stats_begin((_stats_), (char*)__FUNCTION__)

# define STATS_END(_stats_, _error_)					\
  stats_end((_stats_), (char*)__FUNCTION__, (_error_))

# define STATS_RESERVE(_name_, _staid_)					\
  stats_reserve((_name_), (_staid_))

# define STATS_RELEASE(_staid_)						\
  stats_release((_staid_))

# define STATS_INIT()							\
  stats_init()

# define STATS_CLEAN()							\
  stats_clean()

#else

# define STATS_DUMP()							\
  ERROR_NONE

# define STATS_BEGIN(_stats_)						\
  ERROR_NONE

# define STATS_END(_stats_, _error_)					\
  ERROR_NONE

# define STATS_RESERVE(_name_, _staid_)					\
  ERROR_NONE

# define STATS_RELEASE(_staid_)						\
  ERROR_NONE

# define STATS_INIT()							\
  ERROR_NONE

# define STATS_CLEAN()							\
  ERROR_NONE

#endif

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/stats/stats.c
 */

/*
 * ../../kaneton/stats/stats.c
 */

t_error			stats_function(t_staid			staid,
				       char*			function,
				       t_stats_func**		f);

t_error			stats_begin(t_staid			staid,
				    char*			function);

t_error			stats_end(t_staid			staid,
				  char*				function,
				  t_error			error);

t_error			stats_show(t_staid			staid);

t_error			stats_dump(void);

t_error			stats_reserve(char*			name,
				      t_staid*			staid);

t_error			stats_release(t_staid			staid);

t_error			stats_init(void);

t_error			stats_clean(void);


/*
 * eop
 */

#endif
