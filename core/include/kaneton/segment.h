/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Tue Oct 25 13:41:48 2005   mycure
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
 * container parameters
 */

#define SEGMENT_BPT_NODESZ	4096

/*
 * segment types
 */

#define SEGMENT_TYPE_MEMORY	(1 << 0)
#define SEGMENT_TYPE_CATCH	(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * segment object
 */

typedef struct
{
  t_segid			segid;

  t_type			type;

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

t_error			segment_first_fit(o_as*			as,
					  t_psize		size,
					  t_perms		perms,
					  t_segid*		segid);

t_error			segment_rsv(t_asid			asid,
				    t_psize			size,
				    t_perms			perms,
				    t_segid*			segid);

t_error			segment_rel(t_asid			asid,
				    t_segid			segid);

t_error			segment_catch(t_asid			asid,
				      t_segid			segid);

t_error			segment_perms(t_asid			asid,
				      t_segid			segid,
				      t_perms			perms);

t_error			segment_type(t_asid			asid,
				     t_segid			segid,
				     t_type			type);

t_error			segment_flush(t_asid			asid);

t_error			segment_get(t_segid			segid,
				    o_segment**			o);

t_error			segment_init(t_fit			fit);

t_error			segment_clean(void);

t_error			segment_test(void);


/*
 * eop
 */

#endif
