/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...otloader/ibm-pc.ia32/educational/types.h
 *
 * created       matthieu bucchianeri   [tue jul 17 15:14:07 2007]
 * updated       matthieu bucchianeri   [tue jul 17 15:21:48 2007]
 */

#ifndef BOOTLOADER_TYPES_H
#define BOOTLOADER_TYPES_H

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ERROR_NONE		(1 << 0)
#define ERROR_UNKNOWN		(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * basic types
 */

typedef unsigned char		t_uint8;
typedef signed char		t_sint8;
typedef unsigned short		t_uint16;
typedef signed short		t_sint16;
typedef unsigned int		t_uint32;
typedef signed int		t_sint32;
typedef unsigned long long int	t_uint64;
typedef signed long long int	t_sint64;

/*
 * error type
 */

typedef t_sint32		t_error;
/*
 * memory types
 */

typedef t_uint32		t_paddr;
typedef t_uint32		t_psize;
typedef t_uint32		t_vaddr;
typedef t_uint32		t_vsize;

typedef t_uint32		t_size;
typedef t_uint32		t_offset;

#endif
