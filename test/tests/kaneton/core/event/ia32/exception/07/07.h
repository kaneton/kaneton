/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/exception/07/07.h
 *
 * created       julien quintard   [wed apr 15 07:15:32 2009]
 * updated       julien quintard   [tue nov 16 22:41:05 2010]
 */

#ifndef TESTS_KANETON_EVENT_IA32_EXCEPTION_07_07_H
#define TESTS_KANETON_EVENT_IA32_EXCEPTION_07_07_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../engine/engine.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      07.c
 */

/*
 * 07.c
 */

void			test_event_ia32_exception_07_handler(t_id	id,
							     t_vaddr	address,
							     t_uint32	error);

void			test_event_ia32_exception_07(void);


/*
 * eop
 */

#endif
