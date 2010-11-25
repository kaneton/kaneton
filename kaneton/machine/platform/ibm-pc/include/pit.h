/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/platform/ibm-pc/include/pit.h
 *
 * created       renaud voltz   [wed mar 15 10:59:38 2006]
 * updated       matthieu bucchianeri   [fri jun 15 01:30:40 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains constants about the programmable pit on ibmpc.
 */

#ifndef PLATFORM_PIT_H
#define PLATFORM_PIT_H	1


/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pit ports
 */

#define PLATFORM_PIT_0			0x40
#define PLATFORM_PIT_1			0x41
#define PLATFORM_PIT_2			0x42

#define PLATFORM_PIT_8254_CTRL		0x43

/*
 * pit oscillator frequency
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

t_error			platform_pit_initialize(void);

t_error			platform_pit_clean(void);


/*
 * eop
 */

#endif
