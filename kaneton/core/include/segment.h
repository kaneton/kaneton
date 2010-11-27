/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.TETON/kaneton/core/include/segment.h
 *
 * created       julien quintard   [wed jun  6 14:00:28 2007]
 * updated       julien quintard   [sat nov 27 05:28:56 2010]
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
 * ---------- macros ----------------------------------------------------------
 */

/*
 * set parameters
 */

#define SEGMENT_BPT_NODESZ	4096

/*
 * segment types
 */

#define SEGMENT_TYPE_MEMORY	(1 << 0)
#define SEGMENT_TYPE_CATCH	(1 << 1)
#define SEGMENT_TYPE_SYSTEM	(1 << 2)

/*
 * ---------- algorithms ------------------------------------------------------
 */

#define SEGMENT_ALGORITHM_FIT	(1 << 0)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * segment object
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
 * segment manager
 */

typedef struct
{
  o_id				id;

  t_paddr			start;
  t_psize			size;

  i_set				segments;

  machine_data(m_segment);
}				m_segment;

/*
 * the segment architecture-dependent interface
 */

typedef struct
{
  t_error			(*segment_show)(i_segment);
  t_error			(*segment_clone)(i_as,
						 i_segment,
						 i_segment*);
  t_error			(*segment_inject)(i_as,
						  o_segment*,
						  i_segment*);
  t_error			(*segment_give)(i_as,
						i_segment);
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
  t_error			(*segment_catch)(i_as,
						 i_segment);
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
 *      ../../core/segment/segment.c
 *      ../../core/segment/segment-fit.c
 */

/*
 * ../../core/segment/segment.c
 */

t_error			segment_show(i_segment			segid);

t_error			segment_dump(void);

t_error			segment_clone(i_as			asid,
				      i_segment			old,
				      i_segment*		new);

t_error			segment_inject(i_as		asid,
				       o_segment*	o,
				       i_segment*	segid);

t_error			segment_give(i_as		asid,
				     i_segment		segid);

t_error			segment_resize(i_segment	old,
				       t_psize		size,
				       i_segment*	new);

t_error			segment_split(i_segment		segid,
				      t_psize		size,
				      i_segment*	left,
				      i_segment*	right);

t_error			segment_coalesce(i_segment	left,
					 i_segment	right,
					 i_segment*	segid);

t_error			segment_read(i_segment		segid,
				     t_paddr		offs,
				     void*		buff,
				     t_psize		sz);

t_error			segment_write(i_segment		segid,
				      t_paddr		offs,
				      const void*	buff,
				      t_psize		sz);

t_error			segment_copy(i_segment		dst,
				     t_paddr		offsd,
				     i_segment		src,
				     t_paddr		offss,
				     t_psize		sz);

t_error			segment_reserve(i_as			asid,
					t_psize			size,
					t_permissions		perms,
					i_segment*		segid);

t_error			segment_release(i_segment		segid);

t_error			segment_catch(i_as			asid,
				      i_segment			segid);

t_error			segment_permissions(i_segment		segid,
					    t_permissions	perms);

t_error			segment_type(i_segment			segid,
				     t_type			type);

t_error			segment_flush(i_as			asid);

t_error			segment_exist(i_segment			segid);

t_error			segment_get(i_segment			segid,
				    o_segment**			o);

t_error			segment_initialize(void);

t_error			segment_clean(void);


/*
 * ../../core/segment/segment-fit.c
 */

t_error			segment_fit_first(i_as			asid,
					  t_psize		size,
					  t_paddr*		address);

t_error			segment_space(i_as			asid,
				      t_psize			size,
				      t_paddr*			address);


/*
 * eop
 */

#endif
