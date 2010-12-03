/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...rchitecture/ia32/event/exception/05/05.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 30 16:48:40 2010]
 */

#ifndef TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_EXCEPTION_05_05_H
#define TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_EXCEPTION_05_05_H

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

void			test_architecture_event_exception_05_handler(t_id id,
								     t_vaddr address,
								     t_uint32 error);

void			test_architecture_event_exception_05(void);


/*
 * eop
 */

#endif
