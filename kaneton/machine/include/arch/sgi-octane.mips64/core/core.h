/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-o2.mips64/core/core.h
 *
 * created       enguerrand raymond   [wed oct 18 14:15:13 2006]
 * updated       enguerrand raymond   [sat mar 10 11:05:25 2007]
 */

#ifndef MIPS64_CORE_CORE_H
#define MIPS64_CORE_CORE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ___endian		ENDIAN_BIG
#define ___wordsz		WORDSZ_64

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


/*
 * ../../../../core/arch/machdep/region.c
 */


/*
 * ../../../../core/arch/machdep/segment.c
 */


/*
 * ../../../../core/arch/machdep/event.c
 */

void	mipsr10k_event_handler(void);

t_error			mipsr10k_event_init(void);


/*
 * ../../../../core/arch/machdep/timer.c
 */


/*
 * ../../../../core/arch/machdep/task.c
 */


/*
 * ../../../../core/arch/machdep/thread.c
 */


/*
 * ../../../../core/arch/machdep/sched.c
 */


/*
 * ../../../../core/arch/machdep/message.c
 */


/*
 * ../../../../core/arch/machdep/cpu.c
 */


/*
 * ../../../../core/arch/machdep/io.c
 */

int	io_cons_print_char(char	c);

void	io_cons_attr(t_uint8	attr);

int	io_cons_init(void);


/*
 * eop
 */

#endif
