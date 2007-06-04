/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/core.h
 *
 * created       julien quintard   [sat dec 17 17:13:18 2005]
 * updated       matthieu bucchianeri   [sat jul 29 18:09:16 2006]
 */

#ifndef IA32_CORE_CORE_H
#define IA32_CORE_CORE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/core/as.h>
#include <arch/machdep/core/debug.h>
#include <arch/machdep/core/init.h>
#include <arch/machdep/core/region.h>
#include <arch/machdep/core/segment.h>
#include <arch/machdep/core/stats.h>
#include <arch/machdep/core/event.h>
#include <arch/machdep/core/timer.h>
#include <arch/machdep/core/thread.h>
#include <arch/machdep/core/task.h>
#include <arch/machdep/core/sched.h>
#include <arch/machdep/core/message.h>
#include <arch/machdep/core/cpu.h>
#include <arch/machdep/core/io.h>

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/id.h>
#include <core/task.h>
#include <core/thread.h>
#include <core/segment.h>
#include <core/region.h>
#include <core/as.h>
#include <core/event.h>
#include <core/timer.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../core/arch/machdep/as.c
 *      ../../../../core/arch/machdep/region.c
 *      ../../../../core/arch/machdep/segment.c
 *      ../../../../core/arch/machdep/event.c
 *	../../../../core/arch/machdep/timer.c
 *      ../../../../core/arch/machdep/task.c
 *      ../../../../core/arch/machdep/thread.c
 *      ../../../../core/arch/machdep/sched.c
 *      ../../../../core/arch/machdep/message.c
 *      ../../../../core/arch/machdep/cpu.c
 *      ../../../../core/arch/machdep/io.c
 */

/*
 * ../../../../core/arch/machdep/as.c
 */

t_error			ia32_as_give(i_task			tskid,
				     i_as			asid);

t_error			ia32_as_show(i_as			asid);

t_error			ia32_as_reserve(i_task			tskid,
					i_as*			asid);


/*
 * ../../../../core/arch/machdep/region.c
 */

t_error			ia32_region_map_chunk(t_vaddr		v,
					      t_paddr		p,
					      void*		alloc);

t_error			ia32_region_unmap_chunk(t_vaddr		v);

t_error			ia32_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid);

t_error			ia32_region_release(i_as		asid,
					    i_region		regid);

t_error			ia32_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new);

t_error			ia32_region_init(t_vaddr		start,
					 t_vsize		size);

t_error			ia32_region_clean(void);


/*
 * ../../../../core/arch/machdep/segment.c
 */

t_error			ia32_segment_read(i_region		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_error			ia32_segment_write(i_region		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_error			ia32_segment_copy(i_region		dst,
					  t_paddr		offsd,
					  i_region		src,
					  t_paddr		offss,
					  t_psize		sz);

t_error			ia32_segment_perms(i_segment		segid,
					   t_perms		perms);

t_error			ia32_segment_init(void);

t_error			ia32_segment_clean(void);


/*
 * ../../../../core/arch/machdep/event.c
 */

t_error			ia32_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler);

t_error			ia32_event_release(i_event		id);

t_error			ia32_event_init(void);

t_error			ia32_event_clean(void);

void			ia32_event_handler(t_id		id);

void                    ia32_kbd_handler(t_id			id);

void			ia32_gp_handler(t_id			id,
					t_uint32		error_code);

void			ia32_df_handler(t_id			id,
					t_uint32		error_code);

void			ia32_ts_handler(t_id			id,
					t_uint32		error_code);

void                    ia32_pf_handler(t_id			id,
					t_uint32		error_code);


/*
 * ../../../../core/arch/machdep/timer.c
 */

t_error			ia32_timer_init(void);


/*
 * ../../../../core/arch/machdep/task.c
 */

t_error			ia32_task_clone(i_task			old,
					i_task*			new);

t_error			ia32_task_reserve(t_class			class,
					  t_behav			behav,
					  t_prior			prior,
					  i_task*			id);

t_error			ia32_task_init(void);


/*
 * ../../../../core/arch/machdep/thread.c
 */

t_error			ia32_thread_clone(i_task		taskid,
					  i_thread		old,
					  i_thread*		new);

t_error			ia32_thread_reserve(i_task		taskid,
					    i_thread*		threadid);

t_error			ia32_thread_load(i_thread		threadid,
					 t_thread_context	context);

t_error			ia32_thread_store(i_thread		threadid,
					  t_thread_context*	context);

t_error			ia32_thread_stack(i_thread		threadid,
					  t_stack		stack);

t_error			ia32_thread_init(void);


/*
 * ../../../../core/arch/machdep/sched.c
 */

t_error			ia32_sched_quantum(t_quantum		quantum);

t_error			ia32_sched_yield(i_cpu			cpuid);

t_error			ia32_sched_switch(i_thread		elected);

void			ia32_sched_switch_mmx(t_id			id);

t_error			ia32_sched_init(void);

t_error			ia32_sched_clean(void);


/*
 * ../../../../core/arch/machdep/message.c
 */

void			ia32_message_async_send_handler(void);

void			ia32_message_sync_send_handler(void);

void			ia32_message_async_recv_handler(void);

void			ia32_message_sync_recv_handler(void);

t_error			ia32_message_epilogue(i_thread		thread,
					      t_error		exit_value);

t_error			ia32_message_init(void);

t_error			ia32_message_clean(void);


/*
 * ../../../../core/arch/machdep/cpu.c
 */

t_error			ia32_cpu_current(i_cpu*			cpuid);


/*
 * ../../../../core/arch/machdep/io.c
 */

t_error			ia32_io_grant(i_port			id,
				      i_task			task,
				      t_uint8			width);

t_error			ia32_io_deny(i_port			id,
				     i_task			task,
				     t_uint8			width);

t_error			ia32_io_read_8(i_task			task,
				       i_port			id,
				       t_uint8*			data);

t_error			ia32_io_read_16(i_task			task,
					i_port			id,
					t_uint16*		data);

t_error			ia32_io_read_32(i_task			task,
					i_port			id,
					t_uint32*		data);

t_error			ia32_io_read_64(i_task			task,
					i_port			id,
					t_uint64*		data);

t_error			ia32_io_write_8(i_task			task,
					i_port			id,
					t_uint8			data);

t_error			ia32_io_write_16(i_task			task,
					 i_port			id,
					 t_uint16		data);

t_error			ia32_io_write_32(i_task			task,
					 i_port			id,
					 t_uint32		data);

t_error			ia32_io_write_64(i_task			task,
					 i_port			id,
					 t_uint64		data);

t_error			ia32_io_init(void);


/*
 * eop
 */

#endif
