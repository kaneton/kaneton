/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/core/include/types.h
 *
 * created       julien quintard   [wed jun  6 22:38:37 2007]
 * updated       julien quintard   [mon nov 22 11:50:26 2010]
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

typedef struct
{
  t_vaddr			base;
  t_vsize			size;
}				t_stack;

/*
 * special types
 */

typedef t_uint8			t_boolean;
typedef t_uint32		t_options;
typedef t_uint32		t_permissions;
typedef t_uint32		t_type;
typedef t_uint32		t_state;
typedef t_uint32		t_status;
typedef t_uint32		t_class;
typedef t_uint32		t_behaviour;
typedef t_uint32		t_priority;
typedef t_uint32		t_quantum;
typedef t_uint32		t_operations;
typedef t_uint32		t_timeslice;

#endif
