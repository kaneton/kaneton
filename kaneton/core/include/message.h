/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/message.h
 *
 * created       julien quintard   [wed jun  6 13:34:19 2007]
 * updated       matthieu bucchianeri   [thu sep  6 00:19:29 2007]
 */

#ifndef CORE_MESSAGE_H
#define CORE_MESSAGE_H			1

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

#define MESSAGE_BPT_NODESZ	4096

/*
 * values for completed member
 */

#define MESSAGE_REQUEST_PENDING		0
#define MESSAGE_REQUEST_COMPLETED	1

/*
 * predefined message types
 */

#define MESSAGE_TYPE_INTERFACE		0
#define MESSAGE_TYPE_EVENT		1
#define MESSAGE_TYPE_TIMER		2

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * message copying request
 */

typedef struct
{
  t_status		completed;
}			t_message_request;

/*
 * message object
 */

typedef struct
{
  t_id			message;

  i_as			asid;
  i_thread		blocked;
  void*			data;
  t_vsize		size;
  i_node		sender;

  machine_data(o_message);
}			o_message;

/*
 * message type object
 */

typedef struct
{
  t_id			type;

  t_id			id;

  t_vsize		size;

  i_set			queue;
  i_set			waiters;

  machine_data(o_message_type);
}			o_message_type;

/*
 * message manager
 */

typedef struct
{
  machine_data(m_message);
}			m_message;

/*
 * the message architecture-dependent interface
 */

typedef struct
{
  t_error		(*message_register)(i_task,
					    t_type,
					    t_vsize);
  t_error		(*message_flush)(i_task);
  t_error		(*message_send)(i_task,
					i_node,
					t_type,
					t_vaddr,
					t_vsize);
  t_error		(*message_transmit)(i_task,
					    i_node,
					    t_type,
					    t_vaddr,
					    t_vsize);
  t_error		(*message_throw)(i_task,
					 i_node,
					 t_type,
					 t_vaddr,
					 t_vsize,
					 t_message_request*);
  t_error		(*message_receive)(i_task,
					   t_type,
					   t_vaddr,
					   t_vsize*,
					   i_node*);
  t_error		(*message_grab)(i_task,
					t_type,
				       t_vaddr,
				       t_message_request*);
  t_error		(*message_poll)(i_task,
					t_type,
					t_vaddr,
					t_vsize*,
					i_node*);
  t_error		(*message_state)(i_task,
					 t_message_request);
  t_error		(*message_wait)(i_task,
					t_message_request,
					t_vsize*,
					i_node*);
  t_error		(*message_return)(i_thread,
					  t_error);
  t_error		(*message_return_info)(i_thread,
					       t_error,
					       t_vsize,
					       i_node);
  t_error		(*message_initialize)(void);
  t_error		(*message_clean)(void);
}			d_message;

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * check
 */

#define MESSAGE_CHECK(_message_)					\
  {									\
    if ((_message_) == NULL)						\
      return (ERROR_UNKNOWN);						\
  }

/*
 * enter
 */

#define MESSAGE_ENTER(_message_)					\
  {									\
    MESSAGE_CHECK((_message_));						\
  }

/*
 * leave
 */

#define MESSAGE_LEAVE(_message_, _error_)				\
  {									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/message/message.c
 */

/*
 * ../../core/message/message.c
 */

t_error			message_register(i_task			task,
					 t_type			type,
					 t_vsize		size);

t_error			message_size(i_task			task,
				     t_type			type,
				     t_vsize*			size);

t_error			message_flush(i_task			task);

t_error			message_send(i_task			task,
				     i_node			destination,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize			size);

t_error			message_transmit(i_task			task,
					 i_node			destination,
					 t_type			type,
					 t_vaddr		data,
					 t_vsize		size);

t_error			message_throw(i_task			task,
				      i_node			destination,
				      t_type			type,
				      t_vaddr			data,
				      t_vsize			size,
				      t_message_request*	request);

t_error			message_receive(i_task			task,
					t_type			type,
					t_vaddr			data,
					t_vsize*		size,
					i_node*			sender);

t_error			message_grab(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     t_message_request*		request);

t_error			message_poll(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize*			size,
				     i_node*			sender);

t_error			message_state(i_task			task,
				      t_message_request		request);

t_error			message_wait(i_task			task,
				     t_message_request		request,
				     t_vsize*			size,
				     i_node*			sender);

t_error			message_return(i_thread			thread,
				       t_error			code);

t_error			message_return_info(i_thread		thread,
					    t_error		code,
					    t_vsize		size,
					    i_node		sender);

t_error			message_initialize(void);

t_error			message_clean(void);


/*
 * eop
 */

#endif
