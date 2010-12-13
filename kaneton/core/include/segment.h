/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/segment.h
 *
 * created       julien quintard   [wed jun  6 14:00:28 2007]
 * updated       julien quintard   [sun dec 12 16:05:18 2010]
 */

#ifndef CORE_SEGMENT_H
#define CORE_SEGMENT_H			1

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
 * the support segment algorithms.
 */

#define SEGMENT_ALGORITHM_FIT		(1 << 0)

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * options.
 */

#define SEGMENT_OPTION_NONE		(1 << 0)
#define SEGMENT_OPTION_NORELOCATE	(1 << 1)

/*
 * the default size of the bpt nodes.
 */

#define SEGMENT_BPT_NODESZ		4096

/*
 * the types of segments.
 */

#define SEGMENT_TYPE_MEMORY		(1 << 0)
#define SEGMENT_TYPE_SYSTEM		(1 << 1)

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this macro-function computes a segment identifier based on the address.
 */

#define SEGMENT_IDENTIFIER(_object_)					\
  (i_segment)((_object_)->address / ___kaneton$framesz)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the segment object is identified by a unique identifier 'id' and
 * belongs to an address space 'as'.
 *
 * in addition, every segment has a type 'type', a start address 'address'
 * and a size 'size' along with a set of permissions 'permissions'.
 */

typedef struct
{
  i_segment			id;

  i_as				as;

  t_type			type;

  t_paddr			address;
  t_psize			size;

  t_permissions			permissions;

  machine_data(o_segment);
}				o_segment;

/*
 * the segment manager.
 *
 * the 'base' and 'size' attributes represent the start and size of
 * the segment space i.e the available physical memory.
 */

typedef struct
{
  t_paddr			base;
  t_psize			size;

  i_set				segments;

  machine_data(m_segment);
}				m_segment;

/*
 * the segment dispatcher.
 */

typedef struct
{
  t_error			(*segment_show)(i_segment,
						mt_margin);
  t_error			(*segment_dump)(void);
  t_error			(*segment_clone)(i_as,
						 i_segment,
						 i_segment*);
  t_error			(*segment_inject)(i_as,
						  o_segment*,
						  i_segment*);
  t_error			(*segment_give)(i_segment,
						i_as);
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
						   t_permissions,
						   i_segment*);
  t_error			(*segment_release)(i_segment);
  t_error			(*segment_permissions)(i_segment,
						       t_permissions);
  t_error			(*segment_type)(i_segment,
						t_type);
  t_error			(*segment_flush)(i_as);
  t_error			(*segment_initialize)();
  t_error			(*segment_clean)(void);
}				d_segment;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/segment/segment-fit.c
 */

/*
 * ../../core/segment/segment-fit.c
 */

t_error			segment_show(i_segment			segid,
				     mt_margin			margin);

t_error			segment_dump(void);

t_error			segment_fit_first(i_as			asid,
					  t_psize		size,
					  t_paddr*		address);

t_error			segment_space(i_as			asid,
				      t_psize			size,
				      t_paddr*			address);

t_error			segment_clone(i_as			asid,
				      i_segment			old,
				      i_segment*		new);

t_error			segment_inject(i_as			asid,
				       o_segment*		object,
				       i_segment*		id);

t_error			segment_give(i_segment			segid,
				     i_as			asid);

t_error			segment_resize(i_segment		old,
				       t_psize			size,
				       t_options		options,
				       i_segment*		new);

t_error			segment_split(i_segment			segid,
				      t_psize			size,
				      i_segment*		left,
				      i_segment*		right);

t_error			segment_coalesce(i_segment		left,
					 i_segment		right,
					 i_segment*		id);

t_error			segment_read(i_segment			segid,
				     t_paddr			offs,
				     void*			buffer,
				     t_psize			sz);

t_error			segment_write(i_segment			segid,
				      t_paddr			offs,
				      const void*		buffer,
				      t_psize			sz);

t_error			segment_copy(i_segment			dst,
				     t_paddr			offsd,
				     i_segment			src,
				     t_paddr			offss,
				     t_psize			sz);

t_error			segment_reserve(i_as			asid,
					t_psize			size,
					t_permissions		perms,
					i_segment*		id);

t_error			segment_release(i_segment		segid);

t_error			segment_permissions(i_segment		segid,
					    t_permissions	perms);

t_error			segment_type(i_segment			segid,
				     t_type			type);

t_error			segment_flush(i_as			asid);

t_error			segment_locate(t_paddr			address,
				       i_segment*		id);

t_error			segment_exist(i_segment			segid);

t_error			segment_get(i_segment			segid,
				    o_segment**			object);

t_error			segment_initialize(t_paddr		base,
					   t_psize		size);

t_error			segment_clean(void);


/*
 * eop
 */

#endif
