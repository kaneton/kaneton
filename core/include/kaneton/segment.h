/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Fri Aug 26 01:21:18 2005   mycure
 */

#ifndef KANETON_SEGMENT_H
#define KANETON_SEGMENT_H	1

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
 * fit
 */

#define SEGMENT_FIT_FIRST	0x1
#define SEGMENT_FIT_WORST	0x2

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * segment object
 */

typedef struct
{
  t_segid			segid;

  t_paddr			address;
  t_psize			size;

  t_perms			perms;
}				o_segment;

/*
 * segment manager
 */

typedef struct
{
  o_id				id;

  t_paddr			start;
  t_psize			size;

  t_fit				fit;

  t_setid			container;
}				m_segment;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * check
 */

#define segment_check(_segment_)					\
  {									\
    if ((_segment_) == NULL)						\
      return (-1);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/segment/segment.c
 */

/*
 * ../../kaneton/segment/segment.c
 */

int			segment_dump(void);

int			segment_rsv(t_asid			asid,
				    t_psize			size,
				    t_perms			perms,
				    t_segid*			segid);

int			segment_rel(t_asid			asid,
				    t_segid			segid);

int			segment_perms(t_asid			asid,
				      t_segid			segid,
				      t_perms			perms);

int			segment_flush(t_asid			asid);

int			segment_get(t_segid			segid,
				    o_segment**			o);

int			segment_init(t_fit			fit);

int			segment_clean(void);


/*
 * eop
 */

#endif
