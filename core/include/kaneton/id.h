/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * id.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Fri Aug 26 01:09:25 2005   mycure
 */

#ifndef KANETON_ID_H
#define KANETON_ID_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * identifier type
 */

typedef t_uint64		t_id;

/*
 * identifier object
 */

typedef struct
{
  t_id				id;
}				o_id;

/*
 * ---------- defines ---------------------------------------------------------
 */

#define ID_UNUSED		((t_id)-1)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/id/id.c
 */

/*
 * ../../kaneton/id/id.c
 */

void			id_dump(o_id*				o);

int			id_rsv(o_id*				o,
			       t_id*				id);

int			id_rel(o_id*				o,
			       t_id				id);

int			id_build(o_id*				o);

int			id_destroy(o_id*			o);

int			id_init(void);

int			id_clean(void);


/*
 * eop
 */

#endif
