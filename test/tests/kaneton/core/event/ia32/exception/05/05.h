/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/exception/05/05.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 16 22:31:15 2010]
 */

#ifndef TESTS_KANETON_EVENT_IA32_EXCEPTION_05_05_H
#define TESTS_KANETON_EVENT_IA32_EXCEPTION_05_05_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../engine/engine.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      05.c
 */

/*
 * 05.c
 */

void			test_event_ia32_exception_05_handler(t_id	id,
							     t_vaddr	address,
							     t_uint32	error);

void			test_event_ia32_exception_05(void);


/*
 * eop
 */

#endif
