/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/kaneton/core/include/set.h
 *
 * created       julien quintard   [wed jun  6 11:56:46 2007]
 * updated       matthieu bucchianeri   [thu sep 13 16:49:37 2007]
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
 * set size
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
 * iterator's state
 */

#define ITERATOR_STATE_USED	0x01
#define ITERATOR_STATE_UNUSED	0x02

/*
 * types
 */

#define SET_TYPE_ARRAY		0x01
#define SET_TYPE_BPT		0x02
#define SET_TYPE_LL		0x03
#define SET_TYPE_PIPE		0x04
#define SET_TYPE_STACK		0x05

/*
 * options
 */

#define SET_OPT_NONE		(0 << 0)
#define SET_OPT_FORWARD		(1 << 0)
#define SET_OPT_BACKWARD	(1 << 1)
#define SET_OPT_CONTAINER	(1 << 2)
#define SET_OPT_ALLOC		(1 << 3)
#define SET_OPT_FREE		(1 << 4)
#define SET_OPT_SORT		(1 << 5)
#define SET_OPT_ORGANISE	(1 << 6)

/*
 * trap debug
 */

#undef SET_DEBUG_TRAP

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * iterator
 */

typedef struct
{
  union
  {
    t_iterator_array		array;
    t_iterator_bpt		bpt;
    t_iterator_ll		ll;
  }				u;
}				t_iterator;

/*
 * set object
 *
 * the id field represent the set identifier used to retrieve the set
 * data structure. the union is composed of subparts dependending of the
 * set type.
 */

typedef struct
{
  i_set				setid;

  t_setsz			size;

  t_type			type;
  t_opts			opts;

  t_size			datasz;

  union
  {
    t_set_array			array;
    t_set_bpt			bpt;
    t_set_ll			ll;
  }				u;
}				o_set;

/*
 * set manager
 */

typedef struct
{
  o_id				id;

  t_id				sets;
  o_set*			container;
}				m_set;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define SET_CHECK(_set_)						\
  {									\
    if ((_set_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define SET_ENTER(_set_)						\
  {									\
    SET_CHECK((_set_));							\
  }

/*
 * leave
 */

#define SET_LEAVE(_set_, _error_)					\
  {									\
    return (_error_);							\
  }

/*
 * debug
 */

#if (DEBUG & DEBUG_SET) && defined(SET_DEBUG_TRAP)

#define set_debug(_func_, _id_, _args_...)				\
  fprintf(stderr, "[setd] trap: %s(%qu, %s)\n",				\
          #_func_,							\
          _id_,								\
          #_args_);

#else

#define set_debug(_func_, _id_, _args_...)

#endif

/*
 * traps
 */

#define set_trap(_func_, _id_, _args_...)				\
  (									\
    {									\
      t_error		_r_ = ERROR_UNKNOWN;				\
      o_set*		_set_;						\
									\
      set_debug(_func_, _id_, _args_);					\
									\
      if (set_descriptor((_id_), &_set_) == ERROR_NONE)			\
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
      _r_;								\
    }									\
  )

#define set_type(_type_, _id_)						\
  set_type_##_type_(_id_)

#define set_reserve(_type_, _args_...)					\
  set_reserve_##_type_(_args_)

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
 * foreach
 */

#define set_foreach(_opt_, _id_, _iterator_, _state_)			\
  for ((_state_) = ITERATOR_STATE_UNUSED;				\
        (((_state_) == ITERATOR_STATE_UNUSED) ?				\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_head((_id_), (_iterator_)) == ERROR_NONE :		\
            set_tail((_id_), (_iterator_)) == ERROR_NONE) :		\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_next((_id_), *(_iterator_), (_iterator_)) ==		\
              ERROR_NONE :						\
            set_previous((_id_), *(_iterator_), (_iterator_)) ==	\
              ERROR_NONE));						\
	 (_state_) = ITERATOR_STATE_USED				\
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

t_error			set_dump(void);

t_error			set_size(i_set				setid,
				 t_setsz*			size);

t_error			set_new(o_set*				o);

t_error			set_destroy(i_set			setid);

t_error			set_descriptor(i_set			setid,
				       o_set**			o);

t_error			set_get(i_set				setid,
				t_id				id,
				void**				o);

t_error			set_initialize(void);

t_error			set_clean(void);


/*
 * ../../core/set/set-array.c
 */

t_error			set_type_array(i_set			setid);

t_error			set_show_array(i_set			setid);

t_error			set_head_array(i_set			setid,
				       t_iterator*		iterator);

t_error			set_tail_array(i_set			setid,
				       t_iterator*		iterator);

t_error			set_previous_array(i_set		setid,
					   t_iterator		current,
					   t_iterator*		previous);

t_error			set_next_array(i_set			setid,
				       t_iterator		current,
				       t_iterator*		next);

t_error			set_insert_array(i_set			setid,
					 void*			data);

t_error			set_append_array(i_set			setid,
					 void*			data);

t_error			set_before_array(i_set			setid,
					 t_iterator		iterator,
					 void*			data);

t_error			set_after_array(i_set			setid,
					t_iterator		iterator,
					void*			data);

t_error			set_add_array(i_set			setid,
				      void*			data);

t_error			set_remove_array(i_set			setid,
					 t_id			id);

t_error			set_delete_array(i_set			setid,
					 t_iterator		iterator);

t_error			set_flush_array(i_set			setid);

t_error			set_locate_array(i_set			setid,
					 t_id			id,
					 t_iterator*		iterator);

t_error			set_object_array(i_set			setid,
					 t_iterator		iterator,
					 void**			data);

t_error			set_reserve_array(t_opts		opts,
					  t_setsz		initsz,
					  t_size		datasz,
					  i_set*		setid);

t_error			set_release_array(i_set		setid);

t_error			set_push_array(i_set			setid,
				       void*			data);

t_error			set_pop_array(i_set			setid);

t_error			set_pick_array(i_set			setid,
				       void**			data);


/*
 * ../../core/set/set-ll.c
 */

t_error			set_type_ll(i_set			setid);

t_error			set_show_ll(i_set			setid);

t_error			set_head_ll(i_set			setid,
				    t_iterator*			iterator);

t_error			set_tail_ll(i_set			setid,
				    t_iterator*			iterator);

t_error			set_previous_ll(i_set			setid,
					t_iterator		current,
					t_iterator*		previous);

t_error			set_next_ll(i_set			setid,
				    t_iterator			current,
				    t_iterator*			next);

t_error			set_insert_ll(i_set			setid,
				      void*			data);

t_error			set_append_ll(i_set			setid,
				      void*			data);

t_error			set_before_ll(i_set			setid,
				      t_iterator		iterator,
				      void*			data);

t_error			set_after_ll(i_set			setid,
				     t_iterator			iterator,
				     void*			data);

t_error			set_add_ll(i_set			setid,
				   void*			data);

t_error			set_remove_ll(i_set			setid,
				      t_id			id);

t_error			set_delete_ll(i_set			setid,
				      t_iterator		iterator);

t_error			set_flush_ll(i_set			setid);

t_error			set_locate_ll(i_set			setid,
				      t_id			id,
				      t_iterator*		iterator);

t_error			set_object_ll(i_set			setid,
				      t_iterator		iterator,
				      void**			data);

t_error			set_reserve_ll(t_opts			opts,
				       t_size			datasz,
				       i_set*			setid);

t_error			set_release_ll(i_set			setid);

t_error			set_push_ll(i_set			setid,
				    void*			data);

t_error			set_pop_ll(i_set			setid);

t_error			set_pick_ll(i_set			setid,
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

t_error			set_show_unused_bpt(o_set*		o);

t_error			set_type_bpt(i_set			setid);

t_error			set_build_bpt(o_set*			o,
				      BPT_NODESZ_T		nodesz);

t_error			set_adjust_bpt(o_set*			o,
				       t_bpt_uni(set)		alloc,
				       t_bpt_uni(set)		size);

t_error			set_destroy_bpt(o_set*			o);

t_error			set_show_bpt(i_set			setid);

t_error			set_head_bpt(i_set			setid,
				     t_iterator*		iterator);

t_error			set_tail_bpt(i_set			setid,
				     t_iterator*		iterator);

t_error			set_previous_bpt(i_set			setid,
					 t_iterator		current,
					 t_iterator*		previous);

t_error			set_next_bpt(i_set			setid,
				     t_iterator			current,
				     t_iterator*		next);

t_error			set_insert_bpt(i_set			setid,
				       void*			data);

t_error			set_append_bpt(i_set			setid,
				       void*			data);

t_error			set_before_bpt(i_set			setid,
				       t_iterator		iterator,
				       void*			data);

t_error			set_after_bpt(i_set			setid,
				      t_iterator		iterator,
				      void*			data);

t_error			set_add_bpt(i_set			setid,
				    void*			data);

t_error			set_remove_bpt(i_set			setid,
				       t_id			id);

t_error			set_delete_bpt(i_set			setid,
				       t_iterator		iterator);

t_error			set_flush_bpt(i_set			setid);

t_error			set_locate_bpt(i_set			setid,
				       t_id			id,
				       t_iterator*		iterator);

t_error			set_object_bpt(i_set			setid,
				       t_iterator		iterator,
				       void**			data);

t_error			set_reserve_bpt(t_opts			opts,
					t_size			datasz,
					t_bpt_nodesz(set)	nodesz,
					i_set*			setid);

t_error			set_release_bpt(i_set			setid);

t_error			set_push_bpt(i_set			setid,
				     void*			data);

t_error			set_pop_bpt(i_set			setid);

t_error			set_pick_bpt(i_set			setid,
				     void**			data);


/*
 * ../../core/set/set-pipe.c
 */

t_error			set_type_pipe(i_set			setid);

t_error			set_reserve_pipe(t_opts			opts,
					 t_size			datasz,
					 i_set*			setid);

t_error			set_show_pipe(i_set			setid);

t_error			set_release_pipe(i_set		setid);

t_error			set_flush_pipe(i_set			setid);

t_error			set_push_pipe(i_set			setid,
				      void*			data);

t_error			set_pick_pipe(i_set			setid,
				      void**			data);

t_error			set_pop_pipe(i_set			setid);

t_error			set_head_pipe(i_set			setid,
				      t_iterator*		iterator);

t_error			set_tail_pipe(i_set			setid,
				      t_iterator*		iterator);

t_error			set_previous_pipe(i_set			setid,
					  t_iterator		current,
					  t_iterator*		previous);

t_error			set_next_pipe(i_set			setid,
				      t_iterator		current,
				      t_iterator*		next);

t_error			set_insert_pipe(i_set			setid,
					void*			data);

t_error			set_append_pipe(i_set			setid,
					void*			data);

t_error			set_before_pipe(i_set			setid,
					t_iterator		iterator,
					void*			data);

t_error			set_after_pipe(i_set			setid,
				       t_iterator		iterator,
				       void*			data);

t_error			set_add_pipe(i_set			setid,
				     void*			data);

t_error			set_remove_pipe(i_set			setid,
					t_id			id);

t_error			set_delete_pipe(i_set			setid,
					t_iterator		iterator);

t_error			set_locate_pipe(i_set			setid,
					t_id			id,
					t_iterator*		iterator);

t_error			set_object_pipe(i_set			setid,
					t_iterator		iterator,
					void**			data);


/*
 * ../../core/set/set-stack.c
 */

t_error			set_type_stack(i_set			setid);

t_error			set_reserve_stack(t_opts		opts,
					  t_size		datasz,
					  i_set*		setid);

t_error			set_show_stack(i_set			setid);

t_error			set_release_stack(i_set			setid);

t_error			set_flush_stack(i_set			setid);

t_error			set_push_stack(i_set			setid,
				       void*			data);

t_error			set_pick_stack(i_set			setid,
				       void**			data);

t_error			set_pop_stack(i_set			setid);

t_error			set_head_stack(i_set			setid,
				       t_iterator*		iterator);

t_error			set_tail_stack(i_set			setid,
				       t_iterator*		iterator);

t_error			set_previous_stack(i_set		setid,
					   t_iterator		current,
					   t_iterator*		previous);

t_error			set_next_stack(i_set			setid,
				       t_iterator		current,
				       t_iterator*		next);

t_error			set_insert_stack(i_set			setid,
					 void*			data);

t_error			set_append_stack(i_set			setid,
					 void*			data);

t_error			set_before_stack(i_set			setid,
					 t_iterator		iterator,
					 void*			data);

t_error			set_after_stack(i_set			setid,
					t_iterator		iterator,
					void*			data);

t_error			set_add_stack(i_set			setid,
				      void*			data);

t_error			set_remove_stack(i_set			setid,
					 t_id			id);

t_error			set_delete_stack(i_set			setid,
					 t_iterator		iterator);

t_error			set_locate_stack(i_set			setid,
					 t_id			id,
					 t_iterator*		iterator);

t_error			set_object_stack(i_set			setid,
					 t_iterator		iterator,
					 void**			data);


/*
 * eop
 */

#endif
