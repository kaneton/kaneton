/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [tue dec 20 22:50:57 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../init/init.h"

void		check_stats_01(void);
static void	stats_test_fun(t_staid st);

/*
 * basic tests
 */

void		check_stats_01(void)
{
  t_staid	st1;
  t_staid	st2;
  t_staid	st3;
  t_staid	st4;
  t_staid	st5;
  int		i;

  TEST_ENTER(check_stats_01);

  if (STATS_RESERVE("stats_test_nocall", &st1) != ERROR_NONE ||
      STATS_RESERVE("stats_test_manycalls", &st2) != ERROR_NONE ||
      STATS_RESERVE("stats_test_manyerrors", &st3) != ERROR_NONE ||
      STATS_RESERVE("stats_test_mixed", &st4) != ERROR_NONE ||
      STATS_RESERVE("stats_test_subfun", &st5) != ERROR_NONE)
    cons_msg('!', "error reserving stats objects\n");

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
	{
	  if (STATS_END(st4, ERROR_NONE) != ERROR_NONE)
	    cons_msg('!', "error ending stats (st4)\n");
	}
      else
	{
	  if (STATS_END(st4, ERROR_UNKNOWN) != ERROR_NONE)
	    cons_msg('!', "error ending stats (st4)\n");
	}
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
	{
	  if (STATS_END(st5, ERROR_UNKNOWN) != ERROR_NONE)
	    cons_msg('!', "error ending stats (st5)\n");
	}
    }

  STATS_DUMP();

  if (STATS_RELEASE(st1) != ERROR_NONE ||
      STATS_RELEASE(st2) != ERROR_NONE ||
      STATS_RELEASE(st3) != ERROR_NONE ||
      STATS_RELEASE(st4) != ERROR_NONE ||
      STATS_RELEASE(st5) != ERROR_NONE)
    cons_msg('!', "error releasing stats objects\n");

  st1 = st1;
  st2 = st2;
  st3 = st3;
  st4 = st4;

  TEST_LEAVE(check_stats_01);
}

static void	stats_test_fun(t_staid st)
{
  int		i;

  for (i = 0; i < 5; ++i)
    {
      if (STATS_BEGIN(st) != ERROR_NONE)
	cons_msg('!', "error beginning stats (st_fun)\n");

      if (i % 2)
	{
	  if (STATS_END(st, ERROR_NONE) != ERROR_NONE)
	    cons_msg('!', "error ending stats (st_fun)\n");
	}
      else
	{
	  if (STATS_END(st, ERROR_UNKNOWN) != ERROR_NONE)
	    cons_msg('!', "error ending stats (st_fun)\n");
	}
    }
}
