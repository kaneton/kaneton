/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/include/interrupt/pic.h
 *
 * created       renaud voltz   [wed mar 15 10:49:45 2006]
 * updated       renaud voltz   [wed mar 15 10:49:45 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 *
 */

#ifndef IA32_INTERRUPT_PIC_H
#define IA32_INTERRUPT_PIC_H

/*								[cut] k3 */

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pics addresses
 */

#define MASTER_PORT_A           0x20
#define MASTER_PORT_B           0x21
#define SLAVE_PORT_A            0xa0
#define SLAVE_PORT_B            0xa1

/*
 * master icw's
 */

#define MASTER_ICW1             0x11
#define MASTER_ICW2             0x20
#define MASTER_ICW3             0x04
#define MASTER_ICW4             0x01

/*
 * slave icw's
 */

#define SLAVE_ICW1              0x11
#define SLAVE_ICW2              0x28
#define SLAVE_ICW3              0x02
#define SLAVE_ICW4              0x01

/*								[cut] /k3 */

#endif
