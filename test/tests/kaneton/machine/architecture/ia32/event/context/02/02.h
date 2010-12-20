/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../architecture/ia32/event/context/02/02.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [mon dec 20 21:40:09 2010]
 */

#ifndef TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_CONTEXT_02_02_H
#define TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_CONTEXT_02_02_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint32		gs;
  t_uint32		fs;
  t_uint32		es;
  t_uint32		ds;
  t_uint32		cs;
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
 *      02.c
 */

/*
 * 02.c
 */

void			test_architecture_event_context_02_handler(t_id	id);

void			test_architecture_event_context_02(void);


/*
 * eop
 */

#endif
