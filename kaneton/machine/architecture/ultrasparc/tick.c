/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/misc/tick.c
 *
 * created       matthieu bucchianeri   [mon aug 21 19:58:02 2006]
 * updated       matthieu bucchianeri   [mon aug 28 19:24:19 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

static t_uint64		clock_frequency = 0;

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_ofw_node	root_node;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * calibrate the timer.
 */

void			tick_calibrate(void)
{
  t_ofw_node		node;
  char			name[128];

  ofw_child(root_node, &node);

  while (ofw_sibling(node, &node) == STATUS_OK)
    {
      if (ofw_getprop(node, "device_type", name, 128, NULL) != STATUS_OK)
	continue;

      if (!strcmp(name, "cpu"))
	{
	  if (ofw_getprop(node, "clock-frequency", &clock_frequency,
			  sizeof (clock_frequency), NULL) != STATUS_OK)
	    continue;

	  printf("CPU clock frequency: %ld\n", clock_frequency);
	}
    }
}

/*
 * this function setup the tick counter interval.
 */

void			tick_interval(t_uint64			interval)
{
  if (!clock_frequency)
    tick_calibrate();

  /* XXX enable tick intr */
}
