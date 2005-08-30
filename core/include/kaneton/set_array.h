/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_array.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Thu Jul 21 15:30:04 2005   mycure
 */

#ifndef KANETON_SET_ARRAY_H
#define KANETON_SET_ARRAY_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_opts			opts;

  t_size			datasz;

  t_uint8*			array;
  t_setsz			arraysz;
}				t_set_array;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set_array.c
 */

/*
 * ../../kaneton/set/set_array.c
 */

t_error			set_dump_array(t_setid			setid);

t_error			set_expand_array(o_set*			o);

t_error			set_insert_array(o_set*			o,
					 t_sint32		position);

t_error			set_head_array(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_tail_array(t_setid			setid,
				       t_iterator*		iterator);

t_error			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous);

t_error			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next);

t_error			set_insert_head_array(t_setid		setid,
					      void*		data);

t_error			set_insert_tail_array(t_setid		setid,
					      void*		data);

t_error			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data);

t_error			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data);

t_error			set_add_array(t_setid			setid,
				      void*			data);

t_error			set_remove_array(t_setid		setid,
					 t_id			id);

t_error			set_flush_array(t_setid			setid);

t_error			set_locate_array(t_setid		setid,
					 t_id			id,
					 t_iterator*		iterator);

t_error			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data);

t_error			set_rsv_array(t_opts			opts,
				      t_setsz			initsz,
				      t_size			datasz,
				      t_setid*			setid);

t_error			set_rel_array(t_setid			setid);


/*
 * eop
 */

#endif
