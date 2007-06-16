/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/platform/ibm-pc/include/event.h
 *
 * created       renaud voltz   [wed mar 15 10:49:45 2006]
 * updated       matthieu bucchianeri   [fri jun 15 01:30:33 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains constants about the interrupt controller on
 * ibmpc.
 */

#ifndef PLATFORM_EVENT_H
#define PLATFORM_EVENT_H	1

/*								[cut] k2 */

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pics addresses
 */

#define IBMPC_MASTER_PORT_A           0x20
#define IBMPC_MASTER_PORT_B           0x21
#define IBMPC_SLAVE_PORT_A            0xa0
#define IBMPC_SLAVE_PORT_B            0xa1

/*
 * master icw's
 */

#define IBMPC_MASTER_ICW1             0x11
#define IBMPC_MASTER_ICW2             0x20
#define IBMPC_MASTER_ICW3             0x04
#define IBMPC_MASTER_ICW4             0x01

/*
 * slave icw's
 */

#define IBMPC_SLAVE_ICW1              0x11
#define IBMPC_SLAVE_ICW2              0x28
#define IBMPC_SLAVE_ICW3              0x02
#define IBMPC_SLAVE_ICW4              0x01

/*								[cut] /k2 */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../event.c
 */

/*
 * ../event.c
 */

t_error			ibmpc_pic_init(void);

t_error			ibmpc_enable_irq(t_uint8		irq);

t_error			ibmpc_disable_irq(t_uint8		irq);

t_error			ibmpc_irq_acknowledge(t_uint8		irq);


/*
 * eop
 */

#endif
