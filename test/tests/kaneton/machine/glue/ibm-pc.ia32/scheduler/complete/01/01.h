/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/ibm-pc.ia32/scheduler/complete/01/01.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [sun nov 21 10:13:15 2010]
 */

#ifndef TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_SCHEDULER_COMPLETE_01_01_H
#define TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_SCHEDULER_COMPLETE_01_01_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"
#include "../../scheduler.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      01.c
 */

/*
 * 01.c
 */

void			test_ibmpc_ia32_scheduler_complete_01_thread(t_id    id,
								     t_vaddr data);

void			test_ibmpc_ia32_scheduler_complete_01_content(void);

void			test_ibmpc_ia32_scheduler_complete_01(void);


/*
 * eop
 */

#endif
