/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../architecture/ia32/event/context/01/01.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 30 16:41:33 2010]
 */

#ifndef TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_CONTEXT_01_01_H
#define TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_CONTEXT_01_01_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint32		ebp;
  t_uint32		edi;
  t_uint32		esi;
  t_uint32		edx;
  t_uint32		ecx;
  t_uint32		ebx;
  t_uint32		eax;
} __attribute__((packed)) t_test_ctx;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      01.c
 */

/*
 * 01.c
 */

void			test_architecture_event_context_01_handler(t_id	id);

void			test_architecture_event_context_01(void);


/*
 * eop
 */

#endif
