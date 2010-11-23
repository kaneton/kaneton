/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/exception/05/05.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [sun nov 21 00:03:56 2010]
 */

#ifndef TESTS_KANETON_MACHINE_ARCHITECTURE_IA32_EVENT_EXCEPTION_05_05_H
#define TESTS_KANETON_MACHINE_ARCHITECTURE_IA32_EVENT_EXCEPTION_05_05_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      05.c
 */

/*
 * 05.c
 */

void			test_ia32_event_exception_05_handler(t_id	id,
							     t_vaddr	address,
							     t_uint32	error);

void			test_ia32_event_exception_05(void);


/*
 * eop
 */

#endif
