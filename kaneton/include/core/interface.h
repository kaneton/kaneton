/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/interface.h
 *
 * created       julien quintard   [tue jun 27 23:55:43 2006]
 * updated       julien quintard   [tue jun 27 23:56:41 2006]
 */

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define INTERFACE_OP_MAP_RESERVE 1
#define INTERFACE_OP_MAP_RELEASE 2

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_id			id;
  i_node		node;
  union
  {
    struct
    {
      t_operations	operation;
      t_capability	capability;
      union
      {
	struct
	{
	  t_opts	arg1;
	  t_vsize	arg2;
	  t_perms	arg3;
	}		map_reserve;
	struct
	{
	  t_vaddr	arg1;
	}		map_release;
      }	u;
    } request;
    struct
    {
      t_error		error;
      union
      {
	struct
	{
	  t_vaddr	result1;
	}		map_reserve;
      } u;
    } reply;
  } u;
}			o_message;

/*
 * ---------- macro functions -------------------------------------------------
 */

/* XXX */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/interface/interface.c
 */

/*
 * ../../core/interface/interface.c
 */

void			interface_segment_perms(/*o_message*	message*/void);

void			interface_loop(void);

void			interface_map_reserve(o_message*	message);

void			interface_map_release(o_message*	message);


/*
 * eop
 */

#endif
