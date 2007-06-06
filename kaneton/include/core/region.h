/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/region.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [wed aug 16 19:45:15 2006]
 */

#ifndef CORE_REGION_H
#define CORE_REGION_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct so_region	o_region;
typedef struct sm_region	m_region;
typedef struct sd_region	d_region;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * flags
 */

#define REGION_OPT_NONE		0
#define REGION_OPT_FORCE	(1 << 0)
#define REGION_OPT_USER		(0 << 1)
#define REGION_OPT_PRIVILEGED	(1 << 1)
#define REGION_OPT_LOCAL	(0 << 2)
#define REGION_OPT_GLOBAL	(1 << 2)

// XXX
#endif

/*
 * ---------- dependencies ----------------------------------------------------
 */

// XXX
#ifndef CORE_REGION_H2
#define CORE_REGION_H2	1

// XXX
typedef int _debug_region_1_;

#include <arch/machdep/machdep.h>

// XXX
typedef int _debug_region_2_;

#include <core/id.h>

// XXX
typedef int _debug_region_3_;

#include <core/types.h>

// XXX
typedef int _debug_region_4_;

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

struct				so_region
{
  i_region			regid;

  i_segment			segid;

  t_vaddr			address;
  t_paddr			offset;
  t_vsize			size;
  t_opts			opts;

  machdep_data(o_region);
};

/*
 * region manager
 */

struct				sm_region
{
  o_id				id;

  t_lookup			lookup;

  t_vaddr			start;
  t_vsize			size;

  i_stats			stats;

  machdep_data(m_region);
};

/*
 * the region architecture-dependent interface
 */

struct				sd_region
{
  t_error			(*region_show)(i_as,
					       i_region);
  t_error			(*region_inject)(i_as,
						 o_region*);
  t_error			(*region_split)(i_as,
						i_region,
						t_vsize,
						i_region*,
						i_region*);
  t_error			(*region_resize)(i_as,
						 i_region,
						 t_vsize,
						 i_region*);
  t_error			(*region_coalesce)(i_as,
						   i_region,
						   i_region,
						   i_region*);
  t_error			(*region_reserve)(i_as,
						  i_segment,
						  t_paddr,
						  t_opts,
						  t_vaddr,
						  t_vsize,
						  i_region*);
  t_error			(*region_release)(i_as,
						  i_region);
  t_error			(*region_flush)(i_as);
  t_error			(*region_init)(t_vaddr,
					       t_vsize);
  t_error			(*region_clean)(void);
};

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

t_error			region_show(i_as			asid,
				    i_region			regid);

t_error			region_dump(i_as		asid);

t_error			region_inject(i_as		asid,
				      o_region*		o);

t_error			region_split(i_as			asid,
				     i_region			regid,
				     t_vsize			size,
				     i_region*			left,
				     i_region*			right);

t_error			region_resize(i_as			as,
				      i_region			old,
				      t_vsize			size,
				      i_region*			new);

t_error			region_coalesce(i_as		asid,
					i_region	left,
					i_region	right,
					i_region*	regid);

t_error			region_reserve(i_as			asid,
				       i_segment		segid,
				       t_paddr			offset,
				       t_opts			opts,
				       t_vaddr			address,
				       t_vsize			size,
				       i_region*		regid);

t_error			region_release(i_as			asid,
				       i_region			regid);

t_error			region_flush(i_as			asid);

t_error			region_get(i_as				asid,
				   i_region			regid,
				   o_region**			o);

t_error			region_init(t_vaddr			start,
				    t_vsize			size);

t_error			region_clean(void);


/*
 * eop
 */

#endif
