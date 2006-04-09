/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/segment.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [sun apr  2 23:18:56 2006]
 */

#ifndef CORE_SEGMENT_H
#define CORE_SEGMENT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * set parameters
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

  t_lookup			lookup;

  t_setid			segments;

  machdep_data(m_segment);
}				m_segment;

/*
 * the segment architecture-dependent interface
 */

typedef struct
{
  t_error			(*segment_show)(t_segid);
  t_error			(*segment_clone)(t_asid,
						 t_segid,
						 t_segid*);
  t_error			(*segment_inject)(t_asid,
						  o_segment*);
  t_error			(*segment_give)(t_asid,
						t_segid);
  t_error			(*segment_resize)(t_segid,
						  t_psize,
						  t_segid*);
  t_error			(*segment_split)(t_segid,
						 t_psize,
						 t_segid*,
						 t_segid*);
  t_error			(*segment_coalesce)(t_segid,
						    t_segid,
						    t_segid*);
  t_error			(*segment_read)(t_segid,
						t_paddr,
						void*,
						t_psize);
  t_error			(*segment_write)(t_segid,
						 t_paddr,
						 const void*,
						 t_psize);
  t_error			(*segment_copy)(t_segid,
						t_paddr,
						t_segid,
						t_paddr,
						t_psize);
  t_error			(*segment_reserve)(t_asid,
						   t_psize,
						   t_perms,
						   t_segid*);
  t_error			(*segment_release)(t_segid);
  t_error			(*segment_catch)(t_asid,
						 t_segid);
  t_error			(*segment_perms)(t_segid,
						 t_perms);
  t_error			(*segment_type)(t_segid,
						t_type);
  t_error			(*segment_flush)(t_asid);
  t_error			(*segment_init)();
  t_error			(*segment_clean)(void);
}				d_segment;

/*
 * ---------- macro functions -------------------------------------------------
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
 * ---------- common prototypes -----------------------------------------------
 */

t_error			segment_space(o_as*		as,
				      t_psize		size,
				      t_paddr*		address);


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/segment/segment.c
 */

/*
 * ../../core/segment/segment.c
 */

t_error			segment_show(t_segid			segid);

t_error			segment_dump(void);

t_error			segment_clone(t_asid			asid,
				      t_segid			old,
				      t_segid*			new);

t_error			segment_inject(t_asid		asid,
				       o_segment*	o,
				       t_segid*		segid);

t_error			segment_give(t_segid		segid,
				     t_asid		asid);

t_error			segment_resize(t_segid		old,
				       t_psize		size,
				       t_segid*		new);

t_error			segment_split(t_segid		segid,
				      t_psize		size,
				      t_segid*		left,
				      t_segid*		right);

t_error			segment_coalesce(t_segid	old1,
					 t_segid	old2,
					 t_segid*	new);

t_error			segment_read(t_segid		segid,
				     t_paddr		offs,
				     void*		buff,
				     t_psize		sz);

t_error			segment_write(t_segid		segid,
				      t_paddr		offs,
				      const void*	buff,
				      t_psize		sz);

t_error			segment_copy(t_segid		dst,
				     t_paddr		offsd,
				     t_segid		src,
				     t_paddr		offss,
				     t_psize		sz);

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
 * eop
 */

#endif
