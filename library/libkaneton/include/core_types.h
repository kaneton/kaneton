/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan.../library/libkaneton/include/core_types.h
 *
 * created       matthieu bucchianeri   [sun aug  5 23:36:14 2007]
 * updated       matthieu bucchianeri   [thu sep  6 21:48:46 2007]
 */

#ifndef LIBKANETON_CORE_TYPES_H
#define LIBKANETON_CORE_TYPES_H

/*
 * ---------- dependencies ----------------------------------------------------
 */


#include <core/wait.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef void			(*t_event_handler)(i_event, t_vaddr);
typedef union
{
  t_event_handler		function;
  i_task			taskid;
}				u_event_handler;

typedef struct
{
  i_event			id;

  t_vaddr			data;
}				o_event_message;

typedef t_error			(*t_timer_handler)(i_timer, t_vaddr);
typedef union
{
  t_timer_handler		function;
  i_task			taskid;
}				u_timer_handler;

typedef struct
{
  i_timer			id;

  t_vaddr			data;
}				o_timer_message;

typedef struct
{
  t_vaddr			pc;
  t_vaddr			sp;
}				t_thread_context;

typedef struct
{
  /* XXX */
}		t_message_request;

#endif
