/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32-virtual/kaneton/kaneton.h
 *
 * created       julien quintard   [sat dec 17 17:13:18 2005]
 * updated       julien quintard   [sun dec 18 18:55:21 2005]
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
#include <arch/machdep/kaneton/task.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../kaneton/arch/machdep/kaneton/as.c
 *      ../../../../kaneton/arch/machdep/kaneton/region.c
 *      ../../../../kaneton/arch/machdep/kaneton/segment.c
 *      ../../../../kaneton/arch/machdep/kaneton/task.c
 */

/*
 * ../../../../kaneton/arch/machdep/kaneton/as.c
 */

t_error			ia32_as_give(t_asid			asid,
				     t_tskid			tskid);

t_error			ia32_as_clone(t_tskid			tskid,
				      t_asid			old,
				      t_asid*			new);

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid);

t_error			ia32_as_release(t_asid			asid);

t_error			ia32_as_init(void);

t_error			ia32_as_clean(void);


/*
 * ../../../../kaneton/arch/machdep/kaneton/region.c
 */

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_opts		opts,
					    t_vaddr		address,
					    t_regid*		regid);

t_error			ia32_region_init(t_fit			fit,
					 t_vaddr		start,
					 t_vsize		size);

t_error			ia32_region_clean(void);


/*
 * ../../../../kaneton/arch/machdep/kaneton/segment.c
 */

t_error			ia32_segment_init(t_fit			fit);

t_error			ia32_segment_clean(void);

t_error			ia32_segment_reserve(t_asid		asid,
					     t_psize		size,
					     t_perms		perms,
					     t_segid*		segid);

t_error			ia32_segment_release(t_segid			segid);


/*
 * ../../../../kaneton/arch/machdep/kaneton/task.c
 */

t_error			ia32_task_clone(t_tskid		old,
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
