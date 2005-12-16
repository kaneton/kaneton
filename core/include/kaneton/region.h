/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * region.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Sun Oct 30 23:07:28 2005   mycure
 */

#ifndef KANETON_REGION_H
#define KANETON_REGION_H	1

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
 * flags
 */

#define REGION_OPT_NONE		(1 << 0)
#define REGION_OPT_FORCE	(1 << 1)

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
  t_vsize			size;
}				o_region;

/*
 * region manager
 */

typedef struct
{
  o_id				id;

  t_fit				fit;

  t_vaddr			start;
  t_vsize			size;

  t_staid			stats;
}				m_region;

/*
 * ---------- macros ----------------------------------------------------------
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
    STATS_END((_region_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/region/region.c
 */

/*
 * ../../kaneton/region/region.c
 */

t_error			region_show(t_asid			asid,
				    t_regid			regid);

t_error			region_first_fit(o_as*			as,
					 t_vsize		size,
					 t_vaddr*		address);

t_error			region_reserve(t_asid			asid,
				       t_segid			segid,
				       t_opts			opts,
				       t_vaddr			address,
				       t_regid*			regid);

t_error			region_release(t_asid			asid,
				       t_regid			regid);

t_error			region_flush(t_asid			asid);

t_error			region_get(t_asid			asid,
				   t_regid			regid,
				   o_region**			o);

t_error			region_init(t_fit			fit,
				    t_vaddr			address,
				    t_vsize			size);

t_error			region_clean(void);


/*
 * eop
 */

#endif
