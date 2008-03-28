/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kaneton/kaneton/core/debug/view.c
 *
 * created       matthieu bucchianeri   [wed jun 20 23:51:47 2007]
 * updated       laurent lec   [thu mar 27 21:27:10 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the task viewer that generates graphics showing cpu
 * loading and task switches.
 *
 * warning: this is single processor implementation.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>
#include <libc/libc.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

m_view*		view = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/* XXX move me */

t_uint64		read_timestamp(void)
{
  t_uint32		high;
  t_uint32		low;

#ifdef IA32_DEPENDENT
  asm ("rdtsc"
       : "=d" (high), "=a" (low));
#endif

  return ((t_uint64)high << 32 | low);
}

/*
 * this function resets the statistics set.
 */

t_error			view_reset(void)
{
  o_view*		data;
  t_iterator		i;

  VIEW_ENTER(view);

  while (set_head(view->views, &i) == ERROR_NONE)
    {
      if (set_object(view->views, i, (void**)&data) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (set_remove(view->views, data->id) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  view->record.id = ID_UNUSED;
  view->cycles = 0;

  VIEW_LEAVE(view, ERROR_UNKNOWN);
}

/*
 * return the activity name given its prefix and id. use flyweight design
 * pattern.
 */

static char*		get_activity(const char*		name,
				     t_uint32			nr)
{
  char			activity[20];
  int			i;

  strcpy(activity, name);
  activity[strlen(name)] = '0' + (nr / 10);
  activity[strlen(name) + 1] = '0' + (nr % 10);
  activity[strlen(name) + 2] = 0;

  for (i = 0; i < 999 && view->flyweight[i] != NULL; i++)
    if (!strcmp(activity, view->flyweight[i]))
      return view->flyweight[i];

  ASSERT(i < 999);

  return (view->flyweight[i] = strdup(activity));
}

/*
 * this function signals a switch to an activity.
 *
 * steps:
 *
 * 1) check for self-preemption.
 * 2) add previous record.
 * 3) prepare next record.
 */

t_error			view_signal(const char*			name,
				    t_uint32			nr,
				    t_state			action)
{
  t_uint64		timestamp;
  o_view		o;
  char*			activity = NULL;

  VIEW_ENTER(view);

  /*
   * 1)
   */

  if (view->record.id != ID_UNUSED &&
      (action == VIEW_SIGNAL_PREEMPT || action == VIEW_SIGNAL_RESUME) &&
      (activity = get_activity(name, nr)) == view->record.name)
    VIEW_LEAVE(view, ERROR_NONE);

  if (activity == NULL)
    activity = get_activity(name, nr);

  /*
   * 2)
   */

  timestamp = read_timestamp();

  if (view->record.id != ID_UNUSED)
    {
      view->record.time = timestamp - view->record.time;
      view->cycles += view->record.time;

      if (set_append(view->views, &view->record) != ERROR_NONE)
	VIEW_LEAVE(view, ERROR_UNKNOWN);
    }

  if (action == VIEW_SIGNAL_RUN ||
      action == VIEW_SIGNAL_STOP ||
      action == VIEW_SIGNAL_BLOCK)
    {
      if (id_reserve(&view->id, &o.id) != ERROR_NONE)
	VIEW_LEAVE(view, ERROR_UNKNOWN);

      o.name = activity;
      o.time = (t_uint64)(-action);

      if (set_append(view->views, &o) != ERROR_NONE)
	VIEW_LEAVE(view, ERROR_UNKNOWN);

      if (id_reserve(&view->id, &view->record.id) != ERROR_NONE)
	VIEW_LEAVE(view, ERROR_UNKNOWN);

      view->record.time = timestamp;

      VIEW_LEAVE(view, ERROR_NONE);
    }

  /*
   * 3)
   */

  if (id_reserve(&view->id, &view->record.id) != ERROR_NONE)
    VIEW_LEAVE(view, ERROR_UNKNOWN);

  view->record.name = activity;
  view->record.time = timestamp;

  VIEW_LEAVE(view, ERROR_NONE);
}

/*
 * this function download recorded statistics to the referee via serial link.
 *
 * steps:
 *
 * 1) initialize serial port.
 * 2) loop thought the records.
 * 3) send record data.
 * 4) remove the record.
 * 5) send the enf of file signal.
 * 6) reset the manager.
 */

t_error			view_download(void)
{
  t_iterator		it;
  o_view*		o;
  t_uint32		n = 0;
  t_setsz		sz;
  char			c = '-';

  VIEW_ENTER(view);

  cons_msg('+', "transfering view data...\n");

  /*
   * 1)
   */

#ifdef IA32_DEPENDENT
  if (serial_init(SERIAL_PRIMARY, SERIAL_BR57600, SERIAL_8N1) != ERROR_NONE)
    VIEW_LEAVE(view, ERROR_UNKNOWN);
  serial_send(SERIAL_PRIMARY, (t_uint8*)&view->cycles, sizeof (t_uint64));

  for (n = 0; n < 999 && view->flyweight[n] != NULL; n++)
    ;

  n *= 1000;
  serial_send(SERIAL_PRIMARY, (t_uint8*)&n, sizeof (t_uint64));
  n = 0;
#endif

  /*
   * 2)
   */

  if (set_size(view->views, &sz) != ERROR_NONE)
    VIEW_LEAVE(view, ERROR_UNKNOWN);

  while (set_head(view->views, &it) == ERROR_NONE)
    {
      if (set_object(view->views, it, (void**)&o) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /*
       * 3)
       */

#ifdef IA32_DEPENDENT
      serial_send(SERIAL_PRIMARY, o->name, strlen(o->name));
      serial_send(SERIAL_PRIMARY, (t_uint8*)&o->time, sizeof (t_uint64));
#endif

      printf("\r--> %c (%u records / %u)", c, ++n, sz);

      if (c == '-')
	c = '\\';
      else if (c == '\\')
	c = '|';
      else if (c == '|')
	c = '/';
      else
	c = '-';

      /*
       * 4)
       */

      if (set_remove(view->views, o->id) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  printf("\n");

  /*
   * 5)
   */

#ifdef IA32_DEPENDENT
  serial_send(SERIAL_PRIMARY, "eof", 3);
#endif

  cons_msg('+', "done.\n");

  /*
   * 6)
   */

  view->record.id = ID_UNUSED;
  view->cycles = 0;

  VIEW_LEAVE(view, ERROR_NONE);
}

/*
 * this function initialises the view manager.
 *
 * steps:
 *
 * 1) allocate and initialises the view manager structure.
 * 2) reserve an id object.
 * 3) reserve the views set, which will contains switches information.
 */

t_error			view_initialize(void)
{
  /*
   * 1)
   */

  if ((view = malloc(sizeof(m_view))) == NULL)
    {
      cons_msg('!', "view: cannot allocate memory for the view manager "
	       "structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(view, 0x0, sizeof(m_view));
  view->record.id = ID_UNUSED;

  /*
   * 2)
   */

  if (id_build(&view->id) != ERROR_NONE)
    {
      cons_msg('!', "view: unable to initialise the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(o_view),
		  &view->views) != ERROR_NONE)
    {
      cons_msg('!', "view: unable to reserve the view set\n");

      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * this function just cleans the view manager.
 *
 * steps:
 *
 * 1) release the view's set.
 * 2) destroy the id object.
 * 3) free the view manager structure's memory.
 */

t_error			view_clean(void)
{
  o_view*		data;
  t_iterator		i;
  int			j;

  /*
   * 1)
   */

  while (set_head(view->views, &i) == ERROR_NONE)
    {
      if (set_object(view->views, i, (void**)&data) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (set_remove(view->views, data->id) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  if (set_release(view->views) != ERROR_NONE)
    {
      cons_msg('!', "view: unable to release the views' set\n");

      return (ERROR_UNKNOWN);
    }

  for (j = 0; j < 999 && view->flyweight[j] != NULL; j++)
    free(view->flyweight[j]);

  /*
   * 2)
   */

  if (id_destroy(&view->id) != ERROR_NONE)
    {
      cons_msg('!', "view: unable to destroy the identifier object\n");

      return (ERROR_UNKNOWN);
    }

  /*
   * 3)
   */

  free(view);

  view = NULL;

  return (ERROR_NONE);
}
