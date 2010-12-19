/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tecture/ia32/educational/include/types.h
 *
 * created       julien quintard   [wed jun  6 19:07:02 2007]
 * updated       julien quintard   [fri dec 17 22:22:25 2010]
 */

#ifndef ARCHITECTURE_TYPES_H
#define ARCHITECTURE_TYPES_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * fundamental types.
 */

typedef unsigned char		t_uint8;
typedef signed char		t_sint8;
typedef unsigned short		t_uint16;
typedef signed short		t_sint16;
typedef unsigned int		t_uint32;
typedef signed int		t_sint32;
typedef unsigned long long int	t_uint64;
typedef signed long long int	t_sint64;

typedef t_uint8			t_reg8;
typedef t_uint16		t_reg16;
typedef t_uint32		t_reg32;
typedef t_uint64		t_reg64;

/*
 * the physical address and size types.
 */

typedef t_uint32		t_paddr;
typedef t_uint32		t_psize;

/*
 * the virtual address and size types.
 */

typedef t_uint32		t_vaddr;
typedef t_uint32		t_vsize;

#endif
