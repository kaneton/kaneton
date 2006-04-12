/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/as.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [wed apr 12 11:53:19 2006]
 */

#ifndef CORE_AS_H
#define CORE_AS_H		1

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
 * init sizes for the array data structure set
 */

#define AS_SEGMENTS_INITSZ	0x4
#define AS_REGIONS_INITSZ	0x4

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * as object
 */

typedef struct
{
  t_asid			asid;

  i_task			tskid;

  t_setid			segments;
  t_setid			regions;

  machdep_data(o_as);
}				o_as;

/*
 * as manager
 */

typedef struct
{
  o_id				id;

  i_stats			stats;

  t_setid			ass;

  machdep_data(m_as);
}				m_as;

/*
 * the as architecture dependent interface
 */

typedef struct
{
  t_error			(*as_show)(t_asid);
  t_error			(*as_give)(t_asid,
					   i_task);
  t_error			(*as_vaddr)(t_asid,
					    t_paddr,
					    t_vaddr*);
  t_error			(*as_paddr)(t_asid,
					    t_vaddr,
					    t_paddr*);
  t_error			(*as_clone)(i_task,
					    t_asid,
					    t_asid*);
  t_error			(*as_reserve)(i_task,
					      t_asid*);
  t_error			(*as_release)(t_asid);
  t_error			(*as_init)(void);
  t_error			(*as_clean)(void);
}				d_as;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define AS_CHECK(_as_)							\
  {									\
    if ((_as_) == NULL)							\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define AS_ENTER(_as_)							\
  {									\
    AS_CHECK((_as_));							\
									\
    STATS_BEGIN((_as_)->stats);						\
  }

/*
 * leave
 */

#define AS_LEAVE(_as_, _error_)						\
  {									\
    STATS_END((_as_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/as/as.c
 */

/*
 * ../../core/as/as.c
 */

t_error			as_show(t_asid				asid);

t_error			as_dump(void);

t_error			as_give(t_asid			asid,
				i_task			tskid);

t_error			as_vaddr(t_asid			asid,
				 t_paddr		physical,
				 t_vaddr*		virtual);

t_error			as_paddr(t_asid		asid,
				 t_vaddr	virtual,
				 t_paddr*	physical);

t_error			as_clone(i_task				tskid,
				 t_asid				old,
				 t_asid*			new);

t_error			as_reserve(i_task			tskid,
				   t_asid*			asid);

t_error			as_release(t_asid			asid);

t_error			as_get(t_asid				asid,
			       o_as**				o);

t_error			as_init(void);

t_error			as_clean(void);


/*
 * eop
 */

#endif
