/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/types.h
 *
 * created       julien quintard   [wed jun  6 22:38:37 2007]
 * updated       julien quintard   [wed mar  2 20:26:08 2011]
 */

#ifndef CORE_TYPES_H
#define CORE_TYPES_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <machine/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the general-purpose size and offset types.
 */

typedef t_uint32		t_size;
typedef t_uint32		t_offset;

/*
 * the types below are used in a variety of situations. however their
 * names are pretty clear about their usage.
 */

typedef t_uint8			t_boolean;
typedef t_uint32		t_options;
typedef t_uint32		t_permissions;
typedef t_uint32		t_type;
typedef t_uint32		t_state;
typedef t_uint32		t_class;
typedef t_uint32		t_behaviour;
typedef t_uint32		t_priority;
typedef t_uint32		t_quantum;
typedef t_uint32		t_operations;
typedef t_uint64		t_timeslice;
typedef t_sint32		t_value;
typedef t_sint64		t_delay;
typedef t_uint32		t_width;
typedef t_uint64		t_flags;
typedef t_uint32		t_privilege;
typedef t_uint32		t_data;

#endif
