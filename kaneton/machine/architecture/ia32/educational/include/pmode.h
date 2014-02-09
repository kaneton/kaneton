/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tecture/ia32/educational/include/pmode.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [sat jan  8 19:15:41 2011]
 */

#ifndef ARCHITECTURE_PMODE_H
#define ARCHITECTURE_PMODE_H	1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../pmode.c
 */

/*
 * ../pmode.c
 */

t_status	architecture_pmode_enable(void);

t_status	architecture_pmode_setup(void);

t_status	architecture_pmode_registers(t_uint16	code,
					     t_uint16	data);


/*
 * eop
 */

#endif
