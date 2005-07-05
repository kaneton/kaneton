/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Mon Jul  4 21:48:21 2005   mycure
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
 * a set object
 *
 * the setid field represent the set identifier used to retrieve the set
 * data structure. the union is composed of subparts dependending of the
 * set type.
 */

typedef struct
{
  t_setid			setid;

  t_type			type;

  union
  {
    t_set_ll			ll;
  }				u;
}				o_set;

/*
 * the set manager
 *
 * this type contains all the information needed by the set manager.
 */

typedef struct
{
  t_segid			id;

  o_set*			sets;
}				m_set;

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_set		set;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * traps
 */

#define set_trap(_func_, _setid_, _args_...)				\
  (									\
    {									\
      int		_r_ = -1;					\
      o_set*		_set_;						\
									\
      if (set_get((_setid_), &_set_) == 0)				\
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

#define set_add(_setid_, _args_...)					\
  set_trap(set_add, _setid_, _args_)

#define set_remove(_setid_, _args_...)					\
  set_trap(set_remove, _setid_, _args_)

/*
 * foreach
 */

#define SET_FOREACH(_type_, _mode_, _setid_, _iterator_)		\
  for ((_iterator_) = NULL;						\
        (((_iterator_) == NULL) ?					\
          ((_mode_) == FOREACH_FORWARD ?				\
            set_head((_type_), (_setid_), (_iterator_)) :		\
            set_tail((_type_), (_setid_), (_iterator_))) :		\
          ((_mode_) == FOREACH_FORWARD ?				\
            set_next((_type_), (_setid_),				\
                     *(_iterator_), (_iterator_)) :			\
            set_prev((_type_), (_setid_), *(_iterator_),		\
                     (_iterator_)))) == 0;				\
       )

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set.c
 */

/*
 * ../../kaneton/set/set.c
 */

int			set_object(o_set*			o);

int			set_id(t_setid*				setid);

int			set_get(t_setid				setid,
				o_set**				o);

int			set_init(void);


/*
 * eop
 */

#endif
