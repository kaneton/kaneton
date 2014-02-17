/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...n/tool/firmware/qemu-mips.mips64/types.h
 *
 * created       matthieu bucchianeri   [tue jul 17 15:14:07 2007]
 * updated       enguerrand raymond   [wed may 13 08:43:08 2009]
 */

#ifndef BOOTLOADER_TYPES_H
#define BOOTLOADER_TYPES_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define STATUS_OK		(1 << 0)
#define STATUS_UNKNOWN_ERROR		(1 << 1)

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
typedef unsigned long		t_uint64;
typedef signed long		t_sint64;

typedef t_uint64		t_id;

/*
 * error type
 */

typedef t_sint32		t_status;

/*
 * memory types
 */

typedef t_uint64		t_paddr;
typedef t_uint64		t_psize;
typedef t_uint64		t_vaddr;
typedef t_uint64		t_vsize;

typedef t_uint64		t_size;
typedef t_uint32		t_offset;

#endif
