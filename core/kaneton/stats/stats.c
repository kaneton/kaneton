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
 * last update   Mon Nov 28 12:50:15 2005   buckman
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
 * ---------- assignments -----------------------------------------------------
 *
 * the students have nothing to do here.
 *
 * nevetheless they can use the stats manager if they want to evaluate
 * their kernel implementation.
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

t_error			stats_function(t_staid			staid,
				       char*			function,
				       t_stats_func**		f)
{
  t_sint64		slot = -1;
  t_sint64		i;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  for (i = 0; i < stats->container[staid].functionssz; i++)
    {
      if (stats->container[staid].functions[i].name == NULL)
	{
	  if (slot == -1)
	    slot = i;

	  continue;
	}

      if (strcmp(function, stats->container[staid].functions[i].name) == 0)
	{
	  slot = i;
	  break;
	}
    }

  /*
   * 2)
   */

  if ((i == stats->container[staid].functionssz) && (slot == -1))
    {
      slot = stats->container[staid].functionssz;

      if ((stats->container[staid].functions =
	   realloc(stats->container[staid].functions,
		   (stats->container[staid].functionssz * 2) *
		   sizeof(t_stats_func))) == NULL)
	STATS_LEAVE(stats, ERROR_UNKNOWN);

      stats->container[staid].functionssz *= 2;

      memset(stats->container[staid].functions + slot,
	     0x0, (stats->container[staid].functionssz - slot) *
	     sizeof(t_stats_func));
    }

  /*
   * 3)
   */

  *f = &stats->container[staid].functions[slot];

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

t_error			stats_begin(t_staid			staid,
				    char*			function)
{
  t_stats_func*		f;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  if (staid < 0 || staid >= stats->containersz ||
      stats->container[staid].name == NULL)
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

      /*
       * XXX start the timer here
       */
    }

  /*
   * 4)
   */

  f->calls++;

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function annonces the end of a function.
 *
 * steps:
 *
 * 1) checks if the stats object is correct.
 * 2) gets the function slot.
 * 3) if an error occured, increases the number of errors in this function.
 */

t_error			stats_end(t_staid			staid,
				  char*				function,
				  t_error			error)
{
  t_stats_func*		f;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  if (staid < 0 || staid >= stats->containersz ||
      stats->container[staid].name == NULL)
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

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function shows a stats object.
 */

t_error			stats_show(t_staid			staid)
{
  t_sint64		i;

  STATS_ENTER(stats);

  if (staid < 0 || staid >= stats->containersz ||
      stats->container[staid].name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  cons_msg('#', "  [%qu] %s\n",
	   staid,
	   stats->container[staid].name);

  for (i = 0; i < stats->container[staid].functionssz; i++)
    {
      if (stats->container[staid].functions[i].name == NULL)
	continue;
      cons_msg('#', "    %s: %qu call(s) [%qu errors]\n",
	       stats->container[staid].functions[i].name,
	       stats->container[staid].functions[i].calls,
	       stats->container[staid].functions[i].errors);

      /*
       * XXX dump the timer here
       */
    }

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function dumps the statistics.
 */

t_error			stats_dump(void)
{
  t_staid		staid;
  t_sint64		i;

  STATS_ENTER(stats);

  cons_msg('#', "dumping statistics:\n");

  for (staid = 0; staid < stats->containersz; staid++)
    {
      if (stats->container[staid].name == NULL)
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
				      t_staid*			staid)
{
  STATS_ENTER(stats);

  /*
   * 1)
   */

  for (*staid = 0; *staid < stats->containersz; (*staid)++)
    if (stats->container[*staid].name == NULL)
      break;

  /*
   * 2)
   */

  if (*staid == stats->containersz)
    {
      if ((stats->container = realloc(stats->container,
				      (stats->containersz * 2) *
				      sizeof (t_stats))) == NULL)
	STATS_LEAVE(stats, ERROR_UNKNOWN);

      *staid = stats->containersz;

      stats->containersz *= 2;

      memset(stats->container + *staid, 0x0,
	     (stats->containersz - *staid) * sizeof (t_stats));
    }

  /*
   * 3)
   */

  if ((stats->container[*staid].name = strdup(name)) == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  if ((stats->container[*staid].functions =
       malloc(STATS_FUNCTIONS_INITSZ * sizeof(t_stats_func))) == NULL)
    {
      memset(&stats->container[*staid], 0x0, sizeof(t_stats));

      STATS_LEAVE(stats, ERROR_UNKNOWN);
    }

  stats->container[*staid].functionssz = STATS_FUNCTIONS_INITSZ;

  memset(stats->container[*staid].functions, 0x0,
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

t_error			stats_release(t_staid			staid)
{
  t_sint64		i;

  STATS_ENTER(stats);

  /*
   * 1)
   */

  if (staid < 0 || staid >= stats->containersz ||
      stats->container[staid].name == NULL)
    STATS_LEAVE(stats, ERROR_UNKNOWN);

  /*
   * 2)
   */

  free(stats->container[staid].name);

  for (i = 0; i < stats->container[staid].functionssz; i++)
    {
      if (stats->container[staid].functions[i].name != NULL)
	free(stats->container[staid].functions[i].name);
    }

  /*
   * XXX release the timers here
   */

  free(stats->container[staid].functions);

  /*
   * 3)
   */

  memset(&stats->container[staid], 0x0, sizeof(t_stats));

  STATS_LEAVE(stats, ERROR_NONE);
}

/*
 * this function initialises the statistics manager.
 *
 * steps:
 *
 * 1) allocates and initialises the statistics manager main structure.
 * 2) allocates and initialises the statistics object data structure.
 * 3) runs tests if necessary
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

  if ((stats->container = malloc(STATS_CONTAINER_INITSZ *
				 sizeof(t_stats))) == NULL)
    {
      cons_msg('!', "stats: unable to allocate the stats container\n");

      return (ERROR_UNKNOWN);
    }

  stats->containersz = STATS_CONTAINER_INITSZ;

  memset(stats->container, 0x0, stats->containersz * sizeof(t_stats));

  /*
   * 3)
   */

#if (DEBUG & DEBUG_STATS)
  stats_test();
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
  t_staid		staid;
  t_sint64		i;

  /*
   * 1)
   */

  for (staid = 0; staid < stats->containersz; staid++)
    stats_release(staid);

  free(stats->container);

  /*
   * 2)
   */

  free(stats);

  return (ERROR_NONE);
}


#if (DEBUG & DEBUG_STATS)
/*
 * this function is used by stats_test to test stats in functions.
 *
 */

static void	stats_test_fun(t_staid st)
{
  int		i;

  for (i = 0; i < 5; ++i)
    {
      if (STATS_BEGIN(st) != ERROR_NONE)
	cons_msg('!', "error beginning stats (st_fun)\n");
      if (i % 2)
	if (STATS_END(st, ERROR_NONE) != ERROR_NONE)
	  cons_msg('!', "error ending stats (st_fun)\n");
      else
	if (STATS_END(st, ERROR_UNKNOWN) != ERROR_NONE)
	  cons_msg('!', "error ending stats (st_fun)\n");
    }
}
#endif

/*
 * this function tests the stat manager.
 *
 * steps:
 *
 * 1) reserves some stats objects.
 * 2) simulates some calls with or without errors.
 *   a) basic operations (calls / errors).
 *   b) a fast solution to simulate plenty of calls.
 *   c) programming errors and unwanted behaviors (must not segfault !).
 * 3) displays the stats.
 * 4) releases the stats objects.
 */

t_error			stats_test(void)
{
  t_staid		st1;
  t_staid		st2;
  t_staid		st3;
  t_staid		st4;
  t_staid		st5;
  t_staid		st6;
  t_staid		sterr;
  t_staid		stuseless1;
  t_staid		stuseless2;
  t_staid		stuseless3;
  t_staid		stuseless4;
  int			i;

  STATS_ENTER(stats);
#if (DEBUG & DEBUG_STATS)
  cons_msg('#', "testing the stats manager\n");

  /*
   * 1)
   */

  if (STATS_RESERVE("stats_test_nocall", &st1) != ERROR_NONE ||
      STATS_RESERVE("stats_test_manycalls", &st2) != ERROR_NONE ||
      STATS_RESERVE("stats_test_manyerrors", &st3) != ERROR_NONE ||
      STATS_RESERVE("stats_test_mixed", &st4) != ERROR_NONE ||
      STATS_RESERVE("stats_test_subfun", &st5) != ERROR_NONE ||
      STATS_RESERVE("stats_test_lotoffun", &st6) != ERROR_NONE ||
      STATS_RESERVE("stats_test_err", &sterr) != ERROR_NONE ||
      STATS_RESERVE("stats_useless1", &stuseless1) != ERROR_NONE ||
      STATS_RESERVE("stats_useless2", &stuseless2) != ERROR_NONE ||
      STATS_RESERVE("stats_useless3", &stuseless3) != ERROR_NONE ||
      STATS_RESERVE("stats_useless4", &stuseless4) != ERROR_NONE)
    cons_msg('!', "error reserving stats objects\n");

  /*
   * 2)
   */

  /*
   * a)
   */

  for (i = 0; i < 1234; ++i)
    {
      if (STATS_BEGIN(st2) != ERROR_NONE)
	cons_msg('!', "error beginning stats (st2)\n");
      if (STATS_END(st2, ERROR_NONE) != ERROR_NONE)
	cons_msg('!', "error ending stats (st2)\n");
    }
  for (i = 0; i < 567; ++i)
    {
      if (STATS_BEGIN(st3) != ERROR_NONE)
	cons_msg('!', "error beginning stats (st3)\n");
      if (STATS_END(st3, ERROR_UNKNOWN) != ERROR_NONE)
	cons_msg('!', "error ending stats (st3)\n");
    }
  for (i = 0; i < 200; ++i)
    {
      if (STATS_BEGIN(st4) != ERROR_NONE)
	cons_msg('!', "error beginning stats (st4)\n");
      if (i % 2)
	if (STATS_END(st4, ERROR_NONE) != ERROR_NONE)
	  cons_msg('!', "error ending stats (st4)\n");
      else
	if (STATS_END(st4, ERROR_UNKNOWN) != ERROR_NONE)
	  cons_msg('!', "error ending stats (st4)\n");
    }
  for (i = 0; i < 10; ++i)
    {
      if (STATS_BEGIN(st5) != ERROR_NONE)
	cons_msg('!', "error beginning stats (st5)\n");
      if (i % 2)
	{
	  if (STATS_END(st5, ERROR_NONE) != ERROR_NONE)
	    cons_msg('!', "error ending stats (st5)\n");
	  stats_test_fun(st5);
	}
      else
	if (STATS_END(st5, ERROR_UNKNOWN) != ERROR_NONE)
	  cons_msg('!', "error ending stats (st5)\n");
    }

  /*
   * b)
   */

  stats_begin(st6, "fun1");
  stats_begin(st6, "fun2");
  stats_begin(st6, "fun3");
  stats_begin(st6, "fun4");
  stats_begin(st6, "fun5");
  stats_begin(st6, "fun6");
  stats_end(st6, "fun1", ERROR_NONE);
  stats_end(st6, "fun2", ERROR_NONE);
  stats_end(st6, "fun3", ERROR_UNKNOWN);
  stats_end(st6, "fun4", ERROR_NONE);
  stats_end(st6, "fun5", ERROR_UNKNOWN);
  stats_end(st6, "fun6", ERROR_NONE);

  /*
   * c)
   */

  stats_end(sterr, "foo", ERROR_UNKNOWN);
  stats_begin(sterr, "foo");
  stats_end(sterr, "foo", ERROR_UNKNOWN);
  stats_end(sterr, "foo", ERROR_UNKNOWN);
  stats_begin(sterr, "foo");
  stats_begin(sterr, "foo");
  stats_begin(sterr, "foo");
  stats_end(sterr, "foo", ERROR_UNKNOWN);
  STATS_RELEASE(-4);
  STATS_RELEASE(stuseless4 + 11);

  /*
   * 3)
   */

  if (stats_show(st1) != ERROR_NONE)
    cons_msg('!', "error displaying stats (st1)\n");
  if (stats_show(st2) != ERROR_NONE)
    cons_msg('!', "error displaying stats (st2)\n");
  if (stats_show(st3) != ERROR_NONE)
    cons_msg('!', "error displaying stats (st3)\n");
  if (stats_show(st4) != ERROR_NONE)
    cons_msg('!', "error displaying stats (st4)\n");
  if (stats_show(st5) != ERROR_NONE)
    cons_msg('!', "error displaying stats (st5)\n");
  if (stats_show(st6) != ERROR_NONE)
    cons_msg('!', "error displaying stats (st6)\n");
  if (stats_show(sterr) != ERROR_NONE)
    cons_msg('!', "error displaying stats (sterr)\n");

  /*
   * 4)
   */

  if (STATS_RELEASE(st1) != ERROR_NONE ||
      STATS_RELEASE(st2) != ERROR_NONE ||
      STATS_RELEASE(st3) != ERROR_NONE ||
      STATS_RELEASE(st4) != ERROR_NONE ||
      STATS_RELEASE(st5) != ERROR_NONE ||
      STATS_RELEASE(st6) != ERROR_NONE ||
      STATS_RELEASE(sterr) != ERROR_NONE ||
      STATS_RELEASE(stuseless1) != ERROR_NONE ||
      STATS_RELEASE(stuseless2) != ERROR_NONE ||
      STATS_RELEASE(stuseless3) != ERROR_NONE ||
      STATS_RELEASE(stuseless4) != ERROR_NONE)
    cons_msg('!', "error releasing stats objects\n");
#endif
  STATS_LEAVE(stats, ERROR_NONE);
}
