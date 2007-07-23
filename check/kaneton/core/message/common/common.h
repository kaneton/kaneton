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

# include <libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

extern i_task ktask;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../basic_01u/basic_01u.c
 *	../async_01u/async_01u.c
 *	../async_02u/async_02u.c
 *	../async_03u/async_03u.c
 *	../sync_01u/sync_01u.c
 *	../sync_02u/sync_02u.c
 *	../sync_03u/sync_03u.c
 *	../blocking_01u/blocking_01u.c
 *	../sender_01u/sender_01u.c
 *	../sender_02u/sender_02u.c
 *	../recvsz_01u/recvsz_01u.c
 *	../recvsz_02u/recvsz_02u.c
 *	../triangle_01u/triangle_01u.c
 *	../fifo_01u/fifo_01u.c
 *	../ring_01u/ring_01u.c
 *	../pipe_01u/pipe_01u.c
 *	../basic_01k/basic_01k.c
 *	../async_01k/async_01k.c
 *	../async_02k/async_02k.c
 *	../async_03k/async_03k.c
 *	../sync_01k/sync_01k.c
 *	../sync_02k/sync_02k.c
 *	../sync_03k/sync_03k.c
 *	../blocking_01k/blocking_01k.c
 *	../sender_01k/sender_01k.c
 *	../sender_02k/sender_02k.c
 *	../recvsz_01k/recvsz_01k.c
 *	../recvsz_02k/recvsz_02k.c
 *	../triangle_01k/triangle_01k.c
 *	../fifo_01k/fifo_01k.c
 *	../ring_01k/ring_01k.c
 *	../pipe_01k/pipe_01k.c
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


/*
 * ../basic_01u/basic_01u.c
 */

void		check_message_basic_01u(void);


/*
 * ../async_01u/async_01u.c
 */

void		check_message_async_01u(void);


/*
 * ../async_02u/async_02u.c
 */

void		check_message_async_02u(void);


/*
 * ../async_03u/async_03u.c
 */

void		check_message_async_03u(void);


/*
 * ../sync_01u/sync_01u.c
 */

void		check_message_sync_01u(void);


/*
 * ../sync_02u/sync_02u.c
 */

void		check_message_sync_02u(void);


/*
 * ../sync_03u/sync_03u.c
 */

void		check_message_sync_03u(void);


/*
 * ../blocking_01u/blocking_01u.c
 */

void		check_message_blocking_01u(void);


/*
 * ../sender_01u/sender_01u.c
 */

void		check_message_sender_01u(void);


/*
 * ../sender_02u/sender_02u.c
 */

void		check_message_sender_02u(void);


/*
 * ../recvsz_01u/recvsz_01u.c
 */

void		check_message_recvsz_01u(void);


/*
 * ../recvsz_02u/recvsz_02u.c
 */

void		check_message_recvsz_02u(void);


/*
 * ../triangle_01u/triangle_01u.c
 */

void		check_message_triangle_01u(void);


/*
 * ../fifo_01u/fifo_01u.c
 */

void		check_message_fifo_01u(void);


/*
 * ../ring_01u/ring_01u.c
 */

void		check_message_ring_01u(void);


/*
 * ../pipe_01u/pipe_01u.c
 */

void		check_message_pipe_01u(void);


/*
 * ../basic_01k/basic_01k.c
 */

void		check_message_basic_01k(void);


/*
 * ../async_01k/async_01k.c
 */

void		check_message_async_01k(void);


/*
 * ../async_02k/async_02k.c
 */

void		check_message_async_02k(void);


/*
 * ../async_03k/async_03k.c
 */

void		check_message_async_03k(void);


/*
 * ../sync_01k/sync_01k.c
 */

void		check_message_sync_01k(void);


/*
 * ../sync_02k/sync_02k.c
 */

void		check_message_sync_02k(void);


/*
 * ../sync_03k/sync_03k.c
 */

void		check_message_sync_03k(void);


/*
 * ../blocking_01k/blocking_01k.c
 */

void		check_message_blocking_01k(void);


/*
 * ../sender_01k/sender_01k.c
 */

void		check_message_sender_01k(void);


/*
 * ../sender_02k/sender_02k.c
 */

void		check_message_sender_02k(void);


/*
 * ../recvsz_01k/recvsz_01k.c
 */

void		check_message_recvsz_01k(void);


/*
 * ../recvsz_02k/recvsz_02k.c
 */

void		check_message_recvsz_02k(void);


/*
 * ../triangle_01k/triangle_01k.c
 */

void		check_message_triangle_01k(void);


/*
 * ../fifo_01k/fifo_01k.c
 */

void		check_message_fifo_01k(void);


/*
 * ../ring_01k/ring_01k.c
 */

void		check_message_ring_01k(void);


/*
 * ../pipe_01k/pipe_01k.c
 */

void		check_message_pipe_01k(void);


/*
 * eop
 */

#endif /* !CHECK_MESSAGE_COMMON_H_ */
