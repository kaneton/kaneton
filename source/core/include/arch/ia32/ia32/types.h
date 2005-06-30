/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * types.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Jun 20 16:28:19 2005   mycure
 */

#ifndef IA32_IA32_TYPES_H
#define IA32_IA32_TYPES_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef unsigned char		t_uint8;
typedef signed char		t_sint8;
typedef unsigned short		t_uint16;
typedef signed short		t_sint16;
typedef unsigned int		t_uint32;
typedef signed int		t_sint32;
typedef unsigned long long	t_uint64;
typedef signed long long	t_sint64;

typedef t_uint8			t_reg8;
typedef t_uint16		t_reg16;
typedef t_uint32		t_reg32;
typedef t_uint64		t_reg64;

typedef t_uint32		t_paddr;
typedef t_uint32		t_psize;
typedef t_uint32		t_vaddr;
typedef t_uint32		t_vsize;

typedef t_uint32		t_size;
typedef t_uint32		t_offset;

typedef t_uint32		t_opts;
typedef t_uint32		t_type;

typedef t_uint64		t_id;
typedef t_id			t_setid;
typedef t_id			t_segid;
typedef t_id			t_asid;

#endif
