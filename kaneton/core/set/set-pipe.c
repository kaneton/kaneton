/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/set/set-pipe.c
 *
 * created       renaud voltz   [wed jan 25 17:11:05 2006]
 * updated       julien quintard   [sun jan 30 20:35:16 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this set implementation provides a FIFO data structure.
 *
 * note that the implementation actually relies on the linked-list
 * implementation.
 *
 * options:
 *   SET_OPTION_ALLOCATE
 *   SET_OPTION_FREE
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the set manager's structure.
 */

extern m_set		_set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reserves a set according to the given options.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) reserves an identifier.
 * 2) initialize and fill the set descriptor.
 * 3) register the set descriptor.
 */

t_status		set_reserve_pipe(t_options		options,
					 t_size			datasz,
					 i_set*			id)
{
  o_set			o;

  /*
   * 0)
   */

  if (datasz < sizeof (t_id))
    CORE_ESCAPE("unable to reserve a set for objects smaller than "
		"an identifier");

  if (id == NULL)
    CORE_ESCAPE("the 'id' argument is null");

  if (options & SET_OPTION_CONTAINER)
    CORE_ESCAPE("this type of set cannot be used as a container");

  if (options & SET_OPTION_ORGANISE)
    CORE_ESCAPE("this type of set does not support the organise option");

  if ((options & SET_OPTION_ALLOCATE) && (options & SET_OPTION_FREE))
    CORE_ESCAPE("unable to reserve a set with both alloc and free options");

  /*
   * 1)
   */

  if (id_reserve(&_set.id, id) != STATUS_OK)
    CORE_ESCAPE("unable to reserve the set identifier");

  /*
   * 2)
   */

  memset(&o, 0x0, sizeof (o_set));

  o.id = *id;
  o.size = 0;
  o.type = SET_TYPE_PIPE;
  o.options = options;
  o.datasz = datasz;

  o.u.ll.head = NULL;
  o.u.ll.tail = NULL;

  /*
   * 3)
   */

  if (set_new(&o) != STATUS_OK)
    CORE_ESCAPE("unable to register the set descriptor");

  CORE_LEAVE();
}

/*
 * this function just returns an error.
 */

t_bool			set_exist_pipe(i_set			setid,
				       t_id			id)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function shows the set's attributes.
 */

t_status		set_show_pipe(i_set			setid,
				      mt_margin			margin)
{
  if (set_show_ll(setid, margin) != STATUS_OK)
    CORE_ESCAPE("unable to show the linked-list set");

  CORE_LEAVE();
}

/*
 * this function releases a set.
 */

t_status		set_release_pipe(i_set			setid)
{
  if (set_release_ll(setid) != STATUS_OK)
    CORE_ESCAPE("unable to release the linked-list set");

  CORE_LEAVE();
}

/*
 * this function flushes the set.
 */

t_status		set_flush_pipe(i_set			setid)
{
  if (set_flush_ll(setid) != STATUS_OK)
    CORE_ESCAPE("unable to flush the linked-list set");

  CORE_LEAVE();
}

/*
 * this function adds an object to the pipe.
 */

t_status		set_push_pipe(i_set			setid,
				      void*			data)
{
  if (set_insert_ll(setid, data) != STATUS_OK)
    CORE_ESCAPE("unable to insert in the linked-list set");

  CORE_LEAVE();
}


/*
 * this function returns the about-to-get-out object from the pipe.
 *
 * steps:
 *
 * 1) locate the last object.
 * 2) retrieve it.
 */

t_status		set_pick_pipe(i_set			setid,
				      void**			data)
{
  s_iterator		iterator;

  /*
   * 1)
   */

  if (set_tail_ll(setid, &iterator) != TRUE)
    CORE_ESCAPE("unable to locate the tail object in the linked-list set");

  /*
   * 2)
   */

  if (set_object_ll(setid, iterator, data) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object");

  CORE_LEAVE();
}

/*
 * this function deletes the about-to-get-out object.
 *
 * steps:
 *
 * 1) locate the last object.
 * 2) delete it.
 */

t_status		set_pop_pipe(i_set			setid)
{
  s_iterator		iterator;

  /*
   * 1)
   */

  if (set_tail_ll(setid, &iterator) != TRUE)
    CORE_ESCAPE("unable to locate the tail object in the linked-list set");

  /*
   * 2)
   */

  if (set_delete_ll(setid, iterator) != STATUS_OK)
    CORE_ESCAPE("unable to delete the object");

  CORE_LEAVE();
}

/*
 * this function returns an iterator on the first element of the pipe
 */

t_bool			set_head_pipe(i_set			setid,
				      s_iterator*		iterator)
{
  if (set_head_ll(setid, iterator) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}


/*
 * this function returns an iterator on the last element of the pipe.
 */

t_bool			set_tail_pipe(i_set			setid,
				      s_iterator*		iterator)
{
  if (set_tail_ll(setid, iterator) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns an iterator on the previous node.
 */

t_bool			set_previous_pipe(i_set			setid,
					  s_iterator		current,
					  s_iterator*		previous)
{
  if (set_previous_ll(setid, current, previous) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function returns an iterator on the next node.
 */

t_bool			set_next_pipe(i_set			setid,
				      s_iterator		current,
				      s_iterator*		next)
{
  if (set_next_ll(setid, current, next) != TRUE)
    CORE_FALSE();

  CORE_TRUE();
}

/*
 * this function just returns an error.
 */

t_status		set_insert_pipe(i_set			setid,
					void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_append_pipe(i_set			setid,
					void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_before_pipe(i_set			setid,
					s_iterator		iterator,
					void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_after_pipe(i_set			setid,
				       s_iterator		iterator,
				       void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_add_pipe(i_set			setid,
				     void*			data)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_remove_pipe(i_set			setid,
					t_id			id)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_delete_pipe(i_set			setid,
					s_iterator		iterator)
{
  CORE_ESCAPE("this type of set does not support this operation");
}

/*
 * this function just returns an error.
 */

t_status		set_locate_pipe(i_set			setid,
					t_id			id,
					s_iterator*		iterator)
{
  if (set_locate_ll(setid, id, iterator) != STATUS_OK)
    CORE_ESCAPE("unable to locate the object from the linked-list set");

  CORE_LEAVE();
}

/*
 * this function returns the object on which the given iterator points to.
 */

t_status		set_object_pipe(i_set			setid,
					s_iterator		iterator,
					void**			data)
{
  if (set_object_ll(setid, iterator, data) != STATUS_OK)
    CORE_ESCAPE("unable to retrieve the object from the linked-list set");

  CORE_LEAVE();
}
