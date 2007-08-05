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
 * updated       matthieu bucchianeri   [sun aug  5 23:37:43 2007]
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

typedef t_error			(*t_timer_handler)(i_timer, t_vaddr);
typedef union
{
  t_timer_handler		function;
  i_task			taskid;
}				u_timer_handler;

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
