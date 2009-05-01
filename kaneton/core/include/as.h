/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/as.h
 *
 * created       julien quintard   [wed jun  6 12:25:01 2007]
 * updated       julien quintard   [wed apr 15 07:21:35 2009]
 */

#ifndef CORE_AS_H
#define CORE_AS_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

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
  i_as				asid;

  i_task			tskid;

  i_set				segments;
  i_set				regions;

  machine_data(o_as);
}				o_as;

/*
 * as manager
 */

typedef struct
{
  o_id				id;

  i_set				ass;

  machine_data(m_as);
}				m_as;

/*
 * the as architecture dependent interface
 */

typedef struct
{
  t_error			(*as_show)(i_as);
  t_error			(*as_give)(i_task,
					   i_as);
  t_error			(*as_vaddr)(i_as,
					    t_paddr,
					    t_vaddr*);
  t_error			(*as_paddr)(i_as,
					    t_vaddr,
					    t_paddr*);
  t_error			(*as_clone)(i_task,
					    i_as,
					    i_as*);
  t_error			(*as_reserve)(i_task,
					      i_as*);
  t_error			(*as_release)(i_as);
  t_error			(*as_initialize)(void);
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
  }

/*
 * leave
 */

#define AS_LEAVE(_as_, _error_)						\
  {									\
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

t_error			as_show(i_as				asid);

t_error			as_dump(void);

t_error			as_give(i_task			tskid,
				i_as			asid);

t_error			as_vaddr(i_as			asid,
				 t_paddr		physical,
				 t_vaddr*		virtual);

t_error			as_paddr(i_as		asid,
				 t_vaddr	virtual,
				 t_paddr*	physical);

t_error			as_read(i_as				asid,
				t_vaddr				src,
				t_vsize				size,
				void*				dst);

t_error			as_write(i_as				asid,
				 const void*			src,
				 t_vsize			size,
				 t_vaddr			dst);

t_error			as_copy(i_as				src_as,
				t_vaddr				src,
				i_as				dst_as,
				t_vaddr				dst,
				t_vsize				size);

t_error			as_clone(i_task				tskid,
				 i_as				old,
				 i_as*				new);

t_error			as_reserve(i_task			tskid,
				   i_as*			asid);

t_error			as_release(i_as			asid);

t_error			as_get(i_as				asid,
			       o_as**				o);

t_error			as_initialize(void);

t_error			as_clean(void);


/*
 * eop
 */

#endif
