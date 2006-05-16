/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/region.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [thu may 11 13:41:15 2006]
 */

#ifndef CORE_REGION_H
#define CORE_REGION_H		1

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
 * flags
 */

#define REGION_OPT_NONE		(1 << 0)
#define REGION_OPT_FORCE	(1 << 1)
#define REGION_OPT_MAPALL	(1 << 2)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * region object
 *
 * a region object is identified by a regid.
 *
 * the identifier is in fact identical to the virtual address of the region.
 */

typedef struct
{
  t_regid			regid;

  t_segid			segid;

  t_vaddr			address;
  t_paddr			offset;
  t_vsize			size;
}				o_region;

/*
 * region manager
 */

typedef struct
{
  o_id				id;

  t_lookup			lookup;

  t_vaddr			start;
  t_vsize			size;

  i_stats			stats;
}				m_region;

/*
 * the region architecture-dependent interface
 */

typedef struct
{
  t_error			(*region_show)(t_asid,
					       t_regid);
  t_error			(*region_inject)(t_asid,
						 o_region*);
  t_error			(*region_reserve)(t_asid,
						  t_segid,
						  t_paddr,
						  t_opts,
						  t_vaddr,
						  t_vsize,
						  t_regid*);
  t_error			(*region_release)(t_asid,
						  t_regid);
  t_error			(*region_flush)(t_asid);
  t_error			(*region_init)(t_vaddr,
					       t_vsize);
  t_error			(*region_clean)(void);
}				d_region;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define REGION_CHECK(_region_)						\
  {									\
    if ((_region_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define REGION_ENTER(_region_)						\
  {									\
    REGION_CHECK((_region_));						\
									\
    STATS_BEGIN((_region_)->stats);					\
  }

/*
 * leave
 */

#define REGION_LEAVE(_region_, _error_)					\
  {									\
    if (_error_ != ERROR_NONE) \
      cons_msg('!', "error at %s (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__); \
    STATS_END((_region_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- common prototypes -----------------------------------------------
 */

t_error			region_space(o_as*		as,
				     t_vsize		size,
				     t_vaddr*		address);

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/region/region.c
 */

/*
 * ../../core/region/region.c
 */

t_error			region_show(t_asid			asid,
				    t_regid			regid);

t_error			region_dump(t_asid		asid);

t_error			region_inject(t_asid		asid,
				      o_region*		o);

t_error			region_reserve(t_asid			asid,
				       t_segid			segid,
				       t_paddr			offset,
				       t_opts			opts,
				       t_vaddr			address,
				       t_vsize			size,
				       t_regid*			regid);

t_error			region_release(t_asid			asid,
				       t_regid			regid);

t_error			region_flush(t_asid			asid);

t_error			region_get(t_asid			asid,
				   t_regid			regid,
				   o_region**			o);

t_error			region_init(t_vaddr			start,
				    t_vsize			size);

t_error			region_clean(void);


/*
 * eop
 */

#endif
