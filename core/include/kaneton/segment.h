/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Tue Aug 30 13:14:25 2005   mycure
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

  t_staid			stats;

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

#define SEGMENT_CHECK(_segment_)					\
  {									\
    if ((_segment_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define SEGMENT_ENTER(_segment_)					\
  {									\
    SEGMENT_CHECK((_segment_));						\
									\
    STATS_BEGIN((_segment_)->stats);					\
  }

/*
 * leave
 */

#define SEGMENT_LEAVE(_segment_, _error_)				\
  {									\
    STATS_END((_segment_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/segment/segment.c
 */

/*
 * ../../kaneton/segment/segment.c
 */

t_error			segment_dump(void);

t_error			segment_rsv(t_asid			asid,
				    t_psize			size,
				    t_perms			perms,
				    t_segid*			segid);

t_error			segment_rel(t_asid			asid,
				    t_segid			segid);

t_error			segment_perms(t_asid			asid,
				      t_segid			segid,
				      t_perms			perms);

t_error			segment_flush(t_asid			asid);

t_error			segment_get(t_segid			segid,
				    o_segment**			o);

t_error			segment_init(t_fit			fit);

t_error			segment_clean(void);


/*
 * eop
 */

#endif
