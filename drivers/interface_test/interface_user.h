#ifndef INTERFACE_USER_H
# define INTERFACE_USER_H

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      interface_user.c
 */

/*
 * interface_user.c
 */

t_error		syscall_as_give(i_task		arg1,
			t_id		arg2);

t_error		syscall_as_vaddr(t_id		arg1,
			t_paddr		arg2,
			t_vaddr*		result1);

t_error		syscall_as_paddr(t_id		arg1,
			t_vaddr		arg2,
			t_paddr*		result1);

t_error		syscall_as_copy(t_id		arg1,
			t_vaddr		arg2,
			i_as		arg3,
			t_vaddr		arg4,
			t_vsize		arg5);

t_error		syscall_as_clone(i_task		arg1,
			t_id		arg2,
			i_as*		result1);

t_error		syscall_as_reserve(i_task		arg1,
			i_as*		result1);

t_error		syscall_as_release(t_id		arg1);

t_error		syscall_as_get(t_id		arg1,
			o_as*		result1);

t_error		syscall_capability_reserve(t_id		arg1,
			t_operations		arg2,
			t_capability*		result1);

t_error		syscall_capability_release(t_id		arg1);

t_error		syscall_capability_restrict(t_id		arg1,
			t_operations		arg2,
			t_capability*		result1);

t_error		syscall_capability_invalidate(t_id		arg1,
			t_id		arg2);

t_error		syscall_capability_get(t_id		arg1,
			t_capability_descriptor*		result1);

t_error		syscall_capability_give(t_id		arg1,
			i_node		arg2);

t_error		syscall_capability_verify(t_capability*		arg1);

t_error		syscall_event_reserve(t_id		arg1,
			t_type		arg2,
			u_event_handler		arg3);

t_error		syscall_event_release(t_id		arg1);

t_error		syscall_event_get(t_id		arg1,
			o_event*		result1);

t_error		syscall_io_grant(t_id		arg1,
			i_task		arg2,
			t_uint8		arg3);

t_error		syscall_io_deny(t_id		arg1,
			i_task		arg2,
			t_uint8		arg3);

t_error		syscall_io_read_8(i_task		arg1,
			t_id		arg2,
			t_uint8*		result1);

t_error		syscall_io_read_16(i_task		arg1,
			t_id		arg2,
			t_uint16*		result1);

t_error		syscall_io_read_32(i_task		arg1,
			t_id		arg2,
			t_uint32*		result1);

t_error		syscall_io_read_64(i_task		arg1,
			t_id		arg2,
			t_uint64*		result1);

t_error		syscall_io_write_8(i_task		arg1,
			t_id		arg2,
			t_uint8		arg3);

t_error		syscall_io_write_16(i_task		arg1,
			t_id		arg2,
			t_uint16		arg3);

t_error		syscall_io_write_32(i_task		arg1,
			t_id		arg2,
			t_uint32		arg3);

t_error		syscall_io_write_64(i_task		arg1,
			t_id		arg2,
			t_uint64		arg3);

t_error		syscall_map_reserve(t_id		arg1,
			t_opts		arg2,
			t_vsize		arg3,
			t_perms		arg4,
			t_vaddr*		result1);

t_error		syscall_map_release(t_id		arg1,
			t_vaddr		arg2);

t_error		syscall_map_resize(t_id		arg1,
			t_vaddr		arg2,
			t_vsize		arg3,
			t_vaddr*		result1);

t_error		syscall_message_register(i_task		arg1,
			t_tag		arg2);

t_error		syscall_region_reserve(t_id		arg1,
			i_segment		arg2,
			t_paddr		arg3,
			t_opts		arg4,
			t_vaddr		arg5,
			t_vsize		arg6,
			i_region*		result1);

t_error		syscall_region_release(i_as		arg1,
			t_id		arg2);

t_error		syscall_region_get(i_as		arg1,
			t_id		arg2,
			o_region*		result1);

t_error		syscall_sched_quantum(t_quantum		arg1);

t_error		syscall_sched_yield(i_cpu		arg1);

t_error		syscall_sched_current(i_thread*		result1);

t_error		syscall_sched_add(i_thread		arg1);

t_error		syscall_sched_remove(i_thread		arg1);

t_error		syscall_sched_update(i_thread		arg1);

t_error		syscall_segment_clone(i_as		arg1,
			t_id		arg2,
			i_segment*		result1);

t_error		syscall_segment_give(i_as		arg1,
			t_id		arg2);

t_error		syscall_segment_copy(t_id		arg1,
			t_paddr		arg2,
			i_segment		arg3,
			t_paddr		arg4,
			t_psize		arg5);

t_error		syscall_segment_reserve(i_as		arg1,
			t_psize		arg2,
			t_perms		arg3,
			i_segment*		result1);

t_error		syscall_segment_release(i_segment		arg1);

t_error		syscall_segment_catch(i_as		arg1,
			t_id		arg2);

t_error		syscall_segment_perms(t_id		arg1,
			t_perms		arg2);

t_error		syscall_segment_type(t_id		arg1,
			t_type		arg2);

t_error		syscall_segment_get(t_id		arg1,
			o_segment*		result1);

t_error		syscall_task_current(i_task*		result1);

t_error		syscall_task_clone(t_id		arg1,
			i_task*		result1);

t_error		syscall_task_reserve(t_class		arg1,
			t_behav		arg2,
			t_prior		arg3,
			i_task*		result1);

t_error		syscall_task_release(t_id		arg1);

t_error		syscall_task_priority(t_id		arg1,
			t_prior		arg2);

t_error		syscall_task_state(t_id		arg1,
			t_state		arg2);

t_error		syscall_task_wait(t_id		arg1,
			t_opts		arg2,
			t_wait*		result1);

t_error		syscall_task_get(t_id		arg1,
			o_task*		result1);

t_error		syscall_thread_give(i_task		arg1,
			t_id		arg2);

t_error		syscall_thread_clone(i_task		arg1,
			t_id		arg2,
			i_thread*		result1);

t_error		syscall_thread_reserve(i_task		arg1,
			t_prior		arg2,
			i_thread*		result1);

t_error		syscall_thread_release(t_id		arg1);

t_error		syscall_thread_priority(t_id		arg1,
			t_prior		arg2);

t_error		syscall_thread_state(t_id		arg1,
			t_state		arg2);

t_error		syscall_thread_stack(t_id		arg1,
			t_stack		arg2);

t_error		syscall_thread_load(t_id		arg1,
			t_thread_context		arg2);

t_error		syscall_thread_store(t_id		arg1,
			t_thread_context*		result1);

t_error		syscall_thread_get(t_id		arg1,
			o_thread*		result1);

t_error		syscall_timer_reserve(t_type		arg1,
			u_timer_handler		arg2,
			t_uint32		arg3,
			t_uint32		arg4,
			i_timer*		result1);

t_error		syscall_timer_release(t_id		arg1);

t_error		syscall_timer_delay(t_id		arg1,
			t_uint32		arg2);

t_error		syscall_timer_repeat(t_id		arg1,
			t_uint32		arg2);

t_error		syscall_timer_modify(t_id		arg1,
			t_uint32		arg2,
			t_uint32		arg3);

t_error		syscall_timer_get(t_id		arg1,
			o_timer*		result1);


/*
 * eop
 */

#endif
