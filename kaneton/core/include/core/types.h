/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/types.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [fri aug 18 18:55:34 2006]
 */

#ifndef CORE_TYPES_H
#define CORE_TYPES_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * register types
 */

typedef t_uint8			t_reg8;
typedef t_uint16		t_reg16;
typedef t_uint32		t_reg32;
typedef t_uint64		t_reg64;

/*
 * memory types
 */

typedef t_uint32		t_paddr;
typedef t_uint32		t_psize;
typedef t_uint32		t_vaddr;
typedef t_uint32		t_vsize;

typedef t_uint32		t_size;
typedef t_uint32		t_offset;

/*
 * special types
 */

typedef t_uint32		t_opts;
typedef t_uint32		t_perms;
typedef t_uint32		t_type;
typedef t_uint32		t_lookup;
typedef t_uint32		t_state;
typedef t_uint32		t_status;
typedef t_uint32		t_class;
typedef t_uint32		t_behav;
typedef t_uint32		t_prior;
typedef t_uint32		t_quantum;
typedef t_uint32		t_operations;
typedef t_uint32		t_timeslice;

// XXX
typedef t_uint64		t_id;

typedef t_id			i_cpu;

typedef t_id			i_task;

typedef t_id			i_as;

typedef t_id			i_event;

typedef t_id			i_segment;

typedef t_id			i_region;


typedef t_id			i_thread;

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this identifier identifies a unique machine in the distributed system.
 */

typedef t_id			i_machine;

/*
 * unique identifier of a node in the distributed system.
 *
 * a node is a communicating task located in a machine of the distributed
 * system.
 */

typedef struct
{
  i_machine			machine;
  /* XXX i_task */ t_id			task;
}				i_node;

#endif
