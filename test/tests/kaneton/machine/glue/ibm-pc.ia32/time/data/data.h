/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/glue/ibm-pc.ia32/time/data/data.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [sat nov 20 23:18:30 2010]
 */

#ifndef TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_DATA_DATA_H
#define TESTS_KANETON_MACHINE_GLUE_IBMPC_IA32_TIME_DATA_DATA_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../engine/engine.h"
#include "../time.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      data.c
 */

/*
 * data.c
 */

void			test_ibmpc_ia32_time_data_handler(t_id		id,
							t_vaddr		data);

void			test_ibmpc_ia32_time_data_content(void);

void			test_ibmpc_ia32_time_data(void);


/*
 * eop
 */

#endif
