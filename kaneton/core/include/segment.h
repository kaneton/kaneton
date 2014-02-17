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
 * updated       julien quintard   [fri apr  8 09:04:47 2011]
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
 * the options related to the segments.
 *
 * the 'system' option indicates the segment contains data related to
 * the system management such as hardware data structure for instance.
 *
 * the 'norelocate' option is used by the resize() function.
 */

#define SEGMENT_OPTION_NONE		(1 << 0)
#define SEGMENT_OPTION_SYSTEM		(1 << 1)

#define SEGMENT_OPTION_NORELOCATE	(1 << 1)

/*
 * the default size of the bpt nodes.
 */

#define SEGMENT_BPT_NODESZ		4096

/*
 * this macro defines the size of the vault i.e the set of segments
 * which must be temporarily prevented from being reserved.
 */

#define SEGMENT_VAULT_SIZE		8

/*
 * these macros indicate the state of a vault entry.
 */

#define SEGMENT_VAULT_STATE_AVAILABLE	0
#define SEGMENT_VAULT_STATE_USED	1

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
 * this structure represents a memory area which needs to be prevented
 * from being reserved.
 */

typedef struct
{
  t_state			state;
  t_paddr			address;
  t_psize			size;
}				s_segment_zone;

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

  t_paddr			address;
  t_psize			size;

  t_permissions			permissions;

  t_options			options;

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

  s_segment_zone		vault[SEGMENT_VAULT_SIZE];

  machine_data(m_segment);
}				m_segment;

/*
 * the segment dispatcher.
 */

typedef struct
{
  t_status		(*segment_show)(i_segment,
						mt_margin);
  t_status		(*segment_dump)(void);
  t_status		(*segment_protect)(t_paddr,
						   t_psize);
  t_status		(*segment_unprotect)(t_paddr,
						     t_psize);
  t_status		(*segment_clone)(i_as,
						 i_segment,
						 i_segment*);
  t_status		(*segment_inject)(i_as,
						  o_segment*,
						  i_segment*);
  t_status		(*segment_give)(i_segment,
						i_as);
  t_status		(*segment_resize)(i_segment,
						  t_psize,
						  i_segment*);
  t_status		(*segment_split)(i_segment,
						 t_psize,
						 i_segment*,
						 i_segment*);
  t_status		(*segment_coalesce)(i_segment,
						    i_segment,
						    i_segment*);
  t_status		(*segment_read)(i_segment,
						t_paddr,
						void*,
						t_psize);
  t_status		(*segment_write)(i_segment,
						 t_paddr,
						 const void*,
						 t_psize);
  t_status		(*segment_copy)(i_segment,
						t_paddr,
						i_segment,
						t_paddr,
						t_psize);
  t_status		(*segment_reserve)(i_as,
						   t_psize,
						   t_permissions,
						   i_segment*);
  t_status		(*segment_release)(i_segment);
  t_status		(*segment_permissions)(i_segment,
						       t_permissions);
  t_status		(*segment_flush)(i_as);
  t_status		(*segment_initialize)();
  t_status		(*segment_clean)(void);
}				d_segment;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/segment/segment-fit.c
 */

/*
 * ../../core/segment/segment-fit.c
 */

t_status			segment_show(i_segment			segid,
					     mt_margin			margin);

t_status			segment_dump(void);

t_status			segment_protect(t_paddr			address,
						t_psize			size);

t_status			segment_unprotect(t_paddr		address,
						  t_psize		size);

t_bool					segment_protected(t_paddr		address);

t_status			segment_fit_first(i_as			asid,
						  t_psize		size,
						  t_paddr*		address);

t_status			segment_space(i_as			asid,
					      t_psize			size,
					      t_paddr*			address);

t_status			segment_clone(i_as			asid,
					      i_segment			old,
					      i_segment*		new);

t_status			segment_inject(i_as			asid,
					       o_segment*		object,
					       i_segment*		id);

t_status			segment_give(i_segment			segid,
					     i_as			asid);

t_status			segment_resize(i_segment		old,
					       t_psize			size,
					       t_options		options,
					       i_segment*		new);

t_status			segment_split(i_segment			segid,
		    			      t_psize			size,
					      i_segment*		left,
					      i_segment*		right);

t_status			segment_coalesce(i_segment		left,
						 i_segment		right,
						 i_segment*		id);

t_status			segment_read(i_segment			segid,
					     t_paddr			offs,
					     void*			buffer,
					     t_psize			sz);

t_status			segment_write(i_segment			segid,
					      t_paddr			offs,
					      const void*		buffer,
					      t_psize			sz);

t_status			segment_copy(i_segment			dst,
					     t_paddr			offsd,
					     i_segment			src,
					     t_paddr			offss,
					     t_psize			sz);

t_status			segment_reserve(i_as			asid,
						t_psize			size,
						t_permissions		perms,
						t_options		options,
						i_segment*		id);

t_status			segment_release(i_segment		segid);

t_status			segment_permissions(i_segment		segid,
						    t_permissions	perms);

t_status			segment_flush(i_as			asid);

t_bool				segment_locate(t_paddr			address,
					       i_segment*		id);

t_bool				segment_exist(i_segment			segid);

t_status			segment_get(i_segment			segid,
					    o_segment**			object);

t_status			segment_initialize(t_paddr		base,
						   t_psize		size);

t_status			segment_clean(void);


/*
 * eop
 */

#endif
