/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/set/set-pipe.c
 *
 * created       renaud voltz   [wed jan 25 17:11:05 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:59:07 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this subpart of the set manager is used to build FIFO data structures like
 * pipes.
 *
 * this is actually a front-end using the linked-list manager.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ---------------------------------------------------------
 */

extern m_set*		set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tells if the set object is a pipe set.
 */

t_error			set_type_pipe(i_set			setid)
{
  o_set*		o;

  SET_ENTER(set);

  if (set_descriptor(setid, &o) != ERROR_NONE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if (o->type == SET_TYPE_PIPE)
    SET_LEAVE(set, ERROR_NONE);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) initialises the set descriptor.
 * 2) avoids bad options.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initialises the set descriptor fields.
 * 5) adds the set descriptor to the set container.
 */

t_error			set_reserve_pipe(t_opts			opts,
					 t_size			datasz,
					 i_set*			setid)
{
  o_set			o;

  SET_ENTER(set);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 2)
   */

  if (opts & SET_OPT_ORGANISE)
    SET_LEAVE(set, ERROR_UNKNOWN);

  if ((opts & SET_OPT_ALLOC) && (opts & SET_OPT_FREE))
    SET_LEAVE(set, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (opts & SET_OPT_CONTAINER)
    {
      *setid = set->sets;
    }
  else
    {
      if (id_reserve(&set->id, setid) != ERROR_NONE)
	SET_LEAVE(set, ERROR_UNKNOWN);
    }

  /*
   * 4)
   */

  o.setid = *setid;
  o.size = 0;
  o.type = SET_TYPE_PIPE;
  o.opts = opts;
  o.datasz = datasz;

  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_NONE)
    {
      if (!(opts & SET_OPT_CONTAINER))
	id_release(&set->id, o.setid);

      SET_LEAVE(set, ERROR_UNKNOWN);
    }

  SET_LEAVE(set, ERROR_NONE);
}

/*
 * this function shows set objects contained in the whole pipe.
 *
 */

t_error			set_show_pipe(i_set			setid)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_show_ll(setid);

  SET_LEAVE(set, retval);
}

/*
 * this function releases a set_pipe.
 */

t_error			set_release_pipe(i_set		setid)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_release_ll(setid);

  SET_LEAVE(set, retval);
}

/*
 * this function flushes the set_pipe and free every element.
 */

t_error			set_flush_pipe(i_set			setid)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_flush_ll(setid);

  SET_LEAVE(set, retval);
}

/*
 * this function add an object into the pipe.
 */

t_error			set_push_pipe(i_set			setid,
				      void*			data)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_insert_ll(setid, data);

  SET_LEAVE(set, retval);
}


/*
 * this function returns current outcomimg object of the pipe.
 */

t_error			set_pick_pipe(i_set			setid,
				      void**			data)
{
  t_error		retval;
  t_iterator		iterator;

  SET_ENTER(set);

  if ((retval = set_tail_ll(setid, &iterator)) != ERROR_NONE)
    SET_LEAVE(set, retval);

  retval = set_object_ll(setid, iterator, data);

  SET_LEAVE(set, retval);
}

/*
 * this function deletes and free the current outcoming object of the pipe.
 */

t_error			set_pop_pipe(i_set			setid)
{
  t_error		retval;
  t_iterator		iterator;

  SET_ENTER(set);

  if ((retval = set_tail_ll(setid, &iterator)) != ERROR_NONE)
    SET_LEAVE(set, retval);

  retval = set_delete_ll(setid, iterator);

  SET_LEAVE(set, retval);
}

/*
 * this function returns an iterator on the first node of the pipe
 *
 * useful for the for_each macro.
 */

t_error			set_head_pipe(i_set			setid,
				      t_iterator*		iterator)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_head_ll(setid, iterator);

  SET_LEAVE(set, retval);
}


/*
 * this function returns an iterator on the last node of the pipe.
 *
 * useful for the for_each macro.
 */
t_error			set_tail_pipe(i_set			setid,
				      t_iterator*		iterator)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_tail_ll(setid, iterator);

  SET_LEAVE(set, retval);
}

/*
 * this function returns an iterator on the previous node.
 *
 * useful for the for_each macro.
 */

t_error			set_previous_pipe(i_set			setid,
					  t_iterator		current,
					  t_iterator*		previous)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_previous_ll(setid, current, previous);

  SET_LEAVE(set, retval);
}

/*
 * this function returns an iterator on nthe next node.
 *
 * useful for the for_each macro
 */

t_error			set_next_pipe(i_set			setid,
				      t_iterator		current,
				      t_iterator*		next)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_next_ll(setid, current, next);

  SET_LEAVE(set, retval);
}

/*
 * this function just returns an error.
 *
 * use set_push_pipe instead.
 */

t_error			set_insert_pipe(i_set			setid,
					void*			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * Not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_append_pipe(i_set			setid,
					void*			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * Not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_before_pipe(i_set			setid,
					t_iterator		iterator,
					void*			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * Not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_after_pipe(i_set			setid,
				       t_iterator		iterator,
				       void*			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error.
 *
 * use set_push_pipe instead.
 */

t_error			set_add_pipe(i_set			setid,
				     void*			data)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error.
 *
 * use set_pop_pipe instead.
 */

t_error			set_remove_pipe(i_set			setid,
					t_id			id)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function just returns an error.
 *
 * use set_pop_pipe instead.
 */

t_error			set_delete_pipe(i_set			setid,
					t_iterator		iterator)
{
  SET_ENTER(set);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_locate_pipe(i_set			setid,
					t_id			id,
					t_iterator*		iterator)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_locate_ll(setid, id, iterator);

  SET_LEAVE(set, ERROR_UNKNOWN);
}

/*
 * this function returns the object of the given iterator.
 */

t_error			set_object_pipe(i_set			setid,
					t_iterator		iterator,
					void**			data)
{
  t_error		retval;

  SET_ENTER(set);

  retval = set_object_ll(setid, iterator, data);

  SET_LEAVE(set, retval);
}
