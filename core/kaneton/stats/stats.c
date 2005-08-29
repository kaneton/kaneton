/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * stats.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/stats
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Aug 29 13:53:30 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the stats manager structure.
 */

m_stats*		stats;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

void			stats_begin(t_staid			staid,
				    char*			function)
{
  printf("stats_begin: %s\n", function);

  printf("-> %s\n", __FUNCTION__);
}

/*
 * XXX
 */

void			stats_end(t_staid			staid,
				  char*				function)
{

}

/*
 * XXX
 */

int			stats_dump(void)
{
  /* XXX */

  return (0);
}

/*
 * XXX
 */

int			stats_add(void)
{

}

/*
 * XXX
 */

int			stats_remove(void)
{

}

/*
 * XXX
 */

int			stats_rsv(char*				name,
				  t_staid*			staid)
{
  o_stats		o;

  memset(&o, 0x0, sizeof(o_stats));

  if ((o.name = strdup(name)) == NULL)
    return (-1);

  if (id_rsv(&stats->id, staid) != 0)
    return (-1);

  o.staid = *staid;

  /* XXX array */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_stats_func), &o.functions) != 0)
    {
      free(o.name);

      return (-1);
    }

  if (set_add(stats->container, &o) != 0)
    {
      free(o.name);

      set_rel(o.functions);

      return (-1);
    }

  return (0);
}

/*
 * XXX
 */

int			stats_rel(t_staid			staid)
{
  o_stats*		o;

  if (set_get(stats->container, staid, (void**)&o) != 0)
    return (-1);

  /* XXX foreach pour pouvoir liberer chaque chaine NAME */
  if (set_rel(o->functions) != 0)
    return (-1);

  free(o->name);

  if (set_remove(stats->container, o->staid) != 0)
    return (-1);

  return (0);
}

/*
 * XXX
 */

int			stats_init(void)
{
  /*
   * 1)
   */

  if ((stats = malloc(sizeof(m_stats))) == NULL)
    {
      cons_msg('!', "stats: cannot allocate memory for the stats manager "
	       "structure\n");

      return (-1);
    }

  memset(stats, 0x0, sizeof(m_stats));

  /*
   * 2)
   */

  if (id_build(&stats->id) != 0)
    {
      cons_msg('!', "stats: unable to initialise the identifier object\n");

      return (-1);
    }

  /*
   * 3)
   */

  /* XXX array */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_stats), &stats->container) != 0)
    {
      cons_msg('!', "stats: unable to reserve the stats container\n");

      return (-1);
    }

#if (DEBUG & DEBUG_STATS)
  stats_dump(void);
#endif

  return (0);
}

/*
 * XXX
 */

int			stats_clean(void)
{
  /*
   * 1)
   */

  if (set_rel(stats->container) != 0)
    {
      cons_msg('!', "stats: unable to release the stats container\n");

      return (-1);
    }

  /*
   * 2)
   */

  if (id_destroy(&stats->id) != 0)
    {
      cons_msg('!', "stats: unable to destroy the identifier object\n");

      return (-1);
    }

  /*
   * 3)
   */

  free(stats);

  return (0);
}
