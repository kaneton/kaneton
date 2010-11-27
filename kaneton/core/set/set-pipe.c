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

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_set*		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function tells if the set object is a pipe set.
 */

t_error			set_type_pipe(i_set			setid)
{
  o_set*		o;

  if (set_descriptor(setid, &o) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the set descriptor");

  if (o->type != SET_TYPE_PIPE)
    CORE_ESCAPE("invalid set type");

  CORE_LEAVE();
}

/*
 * this function reserves a set.
 *
 * steps:
 *
 * 1) initializes the set descriptor.
 * 2) avoids bad options.
 * 3) if necessary, reserves an unused identifier for this new set.
 * 4) initializes the set descriptor fields.
 * 5) adds the set descriptor to the set container.
 */

t_error			set_reserve_pipe(t_options		options,
					 t_size			datasz,
					 i_set*			setid)
{
  o_set			o;

  assert(datasz >= sizeof (t_id));
  assert(setid != NULL);

  /*
   * 1)
   */

  memset(&o, 0x0, sizeof(o_set));

  /*
   * 2)
   */

  if (options & SET_OPTION_ORGANISE)
    CORE_ESCAPE("this type of set does not support the organise option");

  if ((options & SET_OPTION_ALLOC) && (options & SET_OPTION_FREE))
    CORE_ESCAPE("unable to reserve a set with both alloc and free options");

  /*
   * 3)
   */

  if (options & SET_OPTION_CONTAINER)
    {
      *setid = _set->sets;
    }
  else
    {
      if (id_reserve(&_set->id, setid) != ERROR_OK)
	CORE_ESCAPE("unable to reserve the set identifier");
    }

  /*
   * 4)
   */

  o.id = *setid;
  o.size = 0;
  o.type = SET_TYPE_PIPE;
  o.options = options;
  o.datasz = datasz;

  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 5)
   */

  if (set_new(&o) != ERROR_OK)
    {
      if (!(options & SET_OPTION_CONTAINER))
	id_release(&_set->id, o.id);

      CORE_ESCAPE("unable to register the set descriptor");
    }

  CORE_LEAVE();
}

/*
 * not relevant to the pipe.
 *
 * this function just returns an error.
 */

t_error			set_exist_pipe(i_set			setid,
				       t_id			id)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function shows set objects contained in the whole pipe.
 *
 */

t_error			set_show_pipe(i_set			setid)
{
  if (set_show_ll(setid) != ERROR_OK)
    CORE_ESCAPE("unable to show the linked-list set");

  CORE_LEAVE();
}

/*
 * this function releases a set_pipe.
 */

t_error			set_release_pipe(i_set		setid)
{
  if (set_release_ll(setid) != ERROR_OK)
    CORE_ESCAPE("unable to release the linked-list set");

  CORE_LEAVE();
}

/*
 * this function flushes the set_pipe and free every element.
 */

t_error			set_flush_pipe(i_set			setid)
{
  if (set_flush_ll(setid) != ERROR_OK)
    CORE_ESCAPE("unable to flush the linked-list set");

  CORE_LEAVE();
}

/*
 * this function add an object into the pipe.
 */

t_error			set_push_pipe(i_set			setid,
				      void*			data)
{
  if (set_insert_ll(setid, data) != ERROR_OK)
    CORE_ESCAPE("unable to insert in the linked-list set");

  CORE_LEAVE();
}


/*
 * this function returns current outcomimg object of the pipe.
 */

t_error			set_pick_pipe(i_set			setid,
				      void**			data)
{
  t_iterator		iterator;

  if (set_tail_ll(setid, &iterator) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the tail object in the linked-list set");

  if (set_object_ll(setid, iterator, data) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object");

  CORE_LEAVE();
}

/*
 * this function deletes and free the current outcoming object of the pipe.
 */

t_error			set_pop_pipe(i_set			setid)
{
  t_iterator		iterator;

  if (set_tail_ll(setid, &iterator) != ERROR_TRUE)
    CORE_ESCAPE("unable to locate the tail object in the linked-list set");

  if (set_delete_ll(setid, iterator) != ERROR_OK)
    CORE_ESCAPE("unable to delete the object");

  CORE_LEAVE();
}

/*
 * this function returns an iterator on the first node of the pipe
 *
 * useful for the for_each macro.
 */

t_error			set_head_pipe(i_set			setid,
				      t_iterator*		iterator)
{
  if (set_head_ll(setid, iterator) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}


/*
 * this function returns an iterator on the last node of the pipe.
 *
 * useful for the for_each macro.
 */
t_error			set_tail_pipe(i_set			setid,
				      t_iterator*		iterator)
{
  if (set_tail_ll(setid, iterator) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
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
  if (set_previous_ll(setid, current, previous) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
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
  if (set_next_ll(setid, current, next) != ERROR_TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function just returns an error.
 *
 * use set_push_pipe instead.
 */

t_error			set_insert_pipe(i_set			setid,
					void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * Not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_append_pipe(i_set			setid,
					void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * Not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_before_pipe(i_set			setid,
					t_iterator		iterator,
					void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * Not relevant to the pipe.
 * this function just returns an error.
 */

t_error			set_after_pipe(i_set			setid,
				       t_iterator		iterator,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 *
 * use set_push_pipe instead.
 */

t_error			set_add_pipe(i_set			setid,
				     void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just calls the corresponding function for the ll set
 * underlying implementation.
 */

t_error			set_remove_pipe(i_set			setid,
					t_id			id)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 *
 * use set_pop_pipe instead.
 */

t_error			set_delete_pipe(i_set			setid,
					t_iterator		iterator)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * not relevant to the pipe.
 *
 * this function just returns an error.
 */

t_error			set_locate_pipe(i_set			setid,
					t_id			id,
					t_iterator*		iterator)
{
  if (set_locate_ll(setid, id, iterator) != ERROR_OK)
    CORE_ESCAPE("unable to locate the object from the linked-list set");

  CORE_LEAVE();
}

/*
 * this function returns the object of the given iterator.
 */

t_error			set_object_pipe(i_set			setid,
					t_iterator		iterator,
					void**			data)
{
  if (set_object_ll(setid, iterator, data) != ERROR_OK)
    CORE_ESCAPE("unable to retrieve the object from the linked-list set");

  CORE_LEAVE();
}
