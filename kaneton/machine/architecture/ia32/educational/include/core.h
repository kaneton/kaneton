/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/include/core.h
 *
 * created       julien quintard   [wed jun  6 15:58:22 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:57:47 2007]
 */

#ifndef ARCHITECTURE_CORE_H
#define ARCHITECTURE_CORE_H		1


//XXX moveme!
/*
 * ---------- macros ----------------------------------------------------------
 */

#define PMODE_GDT_CORE_CS	0x1
#define PMODE_GDT_CORE_DS	0x2
#define PMODE_GDT_DRIVER_CS	0x3
#define PMODE_GDT_DRIVER_DS	0x4
#define PMODE_GDT_SERVICE_CS	0x5
#define PMODE_GDT_SERVICE_DS	0x6
#define PMODE_GDT_PROGRAM_CS	0x7
#define PMODE_GDT_PROGRAM_DS	0x8

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/stats.h>
#include <architecture/segment.h>
#include <architecture/region.h>
#include <architecture/as.h>
#include <architecture/event.h>
#include <architecture/timer.h>
#include <architecture/io.h>
#include <architecture/thread.h>
#include <architecture/task.h>
#include <architecture/cpu.h>
#include <architecture/sched.h>
#include <architecture/message.h>
#include <architecture/init.h>

#endif

