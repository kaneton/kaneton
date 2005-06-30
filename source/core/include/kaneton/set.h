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
 * last update   Mon Jun 20 23:38:19 2005   mycure
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
 * identifiers
 */

#define SETID_SEGMENTS		((t_setid)0x0)

#define SETID_KSEGMENTS		((t_setid)0x1)
#define SETID_KREGIONS		((t_setid)0x2)

#define SET_ID			((t_setid)0x3)

/*
 * foreach
 */

#define SET_OPT_FORWARD		0x1
#define SET_OPT_BACKWARD	0x2

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the types of function used for allocation.
 */

typedef int			(*t_set_alloc_fn)(t_size	size,
						  void**	address);

typedef int			(*t_set_free_fn)(void*		address,
						 t_size		size);

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

  t_set_alloc_fn		alloc;
  t_set_free_fn			free;

  union
  {
    t_set_ll			ll;
  }				u;
}				o_set;

/*
 * the set manager
 *
 * this type contains all the information needed by the set manager.
 *
 * note that there is three special fields segments, ksegments and kregions.
 * these fields are present because they cannot be managed like other
 * sets because the malloc() function is not provided yet.
 */

typedef struct
{
  t_segid			id;

  o_set				segments;

  o_set				ksegments;
  o_set				kregions;

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

#define set_make(_type_, _args_...)					\
  set_make_##_type_(_args_)

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

 * notice that the set manager is based on the malloc() function. indeed;

 * moreover some data structure type required the malloc() function, the;

 * simplest example being the array data structure which require the realloc();

 * the malloc() function is not provided yet. to bypass this restriction these;

int			set_id(t_setid*				setid);

int			set_get(t_setid				setid,
				o_set**				o);

int			set_init(void);


/*
 * eop
 */

#endif
