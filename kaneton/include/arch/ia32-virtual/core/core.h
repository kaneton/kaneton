/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/core.h
 *
 * created       julien quintard   [sat dec 17 17:13:18 2005]
 * updated       julien quintard   [sat apr  1 23:00:02 2006]
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
 */

/*
 * ../../../../core/arch/machdep/as.c
 */

t_error			ia32_as_show(t_asid			asid);

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid);


/*
 * ../../../../core/arch/machdep/region.c
 */

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    t_regid*		regid);

t_error			ia32_region_release(t_asid		asid,
					    t_regid		regid);

t_error			ia32_region_init(t_vaddr		start,
					 t_vsize		size);

t_error			ia32_region_clean(void);


/*
 * ../../../../core/arch/machdep/segment.c
 */

t_error			ia32_segment_read(t_segid		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_error			ia32_segment_write(t_segid		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_error			ia32_segment_copy(t_segid		dst,
					  t_paddr		offsd,
					  t_segid		src,
					  t_paddr		offss,
					  t_psize		sz);

t_error			ia32_segment_init(void);

t_error			ia32_segment_clean(void);


/*
 * ../../../../core/arch/machdep/event.c
 */

t_error			ia32_event_reserve(t_eventid		eventid,
					   e_event_type		type,
					   u_event_handler	handler);

t_error			ia32_event_release(t_eventid		eventid);

t_error			ia32_event_init(void);

t_error			ia32_event_clean(void);

void			ia32_event_handler(t_uint32		id);

void                    ia32_kbd_handler(t_uint32                    id);

void                    ia32_pf_handler(t_uint32                     error_code);


/*
 * ../../../../core/arch/machdep/timer.c
 */

t_error			ia32_timer_init(void);


/*
 * ../../../../core/arch/machdep/task.c
 */

t_error			ia32_task_clone(t_tskid			old,
					t_tskid*		new);

t_error			ia32_task_reserve(t_class		class,
					  t_behav		behav,
					  t_prior		prior,
					  t_tskid*		tskid);

t_error			ia32_task_release(t_tskid		tskid);

t_error			ia32_task_init(void);

t_error			ia32_task_clean(void);


/*
 * ../../../../core/arch/machdep/thread.c
 */

t_error			ia32_thread_suspend(t_thrid		threadid);

t_error			ia32_thread_execute(t_thrid		threadid);

t_error			ia32_thread_clone(t_thrid		threadid);


/*
 * eop
 */

#endif
