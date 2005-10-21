/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * stats.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri Oct 21 19:57:34 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX dire que id et set ne peuvent pas etre benche car stats depend
 *     d eux.
 *
 * XXX on peut egalement etendre le concept et faire en sorte d'etre
 *     independent pour pouvoir bencher id, et surtout set qui doit etre
 *     benche car c est la qu on va passer bcp de temps.
 *     genre faire un pauvre tableau car il n'y aura que de l'ajout quasiment.
 *
 * XXX greper STATS_BEGIN et STATS_END
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
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

t_error			stats_begin(t_staid			staid,
				    char*			function)
{
  t_id			id;
  o_stats*		o;
  o_stats_func*		f;

  STATS_ENTER(stats);

  if (set_get(stats->container, staid, (void**)&o) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, staid, function);

  id = (t_id)sum2(function, strlen(function));

  if (set_get(o->functions, id, (void**)&f) != ERROR_NONE)
    {
      o_stats_func	n;

      memset(&n, 0x0, sizeof(o_stats_func));

      n.id = id;
      n.name = strdup(function);
      n.calls = 1;
      n.errors = 0;

      /* XXX timer */

      if (set_add(o->functions, &n) != ERROR_NONE)
	{
	  printf("XXX: begin: error\n");

	  STATS_LEAVE(stats, ERROR_UNKNOWN, staid, function);
	}
    }
  else
    {
      f->calls++;
    }

  STATS_LEAVE(stats, ERROR_NONE, staid, function);
}

/*
 * XXX
 */

t_error			stats_end(t_staid			staid,
				  char*				function,
				  t_error			error)
{
  t_id			id;
  o_stats*		o;
  o_stats_func*		f;

  STATS_ENTER(stats);

  if (set_get(stats->container, staid, (void**)&o) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, staid, function, error);

  id = (t_id)sum2(function, strlen(function));

  if (set_get(o->functions, id, (void**)&f) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, staid, function, error);

  if (error != ERROR_NONE)
    f->errors++;

  STATS_LEAVE(stats, ERROR_NONE, staid, function, error);
}

/*
 * XXX
 */

t_error			stats_dump(void)
{
  t_state		outter;
  t_state		inner;
  t_setsz		size;
  o_stats*		o;
  o_stats_func*		f;
  t_iterator		i;
  t_iterator		j;

  STATS_ENTER(stats);

  if (set_size(stats->container, &size) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  cons_msg('#', "dumping %qu statistic(s):\n",
	   size);

  set_foreach(SET_OPT_FORWARD, stats->container, &i, outter)
    {
      if (set_object(stats->container, i, (void**)&o) != ERROR_NONE)
	{
	  cons_msg('!', "stats: cannot find the stats object "
		   "corresponding to its identifier\n");

	  STATS_LEAVE(stats, ERROR_UNKNOWN);
	}

      cons_msg('#', "  [%qu] %s\n",
	       o->staid,
	       o->name);

      set_foreach(SET_OPT_FORWARD, o->functions, &j, inner)
	{
	  if (set_object(o->functions, j, (void**)&f) != ERROR_NONE)
	    {
	      cons_msg('!', "stats: cannot find the stats object "
		       "corresponding to its identifier\n");

	      STATS_LEAVE(stats, ERROR_UNKNOWN);
	    }

	  /* XXX timer */

	  cons_msg('#', "    %s: %u call(s) [%u errors]\n",
		   f->name,
		   f->calls,
		   f->errors);
	}
    }

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * XXX
 */

t_error			stats_rsv(char*				name,
				  t_staid*			staid)
{
  o_stats		o;

  STATS_ENTER(stats);

  memset(&o, 0x0, sizeof(o_stats));

  if ((o.name = strdup(name)) == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN, name, staid);

  if (id_rsv(&stats->id, staid) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, name, staid);

  o.staid = *staid;

  /* XXX array */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_stats_func), &o.functions) != ERROR_NONE)
    {
      free(o.name);

      STATS_LEAVE(stats, ERROR_UNKNOWN, name, staid);
    }

  if (set_add(stats->container, &o) != ERROR_NONE)
    {
      free(o.name);

      set_rel(o.functions);

      STATS_LEAVE(stats, ERROR_UNKNOWN, name, staid);
    }

  STATS_LEAVE(stats, ERROR_NONE, name, staid);
}

/*
 * XXX
 */

t_error			stats_rel(t_staid			staid)
{
  o_stats*		o;

  STATS_ENTER(stats);

  if (set_get(stats->container, staid, (void**)&o) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, staid);

  /* XXX foreach pour pouvoir liberer chaque chaine NAME */
  if (set_rel(o->functions) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, staid);

  free(o->name);

  if (set_remove(stats->container, o->staid) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN, staid);

  STATS_LEAVE(stats, ERROR_NONE, staid);
}

/*
 * XXX
 */

t_error			stats_init(void)
{
  /*
   * 1)
   */

  if ((stats = malloc(sizeof(m_stats))) == NULL)
    {
      cons_msg('!', "stats: cannot allocate memory for the stats manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(stats, 0x0, sizeof(m_stats));

  /*
   * 2)
   */

  if (id_build(&stats->id) != ERROR_NONE)
    {
      cons_msg('!', "stats: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  /* XXX array */

  if (set_rsv(ll, SET_OPT_ALLOC | SET_OPT_SORT,
	      sizeof(o_stats), &stats->container) != ERROR_NONE)
    {
      cons_msg('!', "stats: unable to reserve the stats container\n");

      return (ERROR_UNKNOWN);
    }

#if (DEBUG & DEBUG_STATS)
  stats_dump(void);
#endif

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			stats_clean(void)
{
  /*
   * 1)
   */

  if (set_rel(stats->container) != ERROR_NONE)
    {
      cons_msg('!', "stats: unable to release the stats container\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 2)
   */

  if (id_destroy(&stats->id) != ERROR_NONE)
    {
      cons_msg('!', "stats: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  free(stats);

  return (ERROR_NONE);
}
