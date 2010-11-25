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

#ifndef PLATFORM_IRQ_H
#define PLATFORM_IRQ_H	1


/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pics addresses
 */

#define PLATFORM_IRQ_MASTER_PORT_A	0x20
#define PLATFORM_IRQ_MASTER_PORT_B	0x21

#define PLATFORM_IRQ_SLAVE_PORT_A	0xa0
#define PLATFORM_IRQ_SLAVE_PORT_B	0xa1

/*
 * master icw's
 */

#define PLATFORM_IRQ_MASTER_ICW_1	0x11
#define PLATFORM_IRQ_MASTER_ICW_2	0x20
#define PLATFORM_IRQ_MASTER_ICW_3	0x04
#define PLATFORM_IRQ_MASTER_ICW_4	0x01

/*
 * slave icw's
 */

#define PLATFORM_IRQ_SLAVE_ICW_1	0x11
#define PLATFORM_IRQ_SLAVE_ICW_2	0x28
#define PLATFORM_IRQ_SLAVE_ICW_3	0x02
#define PLATFORM_IRQ_SLAVE_ICW_4	0x01


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../irq.c
 */

/*
 * ../irq.c
 */

t_error			platform_irq_enable(t_uint8		irq);

t_error			platform_irq_disable(t_uint8		irq);

t_error			platform_irq_acknowledge(t_uint8	irq);

t_error			platform_irq_initialize(void);

t_error			platform_irq_clean(void);


/*
 * eop
 */

#endif
