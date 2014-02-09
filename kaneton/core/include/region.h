/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/region.h
 *
 * created       julien quintard   [wed jun  6 13:40:54 2007]
 * updated       julien quintard   [fri apr  8 09:04:13 2011]
 */

#ifndef CORE_REGION_H
#define CORE_REGION_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- algorithms ------------------------------------------------------
 */

/*
 * the several algorithms supported by the region manager.
 *
 * note that such algorithms can be activated through the REGION_ALGORITHM
 * macro.
 */

#define REGION_ALGORITHM_FIT_FIRST	(1 << 0)
#define REGION_ALGORITHM_FIT_BEST	(1 << 1)

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the flags which can be used for reserving regions.
 *
 * the _force_ option enables the caller of region_reserve() to specify
 * the address of reservation.
 */

#define REGION_OPTION_NONE		0
#define REGION_OPTION_FORCE		(1 << 0)

#define REGION_OPTION_INVALID		(~(REGION_OPTION_FORCE))

/*
 * this macro defines the size of the vault i.e the set of regions
 * which must be temporarily prevented from being reserved.
 */

#define REGION_VAULT_SIZE		16

/*
 * these macros indicate the state of a vault entry.
 */

#define REGION_VAULT_STATE_AVAILABLE	0
#define REGION_VAULT_STATE_USED		1

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this macro-function computes a region identifier based on the address.
 */

#define REGION_IDENTIFIER(_object_)					\
  (i_region)((_object_)->address / ___kaneton$pagesz)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure represents a memory area which needs to be prevented
 * from being reserved.
 */

typedef struct
{
  t_state			state;
  i_as				as;
  t_vaddr			address;
  t_vsize			size;
}				s_region_zone;

/*
 * the region object structure.
 *
 * a region represents an area of virtual memory and is identified by
 * a region identifier.
 *
 * every region maps a _segment_ at a given _offset_ and for a certain
 * _size_. the resulting virtual address _address_ can then be used to
 * access the segment through the virtual memory mechanism.
 *
 * the _options_ are used to parameterize both the region's behaviour
 * as well as the way the region is reserved. for more information, please
 * refer to the region_reserve() function.
 */

typedef struct
{
  i_region			id;

  i_segment			segment;
  t_paddr			offset;

  t_vaddr			address;
  t_vsize			size;
  t_options			options;

  machine_data(o_region);
}				o_region;

/*
 * the region manager structure.
 *
 * the _base_ attribute represents the lower bound virtual address while
 * the _size_ represents the number of bytes the virtual memory is composed
 * of.
 *
 * the _vault_ set identifier references a set of regions which need
 * to be prevented from being reserved until the current operation finishes.
 */

typedef struct
{
  t_vaddr			base;
  t_vsize			size;

  s_region_zone			vault[REGION_VAULT_SIZE];

  machine_data(m_region);
}				m_region;

/*
 * the region dispatcher.
 */

typedef struct
{
  t_status		(*region_show)(i_as,
					       i_region,
					       mt_margin);
  t_status		(*region_dump)(void);
  t_status		(*region_protect)(i_as,
						  t_vaddr,
						  t_vsize);
  t_status		(*region_unprotect)(i_as,
						    t_vaddr,
						    t_vsize);
  t_status		(*region_inject)(i_as,
						 o_region*,
						 i_region*);
  t_status		(*region_split)(i_as,
						i_region,
						t_vsize,
						i_region*,
						i_region*);
  t_status		(*region_resize)(i_as,
						 i_region,
						 t_vsize,
						 i_region*);
  t_status		(*region_coalesce)(i_as,
						   i_region,
						   i_region,
						   i_region*);
  t_status		(*region_reserve)(i_as,
						  i_segment,
						  t_paddr,
						  t_options,
						  t_vaddr,
						  t_vsize,
						  i_region*);
  t_status		(*region_release)(i_as,
						  i_region);
  t_status		(*region_flush)(i_as);
  t_status		(*region_initialize)(t_vaddr,
						     t_vsize);
  t_status		(*region_clean)(void);
}				d_region;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/region/region-fit.c
 */

/*
 * ../../core/region/region-fit.c
 */

t_status		region_show(i_as			asid,
				    i_region			regid,
				    mt_margin			margin);

t_status		region_dump(void);

t_status		region_protect(i_as			as,
				       t_vaddr			address,
				       t_vsize			size);

t_status		region_unprotect(i_as			as,
					 t_vaddr		address,
					 t_vsize		size);

t_bool			region_protected(i_as			as,
					 t_vaddr		address);

t_status		region_fit_first(i_as			asid,
					 t_vsize		size,
					 t_vaddr*		address);

t_status		region_space(i_as			asid,
				     t_vsize			size,
				     t_vaddr*			address);

t_status		region_inject(i_as			as,
				      o_region*			object,
				      i_region*			id);

t_status		region_split(i_as			asid,
				     i_region			regid,
				     t_vsize			size,
				     i_region*			left,
				     i_region*			right);

t_status		region_resize(i_as			as,
				      i_region			old,
				      t_vsize			size,
				      i_region*			new);

t_status		region_coalesce(i_as			asid,
					i_region		left,
					i_region		right,
					i_region*		region);

t_status		region_reserve(i_as			asid,
				       i_segment		segid,
				       t_paddr			offset,
				       t_options		options,
				       t_vaddr			address,
				       t_vsize			size,
				       i_region*		region);

t_status		region_release(i_as			asid,
				       i_region			regid);

t_bool			region_locate(i_as			as,
				      t_vaddr			address,
				      i_region*			id);

t_status		region_flush(i_as			asid);

t_status		region_exist(i_as			asid,
				     i_region			regid);

t_status		region_get(i_as				asid,
				   i_region			regid,
				   o_region**			object);

t_status		region_initialize(t_vaddr		base,
					  t_vsize		size);

t_status		region_clean(void);


/*
 * eop
 */

#endif
