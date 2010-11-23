/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/time/reserve/03/03.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2003]
 * updated       julien quintard   [sat nov 20 23:37:14 2010]
 */

#ifndef TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_RESERVE_03_03_H
#define TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_RESERVE_03_03_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"
#include "../../time.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      03.c
 */

/*
 * 03.c
 */

void			test_ibmpc_ia32_time_reserve_03_handler(t_id	id,
								t_vaddr	data);

void			test_ibmpc_ia32_time_reserve_03_content(void);

void			test_ibmpc_ia32_time_reserve_03(void);


/*
 * eop
 */

#endif
