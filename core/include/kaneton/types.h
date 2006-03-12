/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/kaneton/types.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:27:45 2006]
 */

#ifndef KANETON_TYPES_H
#define KANETON_TYPES_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

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
typedef t_uint32		t_fit;
typedef t_uint32		t_state;
typedef t_uint32		t_class;
typedef t_uint32		t_prior;
typedef t_uint32		t_behav;

/*
 * identifier types
 */

typedef t_uint64		t_id;

typedef t_id			t_staid;
typedef t_id			t_setid;
typedef t_id			t_asid;
typedef t_id			t_segid;
typedef t_id			t_regid;
typedef t_id			t_eventid;
typedef t_id			t_timerid;
typedef t_id			t_tskid;
typedef t_id			t_thrid;

/*
 * error type
 */

typedef t_sint32		t_error;


/*
 * XXX EVENT: move me !
 */

typedef void                    (*t_event_hdl)(t_uint32);

/*
 * XXX EVENT: move me !
 */

typedef enum
  {
    E_HANDLE,
    E_NOTIFY
  } e_event_type;

/*
 * XXX EVENT: move me !
 */

typedef union
{
  t_event_hdl                   function;
  t_tskid                       taskid;
}                               u_event_handler;


#endif
