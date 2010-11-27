/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.TETON/kaneton/core/include/as.h
 *
 * created       julien quintard   [wed jun  6 12:25:01 2007]
 * updated       julien quintard   [sat nov 27 20:13:14 2010]
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
 * the as architecture dependent interface
 */

typedef struct
{
  t_error			(*as_show)(i_as);
  t_error			(*as_give)(i_as,
					   i_task);
  t_error			(*as_vaddr)(i_as,
					    t_paddr,
					    t_vaddr*);
  t_error			(*as_paddr)(i_as,
					    t_vaddr,
					    t_paddr*);
  t_error			(*as_clone)(i_as,
					    i_task,
					    i_as*);
  t_error			(*as_reserve)(i_task,
					      i_as*);
  t_error			(*as_release)(i_as);
  t_error			(*as_initialize)(void);
  t_error			(*as_clean)(void);
}				d_as;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/as/as.c
 */

/*
 * ../../core/as/as.c
 */

t_error			as_show(i_as				id);

t_error			as_dump(void);

t_error			as_give(i_as				id,
				i_task				task);

t_error			as_vaddr(i_as				id,
				 t_paddr			physical,
				 t_vaddr*			virtual);

t_error			as_paddr(i_as				id,
				 t_vaddr			virtual,
				 t_paddr*			physical);

t_error			as_read(i_as				id,
				t_vaddr				source,
				t_vsize				size,
				void*				destination);

t_error			as_write(i_as				id,
				 const void*			source,
				 t_vsize			size,
				 t_vaddr			destination);

t_error			as_copy(i_as			source_id,
				t_vaddr			source_address,
				i_as			destination_id,
				t_vaddr			destination_address,
				t_vsize			size);

t_error			as_clone(i_as				id,
				 i_task				task,
				 i_as*				as);

t_error			as_reserve(i_task			task,
				   i_as*			id);

t_error			as_release(i_as			id);

t_error			as_exist(i_as				id);

t_error			as_get(i_as				id,
			       o_as**				o);

t_error			as_initialize(void);

t_error			as_clean(void);


/*
 * eop
 */

#endif
