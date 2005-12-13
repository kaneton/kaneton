/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Tue Nov 15 19:15:34 2005   mycure
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

#include <arch/ia32/kaneton/as.h>
#include <arch/ia32/kaneton/debug.h>
#include <arch/ia32/kaneton/init.h>
#include <arch/ia32/kaneton/segment.h>
#include <arch/ia32/kaneton/stats.h>
#include <arch/ia32/kaneton/task.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../kaneton/arch/ia32/kaneton/as.c
 *      ../../../../kaneton/arch/ia32/kaneton/segment.c
 *      ../../../../kaneton/arch/ia32/kaneton/task.c
 */

/*
 * ../../../../kaneton/arch/ia32/kaneton/as.c
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
 * ../../../../kaneton/arch/ia32/kaneton/segment.c
 */

t_error			ia32_segment_init(t_fit			fit);

t_error			ia32_segment_clean(void);


/*
 * ../../../../kaneton/arch/ia32/kaneton/task.c
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
