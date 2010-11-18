/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/exception/06/06.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 16 22:36:27 2010]
 */

#ifndef TESTS_KANETON_EVENT_IA32_EXCEPTION_06_06_H
#define TESTS_KANETON_EVENT_IA32_EXCEPTION_06_06_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../engine/engine.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      06.c
 */

/*
 * 06.c
 */

void			test_event_ia32_exception_06_handler(t_id	id,
							     t_vaddr	address,
							     t_uint32	error);

void			test_event_ia32_exception_06(void);


/*
 * eop
 */

#endif
