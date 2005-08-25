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

int			set_dump_array(t_setid			setid);

int			set_head_array(t_setid			setid,
				       t_iterator*		iterator);

int			set_tail_array(t_setid			setid,
				       t_iterator*		iterator);

int			set_prev_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		previous);

int			set_next_array(t_setid			setid,
				       t_iterator		current,
				       t_iterator*		next);

int			set_insert_head_array(t_setid		setid,
					      void*		data);

int			set_insert_tail_array(t_setid		setid,
					      void*		data);

int			set_insert_before_array(t_setid		setid,
						t_iterator	iterator,
						void*		data);

int			set_insert_after_array(t_setid		setid,
					       t_iterator	iterator,
					       void*		data);

int			set_add_array(t_setid			setid,
				      void*			data);

int			set_remove_array(t_setid		setid,
					 t_id			id);

int			set_flush_array(t_setid			setid);

int			set_locate_ll(t_setid			setid,
				      t_id			id,
				      t_iterator*		iterator);

int			set_object_array(t_setid		setid,
					 t_iterator		iterator,
					 void**			data);

int			set_rsv_array(t_opts			opts,
				      t_setsz			initsz,
				      t_size			datasz,
				      t_setid*			setid);

int			set_rel_array(t_setid			setid);


/*
 * eop
 */

#endif
