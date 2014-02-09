/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/set.h
 *
 * created       julien quintard   [wed jun  6 11:56:46 2007]
 * updated       julien quintard   [wed dec 15 09:55:26 2010]
 */

#ifndef CORE_SET_H
#define CORE_SET_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the size of a set i.e the number of objects a set contains.
 */

typedef t_sint64		t_setsz;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/set-array.h>
#include <core/set-bpt.h>
#include <core/set-ll.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * iterator's state.
 */

#define ITERATOR_STATE_USED	0x01
#define ITERATOR_STATE_UNUSED	0x02

/*
 * types i.e set implementations.
 */

#define SET_TYPE_ARRAY		0x01
#define SET_TYPE_BPT		0x02
#define SET_TYPE_LL		0x03
#define SET_TYPE_PIPE		0x04
#define SET_TYPE_STACK		0x05

/*
 * options.
 */

#define SET_OPTION_NONE		(0 << 0)
#define SET_OPTION_FORWARD	(1 << 0)
#define SET_OPTION_BACKWARD	(1 << 1)
#define SET_OPTION_CONTAINER	(1 << 2)
#define SET_OPTION_ALLOCATE	(1 << 3)
#define SET_OPTION_FREE		(1 << 4)
#define SET_OPTION_SORT		(1 << 5)
#define SET_OPTION_ORGANISE	(1 << 6)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the iterator type.
 */

typedef struct
{
  union
  {
    s_iterator_array		array;
    s_iterator_bpt		bpt;
    s_iterator_ll		ll;
  }				u;
}				s_iterator;

/*
 * the set object.
 *
 * the 'id' field identifies uniquely the set. the number of elements
 * contained in a set is given by 'size'. likewise, 'datasz' records the
 * size of the objects being stored in this set.
 *
 * the 'type' attribute provides the set implementation and is used
 * by set_trap() to forward the call the the apporpriate set implementation:
 * bpt, ll, array etc. the 'options' attribute contains the options associated
 * with the set.
 *
 * finally the union 'u' includes some set-implementation-specific data.
 */

typedef struct
{
  i_set				id;

  t_setsz			size;

  t_type			type;
  t_options			options;

  t_size			datasz;

  union
  {
    s_set_array			array;
    s_set_bpt			bpt;
    s_set_ll			ll;
  }				u;
}				o_set;

/*
 * the set manager.
 *
 * the 'id' attribute is an identifier object which is used for generating
 * set identifiers.
 *
 * the 'sets' attribute contains the set identifier of the container i.e
 * the set which contains the other sets. indeed, every set is also stored
 * in a set since sets are objects. the 'container' attribute represents
 * the so-called container which holds all the set objects.
 */

typedef struct
{
  o_id				id;

  i_set				sets;

  o_set*			container;
}				m_set;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-functions are wrapper which forward the call to the
 * appopriate set implementation, taking care to provide all the
 * arguments.
 *
 * since the number of arguments is not constrained by these macro-functions
 * a set implementation can accept two arguments for a function while
 * another implementation could accept nine. obviously this means that
 * changing the set type implies updating the source code to adapt
 * such calls but in practice there are not many differences between
 * the implementations as they all comply with the generic set interface.
 *
 * note the specific use of the ## in this macro-function. using the ##
 * with variadic macros enables the use of a variable number of arguments,
 * this number being twelve, three, or even zero.
 *
 * indeed, let us consider the following example
 *
 *   #define printf(_format_, _args_...)				\
 *     module_call(console, print, _format_, _args_)
 *
 *   printf("kaneton\n");
 *
 * this macro function could not work with an empty set or arguments
 * _args_ because the c-preprocess would expand the macro-function to:
 *
 *   module_call(console, print, "kaneton\n", );
 *
 * one can notice that the comma following the format string remains. using
 * the GNU pre-processor specific ##, the comma is removed should the
 * following variadic argument be empty.
 */

#define set_trap(_func_, _id_, _args_...)				\
  (									\
    {									\
      t_uint32	_r_ = STATUS_ERROR;					\
      o_set*	_set_;						\
									\
      if (set_descriptor((_id_), &_set_) == STATUS_OK)			\
        {								\
          switch (_set_->type)						\
            {								\
              case SET_TYPE_ARRAY:					\
                _r_ = _func_##_array((_id_), ##_args_);			\
                break;							\
              case SET_TYPE_BPT:					\
                _r_ = _func_##_bpt((_id_), ##_args_);			\
                break;							\
              case SET_TYPE_LL:						\
                _r_ = _func_##_ll((_id_), ##_args_);			\
                break;							\
              case SET_TYPE_PIPE:					\
                _r_ = _func_##_pipe((_id_), ##_args_);			\
                break;							\
              case SET_TYPE_STACK:					\
                _r_ = _func_##_stack((_id_), ##_args_);			\
                break;							\
            }								\
        }								\
									\
      _r_;								\
    }									\
  )

#define set_reserve(_type_, _args_...)					\
  set_reserve_##_type_(_args_)

#define set_exist(_id_, _args_...)					\
  set_trap(set_exist, _id_, ##_args_)

#define set_release(_id_, _args_...)					\
  set_trap(set_release, _id_, ##_args_)

#define set_head(_id_, _args_...)					\
  set_trap(set_head, _id_, ##_args_)

#define set_tail(_id_, _args_...)					\
  set_trap(set_tail, _id_, ##_args_)

#define set_previous(_id_, _args_...)					\
  set_trap(set_previous, _id_, ##_args_)

#define set_next(_id_, _args_...)					\
  set_trap(set_next, _id_, ##_args_)

#define set_add(_id_, _args_...)					\
  set_trap(set_add, _id_, ##_args_)

#define set_remove(_id_, _args_...)					\
  set_trap(set_remove, _id_, ##_args_)

#define set_delete(_id_, _args_...)					\
  set_trap(set_delete, _id_, ##_args_)

#define set_flush(_id_, _args_...)					\
  set_trap(set_flush, _id_, ##_args_)

#define set_insert(_id_, _args_...)					\
  set_trap(set_insert, _id_, ##_args_)

#define set_append(_id_, _args_...)					\
  set_trap(set_append, _id_, ##_args_)

#define set_before(_id_, _args_...)					\
  set_trap(set_before, _id_, ##_args_)

#define set_after(_id_, _args_...)					\
  set_trap(set_after, _id_, ##_args_)

#define set_locate(_id_, _args_...)					\
  set_trap(set_locate, _id_, ##_args_)

#define set_object(_id_, _args_...)					\
  set_trap(set_object, _id_, ##_args_)

#define set_show(_id_, _args_...)					\
  set_trap(set_show, _id_, ##_args_)

#define set_push(_id_, _args_...)					\
  set_trap(set_push, _id_, ##_args_)

#define set_pop(_id_, _args_...)					\
  set_trap(set_pop, _id_, ##_args_)

#define set_pick(_id_, _args_...)					\
  set_trap(set_pick, _id_, ##_args_)

/*
 * the foreach() macro-function enables the programmer to easily
 * walk through a set.
 *
 * for instance, the following explore a set in the forward way:
 *
 *   i_set	myset;
 *   s_iterator	myiterator;
 *   t_state	mystate;
 *
 *   [...]
 *
 *   set_foreach(SET_OPTION_FORWARD, myset, myiterator, mystate)
 *     {
 *       [...]
 *     }
 */

#define set_foreach(_opt_, _id_, _iterator_, _state_)			\
  for ((_state_) = ITERATOR_STATE_UNUSED;				\
       (((_state_) == ITERATOR_STATE_UNUSED) ?				\
         ((_opt_) == SET_OPTION_FORWARD ?				\
           set_head((_id_), (_iterator_)) == TRUE :		\
           set_tail((_id_), (_iterator_)) == TRUE) :		\
         ((_opt_) == SET_OPTION_FORWARD ?				\
           set_next((_id_), *(_iterator_), (_iterator_)) ==		\
             TRUE :						\
           set_previous((_id_), *(_iterator_), (_iterator_)) ==		\
             TRUE));						\
       (_state_) = ITERATOR_STATE_USED					\
      )

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/set/set.c
 *      ../../core/set/set-array.c
 *      ../../core/set/set-ll.c
 *	../../core/set/set-bpt.c
 *	../../core/set/set-pipe.c
 *	../../core/set/set-stack.c
 */

/*
 * ../../core/set/set.c
 */

t_status		set_dump(void);

t_bool			set_empty(i_set				setid);

t_status		set_size(i_set				setid,
				 t_setsz*			size);

t_status		set_new(o_set*				object);

t_status		set_destroy(i_set			setid);

t_status		set_descriptor(i_set			setid,
				       o_set**			object);

t_status		set_get(i_set				setid,
				t_id				id,
				void**				object);

t_status		set_initialize(void);

t_status		set_clean(void);


/*
 * ../../core/set/set-array.c
 */

t_bool			set_exist_array(i_set			setid,
					t_id			id);

t_status		set_show_array(i_set			setid,
				       mt_margin		margin);

t_bool			set_head_array(i_set			setid,
				       s_iterator*		iterator);

t_bool			set_tail_array(i_set			setid,
				       s_iterator*		iterator);

t_bool			set_previous_array(i_set		setid,
					   s_iterator		current,
					   s_iterator*		previous);

t_bool			set_next_array(i_set			setid,
				       s_iterator		current,
				       s_iterator*		next);

t_status		set_expand_array(o_set			*object,
					 void			*data);

t_status		set_insert_array_at(o_set		*object,
					    t_setsz		pos,
					    void		*data);

t_status		set_insert_array(i_set			setid,
					 void*			data);

t_status		set_append_array(i_set			setid,
					 void*			data);

t_status		set_before_array(i_set			setid,
					 s_iterator		iterator,
					 void*			data);

t_status		set_after_array(i_set			setid,
					s_iterator		iterator,
					void*			data);

t_status		set_add_array(i_set			setid,
				      void*			data);

t_status		set_remove_array(i_set			setid,
					 t_id			id);

t_status		set_delete_array(i_set			setid,
					 s_iterator		iterator);

t_status		set_flush_array(i_set			setid);

t_status		set_locate_array(i_set			setid,
					 t_id			id,
					 s_iterator*		iterator);

t_status		set_object_array(i_set			setid,
					 s_iterator		iterator,
					 void**			data);

t_status		set_reserve_array(t_options		options,
					  t_setsz		initsz,
					  t_size		datasz,
					  i_set*		id);

t_status		set_release_array(i_set			setid);

t_status		set_push_array(i_set			setid,
				       void*			data);

t_status		set_pop_array(i_set			setid);

t_status		set_pick_array(i_set			setid,
				       void**			data);


/*
 * ../../core/set/set-ll.c
 */

t_bool			set_exist_ll(i_set			setid,
				     t_id			id);

t_status		set_show_ll(i_set			setid,
				    mt_margin			margin);

t_bool			set_head_ll(i_set			setid,
				    s_iterator*			iterator);

t_bool			set_tail_ll(i_set			setid,
				    s_iterator*			iterator);

t_bool			set_previous_ll(i_set			setid,
					s_iterator		current,
					s_iterator*		previous);

t_bool			set_next_ll(i_set			setid,
				    s_iterator			current,
				    s_iterator*			next);

t_status		set_insert_ll(i_set			setid,
				      void*			data);

t_status		set_append_ll(i_set			setid,
				      void*			data);

t_status		set_before_ll(i_set			setid,
				      s_iterator		iterator,
				      void*			data);

t_status		set_after_ll(i_set			setid,
				     s_iterator			iterator,
				     void*			data);

t_status		set_add_ll(i_set			setid,
				   void*			data);

t_status		set_remove_ll(i_set			setid,
				      t_id			id);

t_status		set_delete_ll(i_set			setid,
				      s_iterator		iterator);

t_status		set_flush_ll(i_set			setid);

t_status		set_locate_ll(i_set			setid,
				      t_id			id,
				      s_iterator*		iterator);

t_status		set_object_ll(i_set			setid,
				      s_iterator		iterator,
				      void**			data);

t_status		set_reserve_ll(t_options		options,
				       t_size			datasz,
				       i_set*			id);

t_status		set_release_ll(i_set			setid);

t_status		set_push_ll(i_set			setid,
				    void*			data);

t_status		set_pop_ll(i_set			setid);

t_status		set_pick_ll(i_set			setid,
				    void**			data);


/*
 * ../../core/set/set-bpt.c
 */

void			set_load_bpt(t_bpt(set)*		bpt,
				     t_bpt_imm(set)*		node,
				     t_bpt_node(set)		addr);

void			set_unload_bpt(t_bpt(set)*		bpt,
				       t_bpt_imm(set)*		node);

int			set_addrcmp_bpt(t_bpt(set)*		bpt,
					t_bpt_addr(set)		addr1,
					t_bpt_addr(set)		addr2);

int			set_keycmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_key(set)		key1,
				       t_bpt_key(set)		key2);

int			set_valcmp_bpt(t_bpt(set)*		bpt,
				       t_bpt_value(set)		value1,
				       t_bpt_value(set)		value2);

t_bool			set_exist_bpt(i_set			setid,
				      t_id			id);

t_status		set_build_bpt(o_set*			object,
				      BPT_NODESZ_T		nodesz);

t_status		set_adjust_bpt(o_set*			object,
				       t_bpt_uni(set)		alloc,
				       t_bpt_uni(set)		size);

t_status		set_destroy_bpt(o_set*			object);

t_status		set_show_bpt(i_set			setid,
				     mt_margin			margin);

t_bool			set_head_bpt(i_set			setid,
				     s_iterator*		iterator);

t_bool			set_tail_bpt(i_set			setid,
				     s_iterator*		iterator);

t_bool			set_previous_bpt(i_set			setid,
					 s_iterator		current,
					 s_iterator*		previous);

t_bool			set_next_bpt(i_set			setid,
				     s_iterator			current,
				     s_iterator*		next);

t_status		set_insert_bpt(i_set			setid,
				       void*			data);

t_status		set_append_bpt(i_set			setid,
				       void*			data);

t_status		set_before_bpt(i_set			setid,
				       s_iterator		iterator,
				       void*			data);

t_status		set_after_bpt(i_set			setid,
				      s_iterator		iterator,
				      void*			data);

t_status		set_add_bpt(i_set			setid,
				    void*			data);

t_status		set_remove_bpt(i_set			setid,
				       t_id			id);

t_status		set_delete_bpt(i_set			setid,
				       s_iterator		iterator);

t_status		set_flush_bpt(i_set			setid);

t_status		set_locate_bpt(i_set			setid,
				       t_id			id,
				       s_iterator*		iterator);

t_status		set_object_bpt(i_set			setid,
				       s_iterator		iterator,
				       void**			data);

t_status		set_reserve_bpt(t_options		options,
					t_size			datasz,
					t_bpt_nodesz(set)	nodesz,
					i_set*			id);

t_status		set_release_bpt(i_set			setid);

t_status		set_push_bpt(i_set			setid,
				     void*			data);

t_status		set_pop_bpt(i_set			setid);

t_status		set_pick_bpt(i_set			setid,
				     void**			data);


/*
 * ../../core/set/set-pipe.c
 */

t_status		set_reserve_pipe(t_options		options,
					 t_size			datasz,
					 i_set*			id);

t_bool			set_exist_pipe(i_set			setid,
				       t_id			id);

t_status		set_show_pipe(i_set			setid,
				      mt_margin			margin);

t_status		set_release_pipe(i_set			setid);

t_status		set_flush_pipe(i_set			setid);

t_status		set_push_pipe(i_set			setid,
				      void*			data);

t_status		set_pick_pipe(i_set			setid,
				      void**			data);

t_status		set_pop_pipe(i_set			setid);

t_bool			set_head_pipe(i_set			setid,
				      s_iterator*		iterator);

t_bool			set_tail_pipe(i_set			setid,
				      s_iterator*		iterator);

t_bool			set_previous_pipe(i_set			setid,
					  s_iterator		current,
					  s_iterator*		previous);

t_bool			set_next_pipe(i_set			setid,
				      s_iterator		current,
				      s_iterator*		next);

t_status		set_insert_pipe(i_set			setid,
					void*			data);

t_status		set_append_pipe(i_set			setid,
					void*			data);

t_status		set_before_pipe(i_set			setid,
					s_iterator		iterator,
					void*			data);

t_status		set_after_pipe(i_set			setid,
				       s_iterator		iterator,
				       void*			data);

t_status		set_add_pipe(i_set			setid,
				     void*			data);

t_status		set_remove_pipe(i_set			setid,
					t_id			id);

t_status		set_delete_pipe(i_set			setid,
					s_iterator		iterator);

t_status		set_locate_pipe(i_set			setid,
					t_id			id,
					s_iterator*		iterator);

t_status		set_object_pipe(i_set			setid,
					s_iterator		iterator,
					void**			data);


/*
 * ../../core/set/set-stack.c
 */

t_status		set_reserve_stack(t_options		options,
					  t_size		datasz,
					  i_set*		id);

t_bool			set_exist_stack(i_set			setid,
					t_id			id);

t_status		set_show_stack(i_set			setid,
				       mt_margin		margin);

t_status		set_release_stack(i_set			setid);

t_status		set_flush_stack(i_set			setid);

t_status		set_push_stack(i_set			setid,
				       void*			data);

t_status		set_pick_stack(i_set			setid,
				       void**			data);

t_status		set_pop_stack(i_set			setid);

t_bool			set_head_stack(i_set			setid,
				       s_iterator*		iterator);

t_bool			set_tail_stack(i_set			setid,
				       s_iterator*		iterator);

t_bool			set_previous_stack(i_set		setid,
					   s_iterator		current,
					   s_iterator*		previous);

t_bool			set_next_stack(i_set			setid,
				       s_iterator		current,
				       s_iterator*		next);

t_status		set_insert_stack(i_set			setid,
					 void*			data);

t_status		set_append_stack(i_set			setid,
					 void*			data);

t_status		set_before_stack(i_set			setid,
					 s_iterator		iterator,
					 void*			data);

t_status		set_after_stack(i_set			setid,
					s_iterator		iterator,
					void*			data);

t_status		set_add_stack(i_set			setid,
				      void*			data);

t_status		set_remove_stack(i_set			setid,
					 t_id			id);

t_status		set_delete_stack(i_set			setid,
					 s_iterator		iterator);

t_status		set_locate_stack(i_set			setid,
					 t_id			id,
					 s_iterator*		iterator);

t_status		set_object_stack(i_set			setid,
					 s_iterator		iterator,
					 void**			data);


/*
 * eop
 */

#endif
