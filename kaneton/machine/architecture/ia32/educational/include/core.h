/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../include/arch/ibm-pc.ia32-virtual/core.h
 *
 * created       julien quintard   [wed jun  6 15:58:22 2007]
 * updated       julien quintard   [fri jun  8 15:54:43 2007]
 */

#ifndef ARCHITECTURE_CORE_H
#define ARCHITECTURE_CORE_H		1

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

