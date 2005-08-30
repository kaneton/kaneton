/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Tue Aug 30 13:16:44 2005   mycure
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

/*
 * types
 */

#define SET_TYPE_ARRAY		0x01
#define SET_TYPE_BPT		0x02
#define SET_TYPE_LL		0x03

/*
 * options
 */

#define SET_OPT_NONE		0x00
#define SET_OPT_FORWARD		0x01
#define SET_OPT_BACKWARD	0x02
#define SET_OPT_CONTAINER	0x04
#define SET_OPT_ALLOC		0x08
#define SET_OPT_SORT		0x10
#define SET_OPT_ORGANISE	0x20

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * iterator
 */

typedef void*			t_iterator;

/*
 * set size
 */

typedef t_uint64		t_setsz;

/*
 * set object forward declaration
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
 * set object
 *
 * the setid field represent the set identifier used to retrieve the set
 * data structure. the union is composed of subparts dependending of the
 * set type.
 */

struct				s_set
{
  t_setid			id;

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
 * ---------- extern ----------------------------------------------------------
 */

extern m_set*		set;

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
 * traps
 */

#define set_trap(_func_, _setid_, _args_...)				\
  (									\
    {									\
      int		_r_ = ERROR_UNKNOWN;				\
      o_set*		_set_;						\
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

#define set_foreach(_opt_, _setid_, _iterator_)				\
  for (*(_iterator_) = NULL;						\
        ((*(_iterator_) == NULL) ?					\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_head((_setid_), (_iterator_)) == ERROR_NONE :		\
            set_tail((_setid_), (_iterator_)) == ERROR_NONE) :		\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_next((_setid_), *(_iterator_), (_iterator_)) ==		\
              ERROR_NONE :						\
            set_prev((_setid_), *(_iterator_), (_iterator_)) ==		\
              ERROR_NONE));						\
       )

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set.c
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
 * eop
 */

#endif
