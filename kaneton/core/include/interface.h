/*
 * ---------- information -----------------------------------------------------
 *
 * [XXX:improvements] the whole manager should be re-developed!
 */

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/id.h>
#include <core/types.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define INTERFACE_AS_COPY 3
#define INTERFACE_AS_RELEASE 6
#define INTERFACE_AS_ATTRIBUTE_TASK 7
#define INTERFACE_EVENT_RESERVE 8
#define INTERFACE_EVENT_RELEASE 9
#define INTERFACE_EVENT_ATTRIBUTE_TYPE 10
#define INTERFACE_EVENT_ATTRIBUTE_HANDLER 11
#define INTERFACE_EVENT_ATTRIBUTE_DATA 12
#define INTERFACE_MAP_RESERVE 23
#define INTERFACE_MAP_RELEASE 24
#define INTERFACE_MAP_RESIZE 25
#define INTERFACE_REGION_RESERVE 26
#define INTERFACE_REGION_RELEASE 27
#define INTERFACE_SCHEDULER_QUANTUM 28
#define INTERFACE_SCHEDULER_YIELD 29
#define INTERFACE_SCHEDULER_UPDATE 33
#define INTERFACE_SEGMENT_CLONE 34
#define INTERFACE_SEGMENT_GIVE 35
#define INTERFACE_SEGMENT_COPY 36
#define INTERFACE_SEGMENT_RELEASE 38
#define INTERFACE_SEGMENT_PERMISSIONS 40
#define INTERFACE_SEGMENT_ATTRIBUTE_AS 42
#define INTERFACE_SEGMENT_ATTRIBUTE_ADDRESS 44
#define INTERFACE_SEGMENT_ATTRIBUTE_SIZE 45
#define INTERFACE_SEGMENT_ATTRIBUTE_PERMISSIONS 46
#define INTERFACE_TASK_RESERVE 49
#define INTERFACE_TASK_RELEASE 50
#define INTERFACE_TASK_PRIORITY 51
#define INTERFACE_TASK_ATTRIBUTE_PARENT 54
#define INTERFACE_TASK_ATTRIBUTE_CLASS 55
#define INTERFACE_TASK_ATTRIBUTE_BEHAVIOUS 56
#define INTERFACE_TASK_ATTRIBUTE_PRIORITY 57
#define INTERFACE_TASK_ATTRIBUTE_AS 58
#define INTERFACE_TASK_ATTRIBUTE_SCHED 59
#define INTERFACE_THREAD_RELEASE 64
#define INTERFACE_THREAD_PRIORITY 65
#define INTERFACE_THREAD_LOAD 68
#define INTERFACE_THREAD_STORE 69
#define INTERFACE_THREAD_ATTRIBUTE_TASK 70
#define INTERFACE_THREAD_ATTRIBUTE_PRIORITY 71
#define INTERFACE_THREAD_ATTRIBUTE_STATE 72
#define INTERFACE_TIMER_RESERVE 76
#define INTERFACE_TIMER_RELEASE 77
#define INTERFACE_TIMER_ATTRIBUTE_DELAY 81
#define INTERFACE_TIMER_ATTRIBUTE_REPEAT 82
#define INTERFACE_TIMER_ATTRIBUTE_TYPE 83
#define INTERFACE_TIMER_ATTRIBUTE_HANDLER 84
#define INTERFACE_TIMER_ATTRIBUTE_DATA 85


#define INTERFACE_NSYSCALLS 86

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_id			id;
  union
  {
    struct
    {
      t_operations	operation;
      t_capability	capability;
      union
      {
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	  i_as	arg3;
	  t_vaddr	arg4;
	  t_vsize	arg5;
	}		as_copy;
	struct
	{
	  t_id	arg1;
	}		as_release;
	struct
	{
	  t_id	arg1;
	}		as_attribute_task;
	struct
	{
	  t_id	arg1;
	  t_type	arg2;
	  u_event_handler	arg3;
	  t_vaddr	arg4;
	}		event_reserve;
	struct
	{
	  t_id	arg1;
	}		event_release;
	struct
	{
	  t_id	arg1;
	}		event_attribute_type;
	struct
	{
	  t_id	arg1;
	}		event_attribute_handler;
	struct
	{
	  t_id	arg1;
	}		event_attribute_data;
	struct
	{
	  t_id	arg1;
	  i_task	arg2;
	  t_uint8	arg3;
	}		io_grant;
	struct
	{
	  t_id	arg1;
	  i_task	arg2;
	  t_uint8	arg3;
	}		io_deny;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_8;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_16;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_32;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	}		io_read_64;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint8	arg3;
	}		io_write_8;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint16	arg3;
	}		io_write_16;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint32	arg3;
	}		io_write_32;
	struct
	{
	  i_task	arg1;
	  t_id	arg2;
	  t_uint64	arg3;
	}		io_write_64;
	struct
	{
	  t_id	arg1;
	  t_options	arg2;
	  t_vsize	arg3;
	  t_permissions	arg4;
	}		map_reserve;
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	}		map_release;
	struct
	{
	  t_id	arg1;
	  t_vaddr	arg2;
	  t_vsize	arg3;
	}		map_resize;
	struct
	{
	  t_id	arg1;
	  i_segment	arg2;
	  t_paddr	arg3;
	  t_options	arg4;
	  t_vaddr	arg5;
	  t_vsize	arg6;
	}		region_reserve;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		region_release;
	struct
	{
	  t_quantum	arg1;
	}		scheduler_quantum;
	struct
	{
	  i_cpu	arg1;
	}		scheduler_yield;
	struct
	{
	  i_thread	arg1;
	}		scheduler_update;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		segment_clone;
	struct
	{
	  i_as	arg1;
	  t_id	arg2;
	}		segment_give;
	struct
	{
	  t_id	arg1;
	  t_paddr	arg2;
	  i_segment	arg3;
	  t_paddr	arg4;
	  t_psize	arg5;
	}		segment_copy;
	struct
	{
	  i_segment	arg1;
	}		segment_release;
	struct
	{
	  t_id	arg1;
	  t_permissions	arg2;
	}		segment_permissions;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_as;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_address;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_size;
	struct
	{
	  t_id	arg1;
	}		segment_attribute_permissions;
	struct
	{
	  t_class	arg1;
	  t_behaviour	arg2;
	  t_priority	arg3;
	}		task_reserve;
	struct
	{
	  t_id	arg1;
	}		task_release;
	struct
	{
	  t_id	arg1;
	  t_priority	arg2;
	}		task_priority;
	struct
	{
	  t_id	arg1;
	}		task_attribute_parent;
	struct
	{
	  t_id	arg1;
	}		task_attribute_class;
	struct
	{
	  t_id	arg1;
	}		task_attribute_behaviour;
	struct
	{
	  t_id	arg1;
	}		task_attribute_priority;
	struct
	{
	  t_id	arg1;
	}		task_attribute_as;
	struct
	{
	  t_id	arg1;
	}		task_attribute_sched;
	struct
	{
	  t_id	arg1;
	}		thread_release;
	struct
	{
	  t_id	arg1;
	  t_priority	arg2;
	}		thread_priority;
	struct
	{
	  t_id	arg1;
	  s_thread_context	arg2;
	}		thread_load;
	struct
	{
	  t_id	arg1;
	}		thread_store;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_task;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_priority;
	struct
	{
	  t_id	arg1;
	}		thread_attribute_state;
	struct
	{
	  t_type	arg1;
	  u_timer_handler	arg2;
	  t_vaddr	arg3;
	  t_uint32	arg4;
	  t_uint32	arg5;
	}		timer_reserve;
	struct
	{
	  t_id	arg1;
	}		timer_release;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_delay;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_repeat;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_type;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_handler;
	struct
	{
	  t_id	arg1;
	}		timer_attribute_data;
      }	u;
    } request;
    struct
    {
      t_status	error;
      union
      {
	struct
	{
	  i_task	result1;
	}		as_attribute_task;
	struct
	{
	  t_type	result1;
	}		event_attribute_type;
	struct
	{
	  u_event_handler	result1;
	}		event_attribute_handler;
	struct
	{
	  t_vaddr	result1;
	}		event_attribute_data;
	struct
	{
	  t_uint8	result1;
	}		io_read_8;
	struct
	{
	  t_uint16	result1;
	}		io_read_16;
	struct
	{
	  t_uint32	result1;
	}		io_read_32;
	struct
	{
	  t_uint64	result1;
	}		io_read_64;
	struct
	{
	  t_vaddr	result1;
	}		map_reserve;
	struct
	{
	  t_vaddr	result1;
	}		map_resize;
	struct
	{
	  i_region	result1;
	}		region_reserve;
	struct
	{
	  i_segment	result1;
	}		segment_clone;
	struct
	{
	  i_as	result1;
	}		segment_attribute_as;
	struct
	{
	  t_paddr	result1;
	}		segment_attribute_address;
	struct
	{
	  t_psize	result1;
	}		segment_attribute_size;
	struct
	{
	  t_permissions	result1;
	}		segment_attribute_permissions;
	struct
	{
	  i_task	result1;
	}		task_reserve;
	struct
	{
	  i_task	result1;
	}		task_attribute_parent;
	struct
	{
	  t_class	result1;
	}		task_attribute_class;
	struct
	{
	  t_behaviour	result1;
	}		task_attribute_behaviour;
	struct
	{
	  t_priority	result1;
	}		task_attribute_priority;
	struct
	{
	  i_as	result1;
	}		task_attribute_as;
	struct
	{
	  t_state	result1;
	}		task_attribute_sched;
	struct
	{
	  s_thread_context	result1;
	}		thread_store;
	struct
	{
	  i_task	result1;
	}		thread_attribute_task;
	struct
	{
	  t_priority	result1;
	}		thread_attribute_priority;
	struct
	{
	  t_state	result1;
	}		thread_attribute_state;
	struct
	{
	  i_timer	result1;
	}		timer_reserve;
	struct
	{
	  t_uint32	result1;
	}		timer_attribute_delay;
	struct
	{
	  t_uint32	result1;
	}		timer_attribute_repeat;
	struct
	{
	  t_type	result1;
	}		timer_attribute_type;
	struct
	{
	  u_timer_handler	result1;
	}		timer_attribute_handler;
	struct
	{
	  t_vaddr	result1;
	}		timer_attribute_data;
      } u;
    } reply;
  } u;
}			o_syscall;

typedef t_status (*t_interface_dispatch)(o_syscall*);


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/interface/interface.c
 */

/*
 * ../../core/interface/interface.c
 */

t_status	interface_as_copy(o_syscall*	message);

t_status	interface_as_release(o_syscall*	message);

t_status	interface_as_attribute_task(o_syscall*	message);

t_status	interface_event_reserve(o_syscall*	message);

t_status	interface_event_release(o_syscall*	message);

t_status	interface_event_attribute_type(o_syscall*	message);

t_status	interface_event_attribute_handler(o_syscall*	message);

t_status	interface_event_attribute_data(o_syscall*	message);

t_status	interface_map_reserve(o_syscall*	message);

t_status	interface_map_release(o_syscall*	message);

t_status	interface_map_resize(o_syscall*	message);

t_status	interface_region_reserve(o_syscall*	message);

t_status	interface_region_release(o_syscall*	message);

t_status	interface_scheduler_quantum(o_syscall*	message);

t_status	interface_scheduler_yield(o_syscall*	message);

t_status	interface_scheduler_update(o_syscall*	message);

t_status	interface_segment_clone(o_syscall*	message);

t_status	interface_segment_give(o_syscall*	message);

t_status	interface_segment_copy(o_syscall*	message);

t_status	interface_segment_release(o_syscall*	message);

t_status	interface_segment_permissions(o_syscall*	message);

t_status	interface_segment_attribute_as(o_syscall*	message);

t_status	interface_segment_attribute_address(o_syscall*	message);

t_status	interface_segment_attribute_size(o_syscall*	message);

t_status	interface_segment_attribute_permissions(o_syscall*	message);

t_status	interface_task_reserve(o_syscall*	message);

t_status	interface_task_release(o_syscall*	message);

t_status	interface_task_priority(o_syscall*	message);

t_status	interface_task_attribute_parent(o_syscall*	message);

t_status	interface_task_attribute_class(o_syscall*	message);

t_status	interface_task_attribute_behaviour(o_syscall*	message);

t_status	interface_task_attribute_priority(o_syscall*	message);

t_status	interface_task_attribute_as(o_syscall*	message);

t_status	interface_task_attribute_sched(o_syscall*	message);

t_status	interface_thread_release(o_syscall*	message);

t_status	interface_thread_priority(o_syscall*	message);

t_status	interface_thread_load(o_syscall*	message);

t_status	interface_thread_store(o_syscall*	message);

t_status	interface_thread_attribute_task(o_syscall*	message);

t_status	interface_thread_attribute_priority(o_syscall*	message);

t_status	interface_thread_attribute_state(o_syscall*	message);

t_status	interface_timer_reserve(o_syscall*	message);

t_status	interface_timer_release(o_syscall*	message);

t_status	interface_timer_attribute_delay(o_syscall*	message);

t_status	interface_timer_attribute_repeat(o_syscall*	message);

t_status	interface_timer_attribute_type(o_syscall*	message);

t_status	interface_timer_attribute_handler(o_syscall*	message);

t_status	interface_timer_attribute_data(o_syscall*	message);

t_status		interface_notify(t_uint8*		buffer,
					 t_vsize		size,
					 i_node			source);

t_status		interface_initialize(void);

t_status		interface_clean(void);


/*
 * eop
 */

#endif
