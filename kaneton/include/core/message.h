/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/message.h
 *
 * created       matthieu bucchianeri   [sat jun 17 19:39:50 2006]
 * updated       julien quintard   [thu jul  6 16:53:29 2006]
 */

#ifndef CORE_MESSAGE_H
#define CORE_MESSAGE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <core/id.h>
#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * message box
 */

typedef t_id i_msgqueue;
typedef t_id i_msg;
typedef t_id i_waitqueue;
typedef t_uint32 t_tag;

typedef struct
{
  t_vaddr	data;
  t_size	sz;
  i_task	task;
}		t_waiter;

typedef struct
{
  void*		data;
  t_size	sz;

  /* timestamp ? */
}		o_msg;

typedef struct
{
  t_id		id;

  /* Unique queue for now */
  i_msgqueue	msgqueue;

  i_waitqueue	receivers;
  i_waitqueue	senders;
}		t_local_box;

/*
 * message manager
 */

typedef struct
{
  i_stats	stats;

  i_set		local_boxes;

  machdep_data(m_message);
}		m_message;

/*
 * the message architecture-dependent interface
 */

typedef struct
{
  t_error			(*message_init)(void);
  t_error			(*message_clean)(void);
}				d_message;

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

t_error			message_async_send(i_task sender, i_node dest, t_tag tag, void* data, t_size size);

t_error			message_async_recv(i_task taskid, t_tag tag, void* data, size_t maxsz);

t_error			message_sync_send(i_task sender, i_node dest, t_tag tag, void* data, t_size size);

t_error			message_sync_recv(i_task taskid, t_tag tag, void* data, size_t maxsz);

t_error			message_register(i_task taskid, t_tag tag);

t_uint32		syscall_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);

t_uint32		syscall_recv(t_uint32 tag, void* ptr, t_uint32 size);

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
