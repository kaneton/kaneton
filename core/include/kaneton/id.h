/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * id.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Thu Sep 15 17:38:09 2005   mycure
 */

#ifndef KANETON_ID_H
#define KANETON_ID_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * identifier object
 */

typedef struct
{
  t_id				id;
}				o_id;

/*
 * the identifier manager
 */

typedef struct
{
  /*
   * nothing
   */

  int				XXX; /* for alloc bug */

}				m_id;

/*
 * ---------- defines ---------------------------------------------------------
 */

#define ID_UNUSED		((t_id)-1)

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * enter
 */

#define ID_ENTER(_id_)

/*
 * leave
 */

#define ID_LEAVE(_id_, _error_)						\
  {									\
    return ((_error_));							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/id/id.c
 */

/*
 * ../../kaneton/id/id.c
 */

t_error			id_dump(o_id*				o);

t_error			id_rsv(o_id*				o,
			       t_id*				id);

t_error			id_rel(o_id*				o,
			       t_id				id);

t_error			id_build(o_id*				o);

t_error			id_destroy(o_id*			o);

t_error			id_init(void);

t_error			id_clean(void);


/*
 * eop
 */

#endif
