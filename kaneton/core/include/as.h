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
 * updated       julien quintard   [fri dec 10 21:16:27 2010]
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
  i_as				id;

  i_task			task;

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
 * the address space dispatcher.
 */

typedef struct
{
  t_status		(*as_show)(i_as,
					   mt_margin);
  t_status		(*as_dump)(void);
  t_status		(*as_virtual)(i_as,
					      t_paddr,
					      t_vaddr*);
  t_status		(*as_physical)(i_as,
					       t_vaddr,
					       t_paddr*);
  t_status		(*as_read)(i_as,
					   t_vaddr,
					   t_vsize,
					   void*);
  t_status		(*as_write)(i_as,
					    const void*,
					    t_vaddr,
					    t_vsize);
  t_status		(*as_copy)(i_as,
					   t_vaddr,
					   i_as,
					   t_vaddr,
					   t_vsize);
  t_status		(*as_reserve)(i_task,
					      i_as*);
  t_status		(*as_release)(i_as);
  t_status		(*as_initialize)(void);
  t_status		(*as_clean)(void);
}				d_as;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/as/as.c
 */

/*
 * ../../core/as/as.c
 */

t_status		as_show(i_as		id,
				        mt_margin	margin);

t_status		as_dump(void);

t_status		as_virtual(i_as		id,
                                           t_paddr	physical,
                                           t_vaddr*	virtual);

t_status		as_physical(i_as	id,
				            t_vaddr	virtual,
				            t_paddr*	physical);

t_status		as_read(i_as		id,
				        t_vaddr		source,
				        t_vsize		size,
				        void*		destination);

t_status		as_write(i_as		id,
				         const void*	source,
				         t_vsize	size,
				         t_vaddr	destination);

t_status		as_copy(i_as		source_id,
				        t_vaddr		source_address,
				        i_as		destination_id,
				        t_vaddr		destination_address,
				        t_vsize		size);

t_status		as_reserve(i_task	task,
				           i_as*	id);

t_status		as_release(i_as		id);

t_bool				as_exist(i_as		id);

t_status		as_get(i_as		id,
				       o_as**		object);

t_status		as_initialize(void);

t_status		as_clean(void);


/*
 * eop
 */

#endif
