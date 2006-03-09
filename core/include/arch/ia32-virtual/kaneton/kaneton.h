/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/include/arch/ia32-virtual/kaneton/kaneton.h
 *
 * created       julien quintard   [sat dec 17 17:13:18 2005]
 * updated       matthieu bucchianeri   [sat feb 18 18:51:11 2006]
 */

#ifndef IA32_KANETON_KANETON_H
#define IA32_KANETON_KANETON_H	1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/kaneton/as.h>
#include <arch/machdep/kaneton/debug.h>
#include <arch/machdep/kaneton/init.h>
#include <arch/machdep/kaneton/region.h>
#include <arch/machdep/kaneton/segment.h>
#include <arch/machdep/kaneton/stats.h>
#include <arch/machdep/kaneton/event.h>
#include <arch/machdep/kaneton/timer.h>
#include <arch/machdep/kaneton/task.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../kaneton/arch/machdep/as.c
 *      ../../../../kaneton/arch/machdep/region.c
 *      ../../../../kaneton/arch/machdep/segment.c
 *      ../../../../kaneton/arch/machdep/event.c
 *	../../../../kaneton/arch/machdep/timer.c
 *      ../../../../kaneton/arch/machdep/task.c
 */

/*
 * ../../../../kaneton/arch/machdep/as.c
 */

t_error			ia32_as_show(t_asid			asid);

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid);

t_error			ia32_as_release(t_asid			asid);


/*
 * ../../../../kaneton/arch/machdep/region.c
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

t_error			ia32_region_init(t_fit			fit,
					 t_vaddr		start,
					 t_vsize		size);

t_error			ia32_region_clean(void);


/*
 * ../../../../kaneton/arch/machdep/segment.c
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

t_error			ia32_segment_init(t_fit			fit);

t_error			ia32_segment_clean(void);


/*
 * ../../../../kaneton/arch/machdep/event.c
 */

t_error			ia32_event_reserve(t_eventid		eventid,
					   e_event_type		type,
					   u_event_handler	handler);

t_error			ia32_event_release(t_eventid		eventid);

t_error			ia32_event_init(void);

t_error			ia32_event_clean(void);

void			ia32_event_handler(t_uint32		id);


/*
 * ../../../../kaneton/arch/machdep/timer.c
 */

t_error			ia32_timer_init(void);

t_error			ia32_timer_clean(void);


/*
 * ../../../../kaneton/arch/machdep/task.c
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
 * eop
 */

#endif
