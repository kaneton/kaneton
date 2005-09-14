/*
 * copyright quintard julien
 *
 * kaneton
 *
 * kaneton.h
 *
 * path          /home/mycure/kaneton/core/include/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Wed Sep 14 12:36:34 2005   mycure
 */

#ifndef	KANETON_SERIAL_H
#define	KANETON_SERIAL_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define SERIAL_COM1		0x3f8
#define	SERIAL_COM2	0x2f8
#define COM3		0x3e8
#define COM4		0x2e8

#define	BR9600		0x0C
#define	BR19200		0x06
#define	BR38400		0x03
#define	BR57600		0x02
#define	BR115000	0x01

#define	_8N1		0x03
#define	_7N1		0x02
#define	_8N2		0x07
#define	_7N2		0x06

#define 	FIFO_14	0xC7
#define 	FIFO_8	0x87
#define		FIFO_4	0x47
#define 	FIFO_1	0x07

typedef struct
{
  t_uint32			size;
	t_uint32	magic;
	t_uint32	crc;
	t_uint8*	data;
}				t_serial_data;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/debug/serial.c
 */

/*
 * eop
 */

#endif
