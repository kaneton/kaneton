/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chitecture/ia32/educational/include/io.h
 *
 * created       julien quintard   [fri jan  7 20:00:03 2011]
 * updated       julien quintard   [sun jan  9 22:46:17 2011]
 */

#ifndef ARCHITECTURE_IO_H
#define ARCHITECTURE_IO_H	1

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * these macro-functions alias the assembly macro-functions in a more
 * generic way such that platforms can use them for instance.
 */

#define ARCHITECTURE_IO_IN_8		ARCHITECTURE_INB
#define ARCHITECTURE_IO_IN_16		ARCHITECTURE_INW
#define ARCHITECTURE_IO_IN_32		ARCHITECTURE_INL
#define ARCHITECTURE_IO_OUT_8		ARCHITECTURE_OUTB
#define ARCHITECTURE_IO_OUT_16		ARCHITECTURE_OUTW
#define ARCHITECTURE_IO_OUT_32		ARCHITECTURE_OUTL

/*
 * this macro defines the number of bits in an I/O bitmap.
 */

#define ARCHITECTURE_IO_MAP_BITS	65536

/*
 * this macro defines the size of an I/O bitmap, in bytes.
 */

#define ARCHITECTURE_IO_MAP_SIZE	ARCHITECTURE_IO_MAP_BITS / 8

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../io.c
 */

/*
 * ../io.c
 */

t_status		architecture_io_clear(i_task		task);

t_status		architecture_io_set(t_uint8*		map,
					    t_uint32		port,
					    t_uint32		value);

t_status		architecture_io_get(t_uint8*		map,
					    t_uint32		port,
					    t_uint32*		value);

t_status		architecture_io_grant(i_task		task,
					      t_uint32		port,
					      t_uint8		width);

t_status		architecture_io_deny(i_task		task,
					     t_uint32		port,
					     t_uint8		width);

t_status		architecture_io_reset(void);


/*
 * eop
 */

#endif
