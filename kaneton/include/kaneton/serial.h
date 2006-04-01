/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/serial.h
 *
 * created       julien quintard   [fri feb 11 02:19:44 2005]
 * updated       julien quintard   [fri mar 10 03:40:49 2006]
 */

#ifndef	KANETON_SERIAL_H
#define	KANETON_SERIAL_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SERIAL_COM1		0x3f8
#define	SERIAL_COM2		0x2f8
#define SERIAL_COM3		0x3e8
#define SERIAL_COM4		0x2e8

#define	SERIAL_BR9600		0x0C
#define	SERIAL_BR19200		0x06
#define	SERIAL_BR38400		0x03
#define	SERIAL_BR57600		0x02
#define	SERIAL_BR115000		0x01

#define	SERIAL_8N1		0x03
#define	SERIAL_7N1		0x02
#define	SERIAL_8N2		0x07
#define	SERIAL_7N2		0x06

#define SERIAL_FIFO_14		0xC7
#define SERIAL_FIFO_8		0x87
#define	SERIAL_FIFO_4		0x47
#define SERIAL_FIFO_1		0x07

/*
 * ---------- types -----------------------------------------------------------
 */


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/debug/serial.c
 */

/*
 * ../../kaneton/debug/serial.c
 */

t_uint32		chk_sum(void				*data,
				unsigned int			size);

void			serial_read(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32 			size);

void			serial_write(t_uint32			com_port,
				     t_uint8*			data,
				     t_uint32			size);

int			serial_send(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32			size);

int			serial_recv(t_uint32			com_port,
				    t_serial_data		*rdata);

int	serial_put(char c);

void			serial_init(t_uint32			com_port,
				    t_uint8			baud_rate,
				    t_uint8			bit_type,
				    t_uint8			fifo_type);


/*
 * eop
 */

#endif
