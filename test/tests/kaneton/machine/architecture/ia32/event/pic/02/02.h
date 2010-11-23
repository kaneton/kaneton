/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/event/pic/02/02.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [sun nov 21 00:14:26 2010]
 */

#ifndef TESTS_KANETON_MACHINE_ARCHITECTURE_IA32_EVENT_PIC_02_02_H
#define TESTS_KANETON_MACHINE_ARCHITECTURE_IA32_EVENT_PIC_02_02_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#undef MASTER_PORT_A
#undef MASTER_PORT_B
#undef SLAVE_PORT_A
#undef SLAVE_PORT_B

#define MASTER_PORT_A	0x20
#define MASTER_PORT_B	0x21
#define SLAVE_PORT_A	0xa0
#define SLAVE_PORT_B	0xa1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      02.c
 */

/*
 * 02.c
 */

void			test_ia32_event_pic_02_handler(t_id		id);

void			test_ia32_event_pic_02(void);


/*
 * eop
 */

#endif
