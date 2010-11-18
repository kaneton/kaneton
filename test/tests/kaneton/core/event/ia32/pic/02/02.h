/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/event/ia32/pic/02/02.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 16 20:31:01 2010]
 */

#ifndef TESTS_KANETON_EVENT_IA32_PIC_02_02_H
#define TESTS_KANETON_EVENT_IA32_PIC_02_02_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../engine/engine.h"

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

void			test_event_ia32_pic_02_handler(t_id		id);

void			test_event_ia32_pic_02(void);


/*
 * eop
 */

#endif
