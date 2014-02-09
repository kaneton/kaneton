/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...on/machine/platform/ibm-pc/include/pic.h
 *
 * created       renaud voltz   [wed mar 15 10:49:45 2006]
 * updated       julien quintard   [fri jan 14 20:09:41 2011]
 */

#ifndef PLATFORM_PIC_H
#define PLATFORM_PIC_H	1

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pics addresses
 */

#define PLATFORM_PIC_MASTER_PORT_A	0x20
#define PLATFORM_PIC_MASTER_PORT_B	0x21

#define PLATFORM_PIC_SLAVE_PORT_A	0xa0
#define PLATFORM_PIC_SLAVE_PORT_B	0xa1

/*
 * master icw's
 */

#define PLATFORM_PIC_MASTER_ICW_1	0x11
#define PLATFORM_PIC_MASTER_ICW_2	0x20
#define PLATFORM_PIC_MASTER_ICW_3	0x04
#define PLATFORM_PIC_MASTER_ICW_4	0x01

/*
 * slave icw's
 */

#define PLATFORM_PIC_SLAVE_ICW_1	0x11
#define PLATFORM_PIC_SLAVE_ICW_2	0x28
#define PLATFORM_PIC_SLAVE_ICW_3	0x02
#define PLATFORM_PIC_SLAVE_ICW_4	0x01


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../pic.c
 */

/*
 * ../pic.c
 */

t_status		platform_pic_enable(t_uint8		irq);

t_status		platform_pic_disable(t_uint8		irq);

t_status		platform_pic_acknowledge(t_uint8	irq);

t_status		platform_pic_initialize(void);

t_status		platform_pic_clean(void);


/*
 * eop
 */

#endif
