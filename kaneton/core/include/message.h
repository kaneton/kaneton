/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/message.h
 *
 * created       julien quintard   [wed jun  6 13:34:19 2007]
 * updated       julien quintard   [fri dec 17 16:05:04 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * [XXX:improvements] the whole manager should be re-developed!
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
  t_uint32		completed;
}			s_message_request;

/*
 * message object
 */

typedef struct
{
  t_id			id;

  i_as			as;
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
  t_status	(*message_register)(i_task,
					    t_type,
					    t_vsize);
  t_status	(*message_flush)(i_task);
  t_status	(*message_send)(i_task,
					i_node,
					t_type,
					t_vaddr,
					t_vsize);
  t_status	(*message_transmit)(i_task,
					    i_node,
					    t_type,
					    t_vaddr,
					    t_vsize);
  t_status	(*message_throw)(i_task,
					 i_node,
					 t_type,
					 t_vaddr,
					 t_vsize,
					 s_message_request*);
  t_status	(*message_receive)(i_task,
					   t_type,
					   t_vaddr,
					   t_vsize*,
					   i_node*);
  t_status	(*message_grab)(i_task,
					t_type,
				       t_vaddr,
				       s_message_request*);
  t_status	(*message_poll)(i_task,
					t_type,
					t_vaddr,
					t_vsize*,
					i_node*);
  t_status	(*message_state)(i_task,
					 s_message_request);
  t_status	(*message_wait)(i_task,
					s_message_request,
					t_vsize*,
					i_node*);
  t_status	(*message_return)(i_thread,
					  t_status);
  t_status	(*message_return_info)(i_thread,
					       t_status,
					       t_vsize,
					       i_node);
  t_status	(*message_initialize)(void);
  t_status	(*message_clean)(void);
}			d_message;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/message/message.c
 */

/*
 * ../../core/message/message.c
 */

t_status		message_register(i_task			task,
					 t_type			type,
					 t_vsize		size);

t_status		message_size(i_task			task,
				     t_type			type,
				     t_vsize*			size);

t_status		message_flush(i_task			task);

t_status		message_send(i_task			task,
				     i_node			destination,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize			size);

t_status		message_transmit(i_task			task,
					 i_node			destination,
					 t_type			type,
					 t_vaddr		data,
					 t_vsize		size);

t_status		message_throw(i_task			task,
				      i_node			destination,
				      t_type			type,
				      t_vaddr			data,
				      t_vsize			size,
				      s_message_request*	request);

t_status		message_receive(i_task			task,
					t_type			type,
					t_vaddr			data,
					t_vsize*		size,
					i_node*			sender);

t_status		message_grab(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     s_message_request*		request);

t_status		message_poll(i_task			task,
				     t_type			type,
				     t_vaddr			data,
				     t_vsize*			size,
				     i_node*			sender);

t_status		message_state(i_task			task,
				      s_message_request		request);

t_status		message_wait(i_task			task,
				     s_message_request		request,
				     t_vsize*			size,
				     i_node*			sender);

t_status		message_return(i_thread			thread,
				       t_status		code);

t_status		message_return_info(i_thread		thread,
					    t_status	code,
					    t_vsize		size,
					    i_node		sender);

t_status		message_initialize(void);

t_status		message_clean(void);


/*
 * eop
 */

#endif
