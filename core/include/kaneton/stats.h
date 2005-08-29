/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * stats.h
 * 
 * path          /home/mycure/kaneton/core/kaneton/stats
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Mon Aug 29 13:57:32 2005   mycure
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
 * ---------- defines ---------------------------------------------------------
 */

#define STATS_STATE_NONE	(1 << 0)
#define STATS_STATE_FUNC	(1 << 1)

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

  t_state			state;

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

#define stats_check(_stats_)						\
  {									\
    if ((_stats_) == NULL)						\
      return (-1);							\
  }

/*
 * traps
 */

#if 1 //def CONF_STATS

#define STATS_BEGIN(_stats_)						\
  stats_begin((_stats_), __FUNCTION__)

//  stats_begin((_stats_), __FUNCTION__)

#define STATS_END(_stats_)						\

//  stats_end((_stats_), __FUNCTION__)

#else

#define STATS_BEGIN(_stats_)

#define STATS_END(_stats_)

#endif

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/stats/stats.c
 */

/*
 * ../../kaneton/stats/stats.c
 */

void			stats_begin(t_staid			staid,
				    char*			function);

void			stats_end(t_staid			staid,
				  char*				function);

int			stats_dump(void);

int			stats_add(void);

int			stats_remove(void);

int			stats_rsv(char*				name,
				  t_staid*			staid);

int			stats_rel(t_staid			staid);

int			stats_init(void);

int			stats_clean(void);


/*
 * eop
 */

#endif
