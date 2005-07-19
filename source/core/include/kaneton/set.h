/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Tue Jul 19 14:18:26 2005   mycure
 */

#ifndef KANETON_SET_H
#define KANETON_SET_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton/set_ll.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * foreach
 */

#define SET_OPT_FORWARD		0x1
#define SET_OPT_BACKWARD	0x2

/*
 * options
 */

#define SET_OPT_NONE		0x00
#define SET_OPT_ALLOC		0x01

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

typedef struct
{
  t_setid			id;

  t_setsz			size;

  t_type			type;

  union
  {
    t_set_ll			ll;
  }				u;
}				o_set;

/*
 * iterator
 */

typedef void*			t_iterator;

/*
 * set manager
 */

typedef struct
{
  o_id				id;

  t_id				contid;
  o_set*			cont;
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

#define set_check(_set_)						\
  {									\
    if ((_set_) == NULL)						\
      return (-1);							\
  }

/*
 * traps
 */

#define set_trap(_func_, _setid_, _args_...)				\
  (									\
    {									\
      int		_r_ = -1;					\
      o_set*		_set_;						\
									\
      if (set_object((_setid_), &_set_) == 0)				\
        {								\
          switch (_set_->type)						\
            {								\
              case SET_TYPE_LL:						\
                _r_ = _func_##_ll((_setid_), _args_);			\
                break;							\
            }								\
        }								\
      _r_;								\
    }									\
  )

#define set_rsv(_type_, _args_...)					\
  set_rsv_##_type_(_args_)

#define set_head(_setid_, _args_...)					\
  set_trap(set_head, _setid_, _args_)

#define set_tail(_setid_, _args_...)					\
  set_trap(set_tail, _setid_, _args_)

#define set_prev(_setid_, _args_...)					\
  set_trap(set_prev, _setid_, _args_)

#define set_next(_setid_, _args_...)					\
  set_trap(set_next, _setid_, _args_)

#define set_get(_setid_, _args_...)					\
  set_trap(set_get, _setid_, _args_)

#define set_add(_setid_, _args_...)					\
  set_trap(set_add, _setid_, _args_)

#define set_remove(_setid_, _args_...)					\
  set_trap(set_remove, _setid_, _args_)

/*
 * foreach
 */

#define set_foreach(_opt_, _setid_, _iterator_)				\
  for (*(_iterator_) = NULL;						\
        ((*(_iterator_) == NULL) ?					\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_head((_setid_), (_iterator_)) == 0 :			\
            set_tail((_setid_), (_iterator_)) == 0) :			\
          ((_opt_) == SET_OPT_FORWARD ?					\
            set_next((_setid_), *(_iterator_), (_iterator_)) == 0 :	\
            set_prev((_setid_), *(_iterator_), (_iterator_)) == 0));	\
       )

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set.c
 */

/*
 * ../../kaneton/set/set.c
 */

int			set_dump(t_setid			setid);

int			set_object_add(o_set*			o);

int			set_object_remove(t_setid		setid);

int			set_object(t_setid			setid,
				   o_set**			o);

int			set_id_rsv(t_setid*			setid);

int			set_id_rel(t_setid			setid);

int			set_find(t_setid			setid,
				 t_id				id,
				 t_iterator*			iterator);

int			set_init(void);


/*
 * eop
 */

#endif
