/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/time/reserve/02/02.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [sat nov 20 23:36:02 2010]
 */

#ifndef TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_RESERVE_02_02_H
#define TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_RESERVE_02_02_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"
#include "../../time.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      02.c
 */

/*
 * 02.c
 */

void			test_ibmpc_ia32_time_reserve_02_handler(t_id	id,
								t_vaddr	data);

void			test_ibmpc_ia32_time_reserve_02_content(void);

void			test_ibmpc_ia32_time_reserve_02(void);


/*
 * eop
 */

#endif
