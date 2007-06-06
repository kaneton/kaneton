/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/core/core.h
 *
 * created       julien quintard   [wed jun  6 12:22:26 2007]
 * updated       julien quintard   [wed jun  6 16:41:39 2007]
 */

#ifndef GUARD_PUBLIC_CORE_CORE
#define GUARD_PUBLIC_CORE_CORE		1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * endian
 */

#define ENDIAN_LITTLE		(1 << 0)
#define ENDIAN_BIG		(1 << 1)

/*
 * word size
 */

#define WORDSZ_8		(1 << 0)
#define WORDSZ_16		(1 << 1)
#define WORDSZ_32		(1 << 0)
#define WORDSZ_64		(1 << 1)
#define WORDSZ_128		(1 << 2)

/*
 * perms
 */

#define PERM_READ		(1 << 0)
#define PERM_WRITE		(1 << 1)
#define PERM_EXEC		(1 << 2)

/*
 * fits
 */

#define FIT_FIRST		(1 << 0)
#define FIT_BEST		(1 << 1)
#define FIT_NEXT		(1 << 2)
#define FIT_WORST		(1 << 3)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * register types
 */

typedef t_uint8			t_reg8;
typedef t_uint16		t_reg16;
typedef t_uint32		t_reg32;
typedef t_uint64		t_reg64;

/*
 * memory types
 */

typedef t_uint32		t_paddr;
typedef t_uint32		t_psize;
typedef t_uint32		t_vaddr;
typedef t_uint32		t_vsize;

typedef t_uint32		t_size;
typedef t_uint32		t_offset;

/*
 * special types
 */

typedef t_uint32		t_opts;
typedef t_uint32		t_perms;
typedef t_uint32		t_type;
typedef t_uint32		t_lookup;
typedef t_uint32		t_state;
typedef t_uint32		t_status;
typedef t_uint32		t_class;
typedef t_uint32		t_behav;
typedef t_uint32		t_prior;
typedef t_uint32		t_quantum;
typedef t_uint32		t_operations;
typedef t_uint32		t_timeslice;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define core_error(_fmt_...)						\
  {									\
    printf("[!] %s:%u: ",						\
           __FILE__,							\
           __LINE__);							\
    printf(_fmt_);							\
									\
    while (1)								\
      ;									\
  }

#endif

/*
 * ---------- includes --------------------------------------------------------
 */

// XXX deprecated
// #include <core/conf.h>

// XXX
// #include <core/types.h>

#include <core/error.h>
#include <core/id.h>
#include <core/init.h>
#include <core/kernel.h>
#include <core/stats.h>
#include <core/set.h>
#include <core/as.h>
#include <core/segment.h>
#include <core/region.h>
#include <core/map.h>
#include <core/event.h>
#include <core/timer.h>
#include <core/thread.h>
#include <core/task.h>
#include <core/wait.h>
#include <core/sched.h>
#include <core/message.h>
#include <core/capability.h>
#include <core/cpu.h>
#include <core/io.h>
#include <core/mod.h>
#include <core/interface.h>

#include <core/debug.h>
/*                                                                [cut] k1   */
#include <core/serial.h>
/*                                                               [cut] /k1   */

#ifndef GUARD_PRIVATE_CORE_CORE
#define GUARD_PRIVATE_CORE_CORE		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/core.c
 */

/*
 * ../../core/core.c
 */

void			kaneton(t_init*				bootloader);

void			kaneton_dump(void);


/*
 * eop
 */

#endif
