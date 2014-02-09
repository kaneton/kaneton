/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...on/machine/platform/ibm-pc/include/pit.h
 *
 * created       renaud voltz   [wed mar 15 10:59:38 2006]
 * updated       julien quintard   [fri jan 14 20:09:53 2011]
 */

#ifndef PLATFORM_PIT_H
#define PLATFORM_PIT_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * PIT ports
 */

#define PLATFORM_PIT_0			0x40
#define PLATFORM_PIT_1			0x41
#define PLATFORM_PIT_2			0x42

#define PLATFORM_PIT_8254_CTRL		0x43

/*
 * PIT oscillator frequency
 */

#define PLATFORM_PIT_RATE		1193180


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../pit.c
 */

/*
 * ../pit.c
 */

t_status		platform_pit_initialize(void);

t_status		platform_pit_clean(void);


/*
 * eop
 */

#endif
