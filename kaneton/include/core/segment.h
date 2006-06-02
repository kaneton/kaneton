/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/segment.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [fri jun  2 14:12:12 2006]
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
  i_segment			segid;

  i_as				asid;

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

  i_stats			stats;

  t_paddr			start;
  t_psize			size;

  t_lookup			lookup;

  i_set				segments;

  machdep_data(m_segment);
}				m_segment;

/*
 * the segment architecture-dependent interface
 */

typedef struct
{
  t_error			(*segment_show)(i_segment);
  t_error			(*segment_clone)(i_as,
						 i_segment,
						 i_segment*);
  t_error			(*segment_inject)(i_as,
						  o_segment*);
  t_error			(*segment_give)(i_as,
						i_segment);
  t_error			(*segment_resize)(i_segment,
						  t_psize,
						  i_segment*);
  t_error			(*segment_split)(i_segment,
						 t_psize,
						 i_segment*,
						 i_segment*);
  t_error			(*segment_coalesce)(i_segment,
						    i_segment,
						    i_segment*);
  t_error			(*segment_read)(i_segment,
						t_paddr,
						void*,
						t_psize);
  t_error			(*segment_write)(i_segment,
						 t_paddr,
						 const void*,
						 t_psize);
  t_error			(*segment_copy)(i_segment,
						t_paddr,
						i_segment,
						t_paddr,
						t_psize);
  t_error			(*segment_reserve)(i_as,
						   t_psize,
						   t_perms,
						   i_segment*);
  t_error			(*segment_release)(i_segment);
  t_error			(*segment_catch)(i_as,
						 i_segment);
  t_error			(*segment_perms)(i_segment,
						 t_perms);
  t_error			(*segment_type)(i_segment,
						t_type);
  t_error			(*segment_flush)(i_as);
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

t_error			segment_show(i_segment			segid);

t_error			segment_dump(void);

t_error			segment_clone(i_as			asid,
				      i_segment			old,
				      i_segment*		new);

t_error			segment_inject(i_as		asid,
				       o_segment*	o,
				       i_segment*	segid);

t_error			segment_give(i_as		asid,
				     i_segment		segid);

t_error			segment_resize(i_segment	old,
				       t_psize		size,
				       i_segment*	new);

t_error			segment_split(i_segment		segid,
				      t_psize		size,
				      i_segment*	left,
				      i_segment*	right);

t_error			segment_coalesce(i_segment	left,
					 i_segment	right,
					 i_segment*	segid);

t_error			segment_read(i_segment		segid,
				     t_paddr		offs,
				     void*		buff,
				     t_psize		sz);

t_error			segment_write(i_segment		segid,
				      t_paddr		offs,
				      const void*	buff,
				      t_psize		sz);

t_error			segment_copy(i_segment		dst,
				     t_paddr		offsd,
				     i_segment		src,
				     t_paddr		offss,
				     t_psize		sz);

t_error			segment_reserve(i_as			asid,
					t_psize			size,
					t_perms			perms,
					i_segment*		segid);

t_error			segment_release(i_segment		segid);

t_error			segment_catch(i_as			asid,
				      i_segment			segid);

t_error			segment_perms(i_segment			segid,
				      t_perms			perms);

t_error			segment_type(i_segment			segid,
				     t_type			type);

t_error			segment_flush(i_as			asid);

t_error			segment_get(i_segment			segid,
				    o_segment**			o);

t_error			segment_init(void);

t_error			segment_clean(void);


/*
 * eop
 */

#endif
