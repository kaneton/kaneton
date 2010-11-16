/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/event/ia32/context/01/01.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [sun nov 14 15:21:20 2010]
 */

#ifndef TESTS_KANETON_EVENT_IA32_CONTEXT_01_01_H
#define TESTS_KANETON_EVENT_IA32_CONTEXT_01_01_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../engine/engine.h"

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

void			test_event_ia32_context_01_handler(t_id		id);

void			test_event_ia32_context_01(void);


/*
 * eop
 */

#endif
