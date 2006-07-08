/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/stats/stats.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sat jul  8 02:27:41 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this manager is used to make statistics on managers and more precisely
 * on manager functions use: number of calls, number of errors, time past
 * in each function etc..
 *
 * with this manager we will be able to evaluate critical managers
 * like the set manager and especially critical functions which could
 * be optimised in a near futur.
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
 * ---------- externals--------------------------------------------------------
 */

/*
 * the timer manager structure.
 */

extern m_timer*		timer;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tries to find a free slot in the functions array
 * of a statistics object.
 *
 * steps:
 *
 * 1) tries to find the correct function. if not found, tries to find
 *    a free slot.
 * 2) if there is not free slot, expands the functions array.
 * 3) returns a correct pointer on the correct slot or on a free slot.
 */

t_error			stats_function(i_stats			staid,
				       char*			function,
				       t_stats_func**		f)
{
  t_sint64		slot = -1;
  t_sint64		i;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  for (i = 0; i < stats->managers[staid].functionssz; i++)
    {
      if (stats->managers[staid].functions[i].name == NULL)
	{
	  if (slot == -1)
	    slot = i;

	  continue;
	}

      if (strcmp(function, stats->managers[staid].functions[i].name) == 0)
	{
	  slot = i;
	  break;
	}
    }

  /*
   * 2)
   */

  if ((i == stats->managers[staid].functionssz) && (slot == -1))
    {
      slot = stats->managers[staid].functionssz;

      if ((stats->managers[staid].functions =
	   realloc(stats->managers[staid].functions,
		   (stats->managers[staid].functionssz * 2) *
		   sizeof(t_stats_func))) == NULL)
	STATS_LEAVE(stats, ERROR_UNKNOWN);

      stats->managers[staid].functionssz *= 2;

      memset(stats->managers[staid].functions + slot,
	     0x0, (stats->managers[staid].functionssz - slot) *
	     sizeof(t_stats_func));
    }

  /*
   * 3)
   */

  *f = &stats->managers[staid].functions[slot];

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function annonces a new function call.
 *
 * steps:
 *
 * 1) checks if the stats object is correct.
 * 2) gets the function slot from the function name.
 * 3) if the function slot does not exist, creates it.
 * 4) increases the number of function calls.
 */

t_error			stats_begin(i_stats			staid,
				    char*			function)
{
  t_stats_func*		f;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  if (staid >= stats->managerssz || stats->managers[staid].name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (stats_function(staid, function, &f) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (f->name == NULL)
    {
      if ((f->name = strdup(function)) == NULL)
	STATS_LEAVE(stats, ERROR_UNKNOWN);

      f->calls = 0;
      f->errors = 0;
      f->total = 0;
    }

  /*
   * 4)
   */

  if (timer != NULL)
    f->timer_start = timer->timeref;
  f->calls++;

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function annonces the end of a function.
 *
 * steps:
 *
 * 1) check if the stats object is correct.
 * 2) get the function slot.
 * 3) if an error occured, increase the number of errors in this function.
 * 4) update the time statistics.
 */

t_error			stats_end(i_stats			staid,
				  char*				function,
				  t_error			error)
{
  t_stats_func*		f;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  if (staid >= stats->managerssz || stats->managers[staid].name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (stats_function(staid, function, &f) != ERROR_NONE)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  if (f->name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (error != ERROR_NONE)
    f->errors++;

  /*
   * 4)
   */

  if (timer != NULL)
    f->total += timer->timeref - f->timer_start;

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function shows a stats object.
 */

t_error			stats_show(i_stats			staid)
{
  t_sint64		i;
  t_uint32		total = 0;

  STATS_ENTER(stats);

  if (staid >= stats->managerssz || stats->managers[staid].name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  cons_msg('#', "  [%qu] %s\n",
	   staid,
	   stats->managers[staid].name);

  for (i = 0; i < stats->managers[staid].functionssz; i++)
    {
      if (stats->managers[staid].functions[i].name == NULL)
	continue;
      cons_msg('#', "    %s: %qu call(s) [%qu errors] %u ticks total\n",
	       stats->managers[staid].functions[i].name,
	       stats->managers[staid].functions[i].calls,
	       stats->managers[staid].functions[i].errors,
	       stats->managers[staid].functions[i].total);

      total += stats->managers[staid].functions[i].total;
    }

  cons_msg('#', "  [%qu] total tick : %u\n",
	   staid,
	   total);

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function dumps the statistics.
 */

t_error			stats_dump(void)
{
  i_stats		staid;

  STATS_ENTER(stats);

  cons_msg('#', "dumping statistics:\n");

  for (staid = 0; staid < stats->managerssz; staid++)
    {
      if (stats->managers[staid].name == NULL)
	continue;

      if (stats_show(staid) != ERROR_NONE)
	STATS_LEAVE(stats, ERROR_UNKNOWN);
    }

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function reserves a statistics object.
 *
 * steps:
 *
 * 1) tries to find a free slot.
 * 2) if none, expands the array.
 * 3) then, initialises the name statistics object, and its functions
 *    data structure.
 */

t_error			stats_reserve(char*			name,
				      i_stats*			staid)
{
  STATS_ENTER(stats);

  /*
   * 1)
   */

  for (*staid = 0; *staid < stats->managerssz; (*staid)++)
    if (stats->managers[*staid].name == NULL)
      break;

  /*
   * 2)
   */

  if (*staid == stats->managerssz)
    {
      if ((stats->managers = realloc(stats->managers,
				      (stats->managerssz * 2) *
				      sizeof (t_stats))) == NULL)
	STATS_LEAVE(stats, ERROR_UNKNOWN);

      *staid = stats->managerssz;

      stats->managerssz *= 2;

      memset(stats->managers + *staid, 0x0,
	     (stats->managerssz - *staid) * sizeof (t_stats));
    }

  /*
   * 3)
   */

  if ((stats->managers[*staid].name = strdup(name)) == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  if ((stats->managers[*staid].functions =
       malloc(STATS_FUNCTIONS_INITSZ * sizeof(t_stats_func))) == NULL)
    {
      memset(&stats->managers[*staid], 0x0, sizeof(t_stats));

      STATS_LEAVE(stats, ERROR_UNKNOWN);
    }

  stats->managers[*staid].functionssz = STATS_FUNCTIONS_INITSZ;

  memset(stats->managers[*staid].functions, 0x0,
	 STATS_FUNCTIONS_INITSZ * sizeof(t_stats_func));

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function releases a previously reserved statistics object.
 *
 * steps:
 *
 * 1) checks the statistics object validity.
 * 2) free the statistics object and its contents.
 * 3) reinitialises the slot.
 */

t_error			stats_release(i_stats			staid)
{
  t_sint64		i;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  if (staid >= stats->managerssz || stats->managers[staid].name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  /*
   * 2)
   */

  free(stats->managers[staid].name);

  for (i = 0; i < stats->managers[staid].functionssz; i++)
    {
      if (stats->managers[staid].functions[i].name != NULL)
	free(stats->managers[staid].functions[i].name);
    }

  free(stats->managers[staid].functions);

  /*
   * 3)
   */

  memset(&stats->managers[staid], 0x0, sizeof(t_stats));

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function initialises the statistics manager.
 *
 * steps:
 *
 * 1) allocates and initialises the statistics manager main structure.
 * 2) allocates and initialises the statistics object data structure.
 * 3) dumps state if necessary.
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

  if ((stats->managers = malloc(STATS_MANAGERS_INITSZ *
				 sizeof(t_stats))) == NULL)
    {
      cons_msg('!', "stats: unable to allocate the stats managers\n");

      return (ERROR_UNKNOWN);
    }

  stats->managerssz = STATS_MANAGERS_INITSZ;

  memset(stats->managers, 0x0, stats->managerssz * sizeof(t_stats));

  /*
   * 3)
   */

#if (DEBUG & DEBUG_STATS)
  stats_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function cleans the statistics manager.
 *
 * steps:
 *
 * 1) releases every statistics object contained.
 * 2) frees the statistics main structure.
 */

t_error			stats_clean(void)
{
  i_stats		staid;

  /*
   * 1)
   */

  for (staid = 0; staid < stats->managerssz; staid++)
    stats_release(staid);

  free(stats->managers);

  /*
   * 2)
   */

  free(stats);

  return (ERROR_NONE);
}
