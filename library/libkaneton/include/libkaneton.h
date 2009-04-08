/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan.../library/libkaneton/include/libkaneton.h
 *
 * created       matthieu bucchianeri   [sun aug  5 23:20:36 2007]
 * updated       matthieu bucchianeri   [thu sep  6 00:49:51 2007]
 */

#ifndef LIBKANETON_LIBKANETON_H
#define LIBKANETON_LIBKANETON_H

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/id.h>
#include <core/error.h>

#ifndef CORE_CORE_H
#include "core_types.h"
#include "core_macros.h"
#endif

// XXX
extern t_uint64 __task_id;
extern t_uint64 __as_id;

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define message_register(...)						\
  syscall_message_register(__VA_ARGS__)

#define message_size(...)						\
  syscall_message_size(__VA_ARGS__)

#define message_send(...)						\
  syscall_message_send(__VA_ARGS__)

#define message_send_unlock(...)					\
  syscall_message_send_unlock(__VA_ARGS__)

#define message_transmit(...)						\
  syscall_message_transmit(__VA_ARGS__)

#define message_throw(...)						\
  syscall_message_throw(__VA_ARGS__)

#define message_receive(...)						\
  syscall_message_receive(__VA_ARGS__)

#define message_grab(...)						\
  syscall_message_grab(__VA_ARGS__)

#define message_poll(...)						\
  syscall_message_poll(__VA_ARGS__)

#define message_state(...)						\
  syscall_message_state(__VA_ARGS__)

#define message_wait(...)						\
  syscall_message_wait(__VA_ARGS__)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../messaging.c
 *	../libkaneton.c
 *	../thread_args.c
 *	../thread_state_unlock.c
 */

/*
 * ../messaging.c
 */

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
 * ../libkaneton.c
 */

t_error		as_give(i_task		arg1,
			t_id		arg2);

t_error		as_vaddr(t_id		arg1,
			t_paddr		arg2,
			t_vaddr*		result1);

t_error		as_paddr(t_id		arg1,
			t_vaddr		arg2,
			t_paddr*		result1);

t_error		as_copy(t_id		arg1,
			t_vaddr		arg2,
			i_as		arg3,
			t_vaddr		arg4,
			t_vsize		arg5);

t_error		as_clone(i_task		arg1,
			t_id		arg2,
			i_as*		result1);

t_error		as_reserve(i_task		arg1,
			i_as*		result1);

t_error		as_release(t_id		arg1);

t_error		as_attribute_tskid(t_id		arg1,
			i_task*		result1);

t_error		event_reserve(t_id		arg1,
			t_type		arg2,
			u_event_handler		arg3,
			t_vaddr		arg4);

t_error		event_release(t_id		arg1);

t_error		event_attribute_type(t_id		arg1,
			t_type*		result1);

t_error		event_attribute_handler(t_id		arg1,
			u_event_handler*		result1);

t_error		event_attribute_data(t_id		arg1,
			t_vaddr*		result1);

t_error		io_grant(t_id		arg1,
			i_task		arg2,
			t_uint8		arg3);

t_error		io_deny(t_id		arg1,
			i_task		arg2,
			t_uint8		arg3);

t_error		io_read_8(i_task		arg1,
			t_id		arg2,
			t_uint8*		result1);

t_error		io_read_16(i_task		arg1,
			t_id		arg2,
			t_uint16*		result1);

t_error		io_read_32(i_task		arg1,
			t_id		arg2,
			t_uint32*		result1);

t_error		io_read_64(i_task		arg1,
			t_id		arg2,
			t_uint64*		result1);

t_error		io_write_8(i_task		arg1,
			t_id		arg2,
			t_uint8		arg3);

t_error		io_write_16(i_task		arg1,
			t_id		arg2,
			t_uint16		arg3);

t_error		io_write_32(i_task		arg1,
			t_id		arg2,
			t_uint32		arg3);

t_error		io_write_64(i_task		arg1,
			t_id		arg2,
			t_uint64		arg3);

t_error		map_reserve(t_id		arg1,
			t_opts		arg2,
			t_vsize		arg3,
			t_perms		arg4,
			t_vaddr*		result1);

t_error		map_release(t_id		arg1,
			t_vaddr		arg2);

t_error		map_resize(t_id		arg1,
			t_vaddr		arg2,
			t_vsize		arg3,
			t_vaddr*		result1);

t_error		region_reserve(t_id		arg1,
			i_segment		arg2,
			t_paddr		arg3,
			t_opts		arg4,
			t_vaddr		arg5,
			t_vsize		arg6,
			i_region*		result1);

t_error		region_release(i_as		arg1,
			t_id		arg2);

t_error		scheduler_quantum(t_quantum		arg1);

t_error		scheduler_yield(i_cpu		arg1);

t_error		scheduler_current(i_thread*		result1);

t_error		scheduler_add(i_thread		arg1);

t_error		scheduler_remove(i_thread		arg1);

t_error		scheduler_update(i_thread		arg1);

t_error		segment_clone(i_as		arg1,
			t_id		arg2,
			i_segment*		result1);

t_error		segment_give(i_as		arg1,
			t_id		arg2);

t_error		segment_copy(t_id		arg1,
			t_paddr		arg2,
			i_segment		arg3,
			t_paddr		arg4,
			t_psize		arg5);

t_error		segment_reserve(i_as		arg1,
			t_psize		arg2,
			t_perms		arg3,
			i_segment*		result1);

t_error		segment_release(i_segment		arg1);

t_error		segment_catch(i_as		arg1,
			t_id		arg2);

t_error		segment_perms(t_id		arg1,
			t_perms		arg2);

t_error		segment_type(t_id		arg1,
			t_type		arg2);

t_error		segment_attribute_asid(t_id		arg1,
			i_as*		result1);

t_error		segment_attribute_type(t_id		arg1,
			t_type*		result1);

t_error		segment_attribute_address(t_id		arg1,
			t_paddr*		result1);

t_error		segment_attribute_size(t_id		arg1,
			t_psize*		result1);

t_error		segment_attribute_perms(t_id		arg1,
			t_perms*		result1);

t_error		task_current(i_task*		result1);

t_error		task_clone(t_id		arg1,
			i_task*		result1);

t_error		task_reserve(t_class		arg1,
			t_behav		arg2,
			t_prior		arg3,
			i_task*		result1);

t_error		task_release(t_id		arg1);

t_error		task_priority(t_id		arg1,
			t_prior		arg2);

t_error		task_state(t_id		arg1,
			t_state		arg2);

t_error		task_wait(t_id		arg1,
			t_opts		arg2,
			t_wait*		result1);

t_error		task_attribute_parent(t_id		arg1,
			i_task*		result1);

t_error		task_attribute_class(t_id		arg1,
			t_class*		result1);

t_error		task_attribute_behav(t_id		arg1,
			t_behav*		result1);

t_error		task_attribute_prior(t_id		arg1,
			t_prior*		result1);

t_error		task_attribute_asid(t_id		arg1,
			i_as*		result1);

t_error		task_attribute_sched(t_id		arg1,
			t_state*		result1);

t_error		task_attribute_wait(t_id		arg1,
			t_wait*		result1);

t_error		thread_give(i_task		arg1,
			t_id		arg2);

t_error		thread_clone(i_task		arg1,
			t_id		arg2,
			i_thread*		result1);

t_error		thread_reserve(i_task		arg1,
			t_prior		arg2,
			i_thread*		result1);

t_error		thread_release(t_id		arg1);

t_error		thread_priority(t_id		arg1,
			t_prior		arg2);

t_error		thread_state(t_id		arg1,
			t_state		arg2);

t_error		thread_stack(t_id		arg1,
			t_stack		arg2);

t_error		thread_load(t_id		arg1,
			t_thread_context		arg2);

t_error		thread_store(t_id		arg1,
			t_thread_context*		result1);

t_error		thread_attribute_taskid(t_id		arg1,
			i_task*		result1);

t_error		thread_attribute_prior(t_id		arg1,
			t_prior*		result1);

t_error		thread_attribute_sched(t_id		arg1,
			t_state*		result1);

t_error		thread_attribute_wait(t_id		arg1,
			t_wait*		result1);

t_error		thread_attribute_stack(t_id		arg1,
			t_vaddr*		result1);

t_error		thread_attribute_stacksz(t_id		arg1,
			t_vsize*		result1);

t_error		timer_reserve(t_type		arg1,
			u_timer_handler		arg2,
			t_vaddr		arg3,
			t_uint32		arg4,
			t_uint32		arg5,
			i_timer*		result1);

t_error		timer_release(t_id		arg1);

t_error		timer_delay(t_id		arg1,
			t_uint32		arg2);

t_error		timer_repeat(t_id		arg1,
			t_uint32		arg2);

t_error		timer_modify(t_id		arg1,
			t_uint32		arg2,
			t_uint32		arg3);

t_error		timer_attribute_delay(t_id		arg1,
			t_uint32*		result1);

t_error		timer_attribute_repeat(t_id		arg1,
			t_uint32*		result1);

t_error		timer_attribute_type(t_id		arg1,
			t_type*		result1);

t_error		timer_attribute_handler(t_id		arg1,
			u_timer_handler*		result1);

t_error		timer_attribute_data(t_id		arg1,
			t_vaddr*		result1);


/*
 * ../thread_args.c
 */

t_error			thread_args(i_thread			threadid,
				    const void*			args,
				    t_vsize			size);


/*
 * ../thread_state_unlock.c
 */

t_error		thread_state_unlock(t_id		arg1,
				    t_state		arg2,
				    t_uint8*		lock);


/*
 * eop
 */

#endif
