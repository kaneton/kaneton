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
 * updated       julien quintard   [fri jan  7 20:01:28 2011]
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

#endif
