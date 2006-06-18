/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/core.h
 *
 * created       julien quintard   [sat dec 17 17:13:18 2005]
 * updated       matthieu bucchianeri   [sat jun 17 19:44:31 2006]
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
 */

/*
 * ../../../../core/arch/machdep/as.c
 */

t_error			ia32_as_show(i_as			asid);

t_error			ia32_as_reserve(i_task			tskid,
					i_as*			asid);


/*
 * ../../../../core/arch/machdep/region.c
 */

t_error			ia32_region_map_chunk(t_vaddr		v,
					      t_paddr		p);

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

void			ia32_event_handler(t_uint32		id);

void                    ia32_kbd_handler(t_uint32		id);

void                    ia32_pf_handler(t_uint32		error_code);


/*
 * ../../../../core/arch/machdep/timer.c
 */

t_error			ia32_timer_init(void);


/*
 * ../../../../core/arch/machdep/task.c
 */


/*
 * ../../../../core/arch/machdep/thread.c
 */

t_error			ia32_thread_suspend(t_thrid		threadid);

t_error			ia32_thread_execute(t_thrid		threadid);

t_error			ia32_thread_clone(t_thrid		threadid);


/*
 * ../../../../core/arch/machdep/sched.c
 */

t_error			ia32_sched_quantum(t_quantum		quantum);

t_error			ia32_sched_switch(i_thread		thread);

t_error			ia32_sched_init(void);

t_error			ia32_sched_clean(void);


/*
 * ../../../../core/arch/machdep/message.c
 */


/*
 * eop
 */

#endif
