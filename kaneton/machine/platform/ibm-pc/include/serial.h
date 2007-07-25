/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...machine/platform/ibm-pc/include/serial.h
 *
 * created       julien quintard   [wed jun  6 14:06:42 2007]
 * updated       matthieu bucchianeri   [sun jun 17 17:38:12 2007]
 */

#ifndef	PLATFORM_SERIAL_H
#define	PLATFORM_SERIAL_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/debug.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SERIAL_PRIMARY		0x3f8
#define	SERIAL_SECONDARY	0x2f8
#define IBMPC_SERIAL_COM3	0x3e8
#define IBMPC_SERIAL_COM4	0x2e8

#define	SERIAL_BR9600		0x0C
#define	SERIAL_BR19200		0x06
#define	SERIAL_BR38400		0x03
#define	SERIAL_BR57600		0x02
#define	SERIAL_BR115200		0x01

#define	SERIAL_8N1		0x03
#define	SERIAL_7N1		0x02
#define	SERIAL_8N2		0x07
#define	SERIAL_7N2		0x06

#define IBMPC_SERIAL_FIFO_14	0xC7
#define IBMPC_SERIAL_FIFO_8	0x87
#define	IBMPC_SERIAL_FIFO_4	0x47
#define IBMPC_SERIAL_FIFO_1	0x07

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../serial.c
 */

/*
 * ../serial.c
 */

void			ibmpc_serial_read(t_uint32		com_port,
					  t_uint8*		data,
					  t_uint32 		size);

void			ibmpc_serial_write(t_uint32		com_port,
					   t_uint8*		data,
					   t_uint32		size);

int			serial_send(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32			size);

int			serial_recv(t_uint32			com_port,
				    t_serial_data		*rdata);

int			serial_put(char				c);

t_error			serial_init(t_uint32			com_port,
				    t_uint8			baud_rate,
				    t_uint8			bit_type);


/*
 * eop
 */

#endif
