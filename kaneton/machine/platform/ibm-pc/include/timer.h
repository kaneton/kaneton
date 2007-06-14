/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/platform/ibm-pc/include/timer.h
 *
 * created       renaud voltz   [wed mar 15 10:59:38 2006]
 * updated       matthieu bucchianeri   [fri jun 15 01:30:40 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains constants about the programmable timer on ibmpc.
 */

#ifndef PLATFORM_TIMER_H
#define PLATFORM_TIMER_H	1

/*								[cut] k2 */

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pit ports
 */

#define TIMER_0		0x40
#define TIMER_1		0x41
#define TIMER_2		0x42
#define PIT_8254_CTRL	0x43

/*
 * pit oscillator frequency
 */

#define CLOCK_TICK_RATE	1193180

/*								[cut] /k2 */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../timer.c
 */

/*
 * ../timer.c
 */

t_error			ibmpc_timer_init(void);


/*
 * eop
 */

#endif
