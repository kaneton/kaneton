/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Thu Oct 13 21:53:33 2005   mycure
 */

#ifndef KANETON_SET_H
#define KANETON_SET_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/id.h>
#include <kaneton/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/* XXX */
#define stderr NULL
#define BPT_DEBUG
/* XXX */

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
 * iterator's forward declaration
 */

typedef struct s_iterator	t_iterator;

/*
 * set size
 */

typedef t_sint64		t_setsz;

/*
 * set object's forward declaration
 */

typedef struct s_set		o_set;

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton/set_array.h>
#include <kaneton/set_bpt.h>
#include <kaneton/set_ll.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * iterator
 */

struct				s_iterator
{
  union
  {
    t_iterator_array		array;
    t_iterator_bpt		bpt;
    t_iterator_ll		ll;
  }				u;
};

/*
 * set object
 *
 * the setid field represent the set identifier used to retrieve the set
 * data structure. the union is composed of subparts dependending of the
 * set type.
 */

struct				s_set
{
  t_setid			setid;

  t_setsz			size;

  t_type			type;

  union
  {
    t_set_array			array;
    t_set_bpt			bpt;
    t_set_ll			ll;
  }				u;
};

/*
 * set manager
 */

typedef struct
{
  o_id				id;

  t_id				setid;
  o_set*			container;
}				m_set;

/*
 * ---------- macros ----------------------------------------------------------
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

# define set_debug(_func_, _setid_, _args_...)				\
  fprintf(stderr, "[setd] trap: %s(%qu, %s)\n",				\
          #_func_,							\
          _setid_,							\
          #_args_);

#else

# define set_debug(_func_, _setid_, _args...)

#endif

/*
 * traps
 */

#define set_trap(_func_, _setid_, _args_...)				\
  (									\
    {									\
      t_error		_r_ = ERROR_UNKNOWN;				\
      o_set*		_set_;						\
									\
      set_debug(_func_, _setid_, _args_);				\
									\
      if (set_descriptor((_setid_), &_set_) == ERROR_NONE)		\
        {								\
          switch (_set_->type)						\
            {								\
              case SET_TYPE_ARRAY:					\
                _r_ = _func_##_array((_setid_), ##_args_);		\
                break;							\
              case SET_TYPE_BPT:					\
                _r_ = _func_##_bpt((_setid_), ##_args_);		\
                break;							\
              case SET_TYPE_LL:						\
                _r_ = _func_##_ll((_setid_), ##_args_);			\
                break;							\
            }								\
        }								\
      _r_;								\
    }									\
  )

#define set_type(_type_, _setid_)					\
  set_type_##_type_(_setid_)

#define set_rsv(_type_, _args_...)					\
  set_rsv_##_type_(_args_)

#define set_rel(_setid_, _args_...)					\
  set_trap(set_rel, _setid_, ##_args_)

#define set_head(_setid_, _args_...)					\
  set_trap(set_head, _setid_, ##_args_)

#define set_tail(_setid_, _args_...)					\
  set_trap(set_tail, _setid_, ##_args_)

#define set_prev(_setid_, _args_...)					\
  set_trap(set_prev, _setid_, ##_args_)

#define set_next(_setid_, _args_...)					\
  set_trap(set_next, _setid_, ##_args_)

#define set_add(_setid_, _args_...)					\
  set_trap(set_add, _setid_, ##_args_)

#define set_remove(_setid_, _args_...)					\
  set_trap(set_remove, _setid_, ##_args_)

#define set_flush(_setid_, _args_...)					\
  set_trap(set_flush, _setid_, ##_args_)

#define set_insert_head(_setid_, _args_...)				\
  set_trap(set_insert_head, _setid_, ##_args_)

#define set_insert_tail(_setid_, _args_...)				\
  set_trap(set_insert_tail, _setid_, ##_args_)

#define set_insert_before(_setid_, _args_...)				\
  set_trap(set_insert_before, _setid_, ##_args_)

#define set_insert_after(_setid_, _args_...)				\
  set_trap(set_insert_after, _setid_, ##_args_)

#define set_locate(_setid_, _args_...)					\
  set_trap(set_locate, _setid_, ##_args_)

#define set_object(_setid_, _args_...)					\
  set_trap(set_object, _setid_, ##_args_)

#define set_dump(_setid_, _args_...)					\
  set_trap(set_dump, _setid_, ##_args_)

/*
 * foreach
 */

#define set_foreach(_opt_, _setid_, _iterator_, _state_)		\
  for ((_state_) = ITERATOR_STATE_UNUSED;				\
        (((_state_) == ITERATOR_STATE_UNUSED) ?				\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_head((_setid_), (_iterator_)) == ERROR_NONE :		\
            set_tail((_setid_), (_iterator_)) == ERROR_NONE) :		\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_next((_setid_), *(_iterator_), (_iterator_)) ==		\
              ERROR_NONE :						\
            set_prev((_setid_), *(_iterator_), (_iterator_)) ==		\
              ERROR_NONE));						\
	 (_state_) = ITERATOR_STATE_USED				\
       )

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set.c
 *      ../../kaneton/set/set_array.c
 *      ../../kaneton/set/set_ll.c
 */

/*
 * ../../kaneton/set/set.c
 */

t_error			set_type(t_setid			setid,
				 t_type*			type);

t_error			set_size(t_setid			setid,
				 t_setsz*			size);

t_error			set_new(o_set*				o);

t_error			set_delete(t_setid			setid);

t_error			set_descriptor(t_setid			setid,
				       o_set**			o);

t_error			set_get(t_setid				setid,
				t_id				id,
				void**				o);

t_error			set_init(void);

t_error			set_clean(void);

t_error			set_test(t_type				type);


/*
 * ../../kaneton/set/set_array.c
 */

t_error			set_dump_array(t_setid			setid);

t_error			set_expand_array(o_set*			o);

t_error			set_insert_array(o_set*			o,
					 t_sint32		position);

t_error			set_head_array(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_tail_array(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous);

t_error			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next);

t_error			set_insert_head_array(t_setid		setid,
					      void*		data);

t_error			set_insert_tail_array(t_setid		setid,
					      void*		data);

t_error			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data);

t_error			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data);

t_error			set_add_array(t_setid			setid,
				      void*			data);

t_error			set_remove_array(t_setid		setid,
					 t_id			id);

t_error			set_flush_array(t_setid			setid);

t_error			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator);

t_error			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data);

t_error			set_rsv_array(t_opts			opts,
				      t_setsz			initsz,
				      t_size			datasz,
				      t_setid*			setid);

t_error			set_rel_array(t_setid			setid);


/*
 * ../../kaneton/set/set_ll.c
 */

t_error			set_dump_ll(t_setid			setid);

t_error			set_head_ll(t_setid			setid,
				    t_iterator*			iterator);

t_error			set_tail_ll(t_setid			setid,
				    t_iterator*			iterator);

t_error			set_prev_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			previous);

t_error			set_next_ll(t_setid			setid,
				    t_iterator			current,
				    t_iterator*			next);

t_error			set_insert_head_ll(t_setid		setid,
					   void*		data);

t_error			set_insert_tail_ll(t_setid		setid,
					   void*		data);

t_error			set_insert_before_ll(t_setid		setid,
					     t_iterator		iterator,
					     void*		data);

t_error			set_insert_after_ll(t_setid		setid,
					    t_iterator		iterator,
					    void*		data);

t_error			set_add_ll(t_setid			setid,
				   void*			data);

t_error			set_remove_ll(t_setid			setid,
				      t_id			id);

t_error			set_flush_ll(t_setid			setid);

t_error			set_locate_ll(t_setid			setid,
				      t_id			id,
				      t_iterator*		iterator);

t_error			set_object_ll(t_setid			setid,
				      t_iterator		iterator,
				      void**			data);

t_error			set_rsv_ll(t_opts			opts,
				   t_size			datasz,
				   t_setid*			setid);

t_error			set_rel_ll(t_setid			setid);


/*
 * eop
 */

#endif
