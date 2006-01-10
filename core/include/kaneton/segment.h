/*
 * copyright quintard julien
 *
 * kaneton
 *
 * segment.h
 *
 * path          /home/mycure/kaneton/core
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 02:19:44 2005   mycure
** Last update Tue Jan 10 01:19:08 2006 matthieu bucchianeri
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

  t_asid			asid;

  t_type			type;

  t_paddr			address;
  t_psize			size;

  t_perms			perms;

  machdep_data(o_segment);
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

  machdep_data(m_segment);
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
 *      ../../kaneton/segment/segment-fit.c
 */

/*
 * ../../kaneton/segment/segment.c
 */

t_error			segment_show(t_segid			segid);

t_error			segment_dump(void);

t_error			segment_clone(t_asid			asid,
				      t_segid			old,
				      t_segid*			new);

t_error			segment_reserve(t_asid			asid,
					t_psize			size,
					t_perms			perms,
					t_segid*		segid);

t_error			segment_release(t_segid			segid);

t_error			segment_catch(t_asid			asid,
				      t_segid			segid);

t_error			segment_perms(t_segid			segid,
				      t_perms			perms);

t_error			segment_type(t_segid			segid,
				     t_type			type);

t_error			segment_flush(t_asid			asid);

t_error			segment_get(t_segid			segid,
				    o_segment**			o);

t_error			segment_init(void);

t_error			segment_clean(void);


/*
 * ../../kaneton/segment/segment-fit.c
 */

t_error			segment_fit(o_as*		as,
				    t_psize		size,
				    t_paddr*		address);

t_error			segment_first_fit(o_as*			as,
					  t_psize		size,
					  t_paddr*		address);


/*
 * eop
 */

#endif
