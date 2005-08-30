/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * stats.h
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Tue Aug 30 13:15:16 2005   mycure
 */

#ifndef KANETON_STATS_H
#define KANETON_STATS_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>
#include <kaneton/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * stats function
 *
 * the identifier is an hash of the name.
 */

typedef struct
{
  t_id				id;

  char*				name;

  t_uint32			calls;
  t_uint32			errors;

  /* XXX timer */
}				o_stats_func;

/*
 * stats object
 */

typedef struct
{
  t_staid			staid;

  char*				name;

  t_setid			functions;
}				o_stats;

/*
 * stats manager
 */

typedef struct
{
  o_id				id;

  t_setid			container;
}				m_stats;

/*
 * ---------- macros ----------------------------------------------------------
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

#define STATS_DUMP()							\
  stats_dump()

#define STATS_BEGIN(_stats_)						\
  stats_begin((_stats_), (char*)__FUNCTION__)

#define STATS_END(_stats_, _error_)					\
  stats_end((_stats_), (char*)__FUNCTION__, (_error_))

#define STATS_RSV(_name_, _staid_)					\
  stats_rsv((_name_), (_staid_))

#define STATS_REL(_staid_)						\
  stats_rel((_staid_))

#define STATS_INIT()							\
  stats_init()

#define STATS_CLEAN()							\
  stats_clean()

#else

#define STATS_DUMP()

#define STATS_BEGIN(_stats_)

#define STATS_END(_stats_, _error_)

#define STATS_RSV(_name_, _staid_)

#define STATS_REL(_staid_)

#define STATS_INIT()

#define STATS_CLEAN()

#endif

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/stats/stats.c
 */

/*
 * ../../kaneton/stats/stats.c
 */

t_error			stats_begin(t_staid			staid,
				    char*			function);

t_error			stats_end(t_staid			staid,
				  char*				function,
				  t_error			error);

t_error			stats_dump(void);

t_error			stats_rsv(char*				name,
				  t_staid*			staid);

t_error			stats_rel(t_staid			staid);

t_error			stats_init(void);

t_error			stats_clean(void);


/*
 * eop
 */

#endif
