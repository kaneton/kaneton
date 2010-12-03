/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...hine/architecture/ia32/event/pic/01/01.h
 *
 * created       julien quintard   [wed apr 15 05:15:32 2009]
 * updated       julien quintard   [tue nov 30 17:03:07 2010]
 */

#ifndef TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_PIC_01_01_H
#define TESTS_KANETON_MACHINE_ARCHITECTURE_EVENT_PIC_01_01_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "../../../../../../../../engine/engine.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define TEST_MASTER_PORT_A	0x20
#define TEST_MASTER_PORT_B	0x21
#define TEST_SLAVE_PORT_A	0xa0
#define TEST_SLAVE_PORT_B	0xa1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      01.c
 */

/*
 * 01.c
 */

void			test_architecture_event_pic_01_handler(t_id	id);

void			test_architecture_event_pic_01(void);


/*
 * eop
 */

#endif
