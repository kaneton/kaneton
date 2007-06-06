/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/message.h
 *
 * created       julien quintard   [wed jun  6 13:34:19 2007]
 * updated       julien quintard   [wed jun  6 16:21:53 2007]
 */

#ifndef GUARD_CORE_MESSAGE
#define GUARD_CORE_MESSAGE		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/core.h>
#include <core/id.h>
#include <core/thread.h>
#include <core/as.h>
#include <core/stats.h>
#include <core/set.h>
#include <core/task.h>
#include <core/kernel.h>
#include <core/error.h>

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * message box
 */

typedef t_id		i_message_queue;
typedef t_id		i_message;
typedef t_id		i_message_waitqueue;
typedef t_uint32	t_tag;

typedef struct
{
  t_vaddr		data;
  t_vsize		sz;
  i_thread		thread;
  i_as			asid;
}			t_message_waiter;

typedef struct
{
  void*			data;
  t_vsize		sz;

  machdep_data(o_message);
}			o_message;

typedef struct
{
  t_id			id;

  i_message_queue	msgqueue;

  i_message_waitqueue	receivers;
  i_message_waitqueue	senders;
}			t_message_box;

/*
 * message manager
 */

typedef struct
{
  i_stats		stats;

  i_set			local_boxes;

  machdep_data(m_message);
}			m_message;

/*
 * the message architecture-dependent interface
 */

typedef struct
{
  t_error		(*message_init)(void);
  t_error		(*message_clean)(void);
  t_error		(*message_epilogue)(i_thread, t_error);
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
									\
    STATS_BEGIN((_message_)->stats);					\
  }

/*
 * leave
 */

#define MESSAGE_LEAVE(_message_, _error_)				\
  {									\
    STATS_END((_message_)->stats, (_error_));				\
									\
    return (_error_);							\
  }

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/message/message.c
 *	../../core/message/node.c
 */

/*
 * ../../core/message/message.c
 */

t_error			message_init(void);

t_error			message_clean(void);

t_error			message_async_send(i_task	sender,
					   i_node	dest,
					   t_tag	tag,
					   t_vaddr	data,
					   t_vsize	size);

t_error			message_async_recv(i_task	taskid,
					   t_tag	tag,
					   t_vaddr	data,
					   t_vsize	maxsz);

t_error			message_sync_send(i_task	sender,
					  i_node	dest,
					  t_tag		tag,
					  t_vaddr	data,
					  t_vsize	size);

t_error			message_sync_recv(i_task	taskid,
					  t_tag		tag,
					  t_vaddr	data,
					  t_vsize	maxsz,
					  t_state	blocking);

t_error			message_register(i_task	taskid,
					 t_tag	tag);

t_error			message_test(void);


/*
 * ../../core/message/node.c
 */

i_machine get_node_machine(i_node node);

i_task get_node_task(i_node node);

void get_node(i_machine m, i_task t, i_node* node);

void get_kernel_node(i_node* node);

t_uint32 is_local_node(i_node node);

t_uint32 node_cmp(i_node a, i_node b);


/*
 * eop
 */

#endif
