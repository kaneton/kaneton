/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...machine/platform/ibm-pc/include/serial.h
 *
 * created       julien quintard   [wed jun  6 14:06:42 2007]
 * updated       julien quintard   [thu dec  9 15:26:34 2010]
 */

#ifndef	PLATFORM_SERIAL_H
#define	PLATFORM_SERIAL_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * these macro provide the addresses and options related to the serial
 * device.
 */

#define PLATFORM_SERIAL_PRIMARY		0x3f8
#define	PLATFORM_SERIAL_SECONDARY	0x2f8
#define PLATFORM_SERIAL_COM_3		0x3e8
#define PLATFORM_SERIAL_COM_4		0x2e8

#define	PLATFORM_SERIAL_BR9600		0x0C
#define	PLATFORM_SERIAL_BR19200		0x06
#define	PLATFORM_SERIAL_BR38400		0x03
#define	PLATFORM_SERIAL_BR57600		0x02
#define	PLATFORM_SERIAL_BR115200	0x01

#define	PLATFORM_SERIAL_8N1		0x03
#define	PLATFORM_SERIAL_7N1		0x02
#define	PLATFORM_SERIAL_8N2		0x07
#define	PLATFORM_SERIAL_7N2		0x06

#define PLATFORM_SERIAL_FIFO_14		0xC7
#define PLATFORM_SERIAL_FIFO_8		0x87
#define	PLATFORM_SERIAL_FIFO_4		0x47
#define PLATFORM_SERIAL_FIFO_1		0x07

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../serial.c
 */

/*
 * ../serial.c
 */

void        		platform_serial_read(t_uint32		port,
					     t_uint8*		data,
					     t_uint32 		size);

void			platform_serial_write(t_uint32		port,
					      t_uint8*		data,
					      t_uint32		size);

t_status		platform_serial_setup(t_uint32		port,
					      t_uint8		rate,
					      t_uint8		type);

t_status		platform_serial_initialize(void);

t_status		platform_serial_clean(void);


/*
 * eop
 */

#endif
