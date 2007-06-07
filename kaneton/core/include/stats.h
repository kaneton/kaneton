/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/stats.h
 *
 * created       julien quintard   [wed jun  6 13:00:10 2007]
 * updated       julien quintard   [thu jun  7 12:09:57 2007]
 */

#ifndef CORE_STATS_H
#define CORE_STATS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define STATS_MANAGERS_INITSZ	0x8
#define STATS_FUNCTIONS_INITSZ	0x4

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

  t_uint32			timer_start;
  t_uint32			total;
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
  t_stats*			managers;
  t_sint64			managerssz;
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
 *      ../../core/stats/stats.c
 */

/*
 * ../../core/stats/stats.c
 */

t_error			stats_function(i_stats			staid,
				       char*			function,
				       t_stats_func**		f);

t_error			stats_begin(i_stats			staid,
				    char*			function);

t_error			stats_end(i_stats			staid,
				  char*				function,
				  t_error			error);

t_error			stats_show(i_stats			staid);

t_error			stats_dump(void);

t_error			stats_reserve(char*			name,
				      i_stats*			staid);

t_error			stats_release(i_stats			staid);

t_error			stats_init(void);

t_error			stats_clean(void);


/*
 * eop
 */

#endif
