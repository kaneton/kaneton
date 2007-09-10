/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/k4_tests/kaneton/check/kaneton/core/message/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sun jun 10 18:58:48 2007]
 */

#ifndef CHECK_MESSAGE_COMMON_H_
# define CHECK_MESSAGE_COMMON_H_

# include <libc/libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

extern i_task ktask;
extern m_kernel* kernel;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 */

/*
 * common.c
 */

void	check_message_common(void);

t_uint32 check_cmos_sec(void);

void	check_message_tests(void);

int		check_task_create(t_class		class,
				  i_task		*task);

int		check_thread_create(i_task		task,
				    t_prior		prior,
				    t_vaddr		entry,
				    i_thread		*thread);

t_error			syscall_message_register(t_type		type,
						 t_vsize	size);

t_error			syscall_message_size(t_type		type,
					     t_vsize*		size);

t_error			syscall_message_send(i_node		destination,
					     t_type		type,
					     t_vaddr		data,
					     t_vsize		size);

t_error			syscall_message_transmit(i_node		destination,
						 t_type		type,
						 t_vaddr	data,
						 t_vsize	size);

t_error			syscall_message_throw(i_node		destination,
					      t_type		type,
					      t_vaddr		data,
					      t_vsize		size,
					      t_message_request*request);

t_error			syscall_message_receive(t_type		type,
						t_vaddr		data,
						t_vsize*	size,
						i_node*		sender);

t_error			syscall_message_grab(t_type		type,
					     t_vaddr		data,
					     t_message_request*	request);

t_error			syscall_message_poll(t_type		type,
					     t_vaddr		data,
					     t_vsize*		size,
					     i_node*		sender);

t_error			syscall_message_state(t_message_request	request);

t_error			syscall_message_wait(t_message_request	request,
					     t_vsize*		size,
					     i_node*		sender);

t_error			syscall_message_send_unlock(i_node		dest,
						    t_type		type,
						    t_vaddr		data,
						    t_vsize		size,
						    t_uint8*		lock);


/*
 * eop
 */

#endif /* !CHECK_MESSAGE_COMMON_H_ */
