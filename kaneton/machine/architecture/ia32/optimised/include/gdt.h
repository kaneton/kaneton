/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/pmode/gdt.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:48:11 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * protected mode structures and macros.
 *
 */

#ifndef ARCHITECTURE_GDT_H
#define ARCHITECTURE_GDT_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * misc
 */

#define IA32_GDT_MAX_ENTRIES	8191
#define IA32_LDT_MAX_ENTRIES	8191
#define IA32_GDT_CURRENT	NULL

/*
 * gdt flags
 */

#define IA32_GDT_FLAG_GRANULAR	(1 << 3)
#define IA32_GDT_FLAG_USE32	(1 << 2)
#define IA32_GDT_FLAG_USE16	(0 << 2)
#define IA32_GDT_FLAG_AVAILABLE	(1 << 0)

/*
 * system segment types
 */

#define IA32_SEG_TYPE_CODE	((1 << 3) | (1 << 1))
#define IA32_SEG_TYPE_DATA	(1 << 1)
#define IA32_SEG_TYPE_LDT	(1 << 1)
#define IA32_SEG_TYPE_TSS	((1 << 3) | (1 << 0))
#define IA32_SEG_GATE_CALL	((1 << 3) | (1 << 2))
#define IA32_SEG_GATE_TRAP	((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define IA32_SEG_GATE_INTERRUPT	((1 << 3) | (1 << 2) | (1 << 1))

/*
 * gdt type
 */

#define IA32_GDT_TYPE_SYS(Type)						\
  ((Type) & ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))

#define IA32_GDT_TYPE_SEG(Type)						\
  ((Type) & ((1 << 3) | (1 << 1)))

#define IA32_GDT_TYPE_S		(1 << 4)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <architecture/pmode.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * user segments types
 */
typedef enum
  {
    ia32_type_code	= IA32_SEG_TYPE_CODE,
    ia32_type_data	= IA32_SEG_TYPE_DATA,
  } t_ia32_usr_segtype;

/*
 * system segments types
 */
typedef enum
  {
    ia32_type_ldt	= IA32_SEG_TYPE_LDT,
    ia32_type_tss	= IA32_SEG_TYPE_TSS,
    ia32_type_call_gate	= IA32_SEG_GATE_CALL,
    ia32_type_trap_gate	= IA32_SEG_GATE_TRAP,
    ia32_type_int_gate	= IA32_SEG_GATE_INTERRUPT,
  } t_ia32_sys_segtype;

typedef union
{
  t_ia32_usr_segtype	usr;
  t_ia32_sys_segtype	sys;
}			u_ia32_segtype;

/*
 * abstract segment descriptor
 */

typedef struct
{
  t_uint32		base;
  t_uint32		limit;
  t_ia32_prvl		privilege;
  t_uint8		is_system;
  u_ia32_segtype	type;
}			t_ia32_segment;

/*
 * gdt entry
 */

typedef struct
{
  t_uint16		limit_00_15;
  t_uint16		base_00_15;
  t_uint8		base_16_23;
  t_uint8		type;
  t_uint8		limit_16_19 : 4;
  t_uint8		flags : 4;
  t_uint8		base_24_31;
}			__attribute__ ((packed)) t_ia32_gdte;

/*
 * gdt
 */

typedef struct
{
  t_ia32_gdte		*descriptor;
  t_uint16		count;
}			t_ia32_gdt;

/*
 * gdt register
 */

typedef struct
{
  t_uint16		size;
  t_uint32		address;
}			__attribute__ ((packed)) t_ia32_gdtr;

/*
 * ldt entry
 */

typedef struct
{
  t_uint16		limit_00_15;
  t_uint16		base_00_15;
  t_uint8		base_16_23;
  t_uint8		type;
  t_uint8		limit_16_19 : 4;
  t_uint8		flags : 4;
  t_uint8		base_24_31;
}			__attribute__ ((packed)) t_ia32_ldte;

/*
 * ldt
 */

typedef struct
{
  t_ia32_ldte		*descriptor;
  t_uint16		count;
  t_uint16		gdt_entry;
}			t_ia32_ldt;

/*
 * ldt register
 */

typedef struct
{
  t_uint16		selector;
  t_uint16		size;
  t_uint32		address;
}			__attribute__ ((packed)) t_ia32_ldtr;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../gdt.c
 *	../ldt.c
 */

/*
 * ../gdt.c
 */

t_status		ia32_gdt_dump(t_ia32_gdt*		dump_gdt);

t_status		ia32_gdt_size(t_ia32_gdt*		table,
				      t_uint16*			size);

t_status		ia32_gdt_build(t_uint16			entries,
				       t_paddr			base,
				       t_ia32_gdt*		gdt,
				       t_uint8			clear);

t_status		ia32_gdt_activate(t_ia32_gdt		new_gdt);

t_status		ia32_gdt_import(t_ia32_gdt*		gdt);

t_status		ia32_gdt_add_segment(t_ia32_gdt*	table,
					     t_uint16		segment,
					     t_ia32_segment	descriptor);

t_status		ia32_gdt_reserve_segment(t_ia32_gdt*	table,
						 t_ia32_segment	descriptor,
						 t_uint16*	segment);

t_status		ia32_gdt_get_segment(t_ia32_gdt*	table,
					     t_uint16		index,
					     t_ia32_segment*	segment);

t_status		ia32_gdt_delete_segment(t_ia32_gdt*	table,
						t_uint16	segment);

t_status		ia32_gdt_build_selector(t_uint16	segment,
						t_ia32_prvl	privilege,
						t_uint16*	selector);


/*
 * ../ldt.c
 */

t_status		ia32_ldt_base(t_ia32_ldt*		table,
				      t_paddr*			addr);

t_status		ia32_ldt_size(t_ia32_ldt*		table,
				      t_uint16*			size);

t_status		ia32_ldt_dump(t_ia32_ldt*		table);

t_status		ia32_ldt_activate(t_ia32_ldt		table);

t_status		ia32_ldt_build(t_uint16			entries,
				       t_paddr			base,
				       t_ia32_ldt*		ldt,
				       t_uint8			clear);

t_status		ia32_ldt_destroy(t_ia32_ldt		*ldt);

t_status		ia32_ldt_add_segment(t_ia32_ldt*	table,
					     t_uint16		segment,
					     t_ia32_segment	descriptor);

t_status		ia32_ldt_reserve_segment(t_ia32_ldt*	table,
						 t_ia32_segment	descriptor,
						 t_uint16*	segment);

t_status		ia32_ldt_get_segment(t_ia32_ldt*	table,
					     t_uint16		index,
					     t_ia32_segment*	segment);

t_status		ia32_ldt_delete_segment(t_ia32_ldt*	table,
						t_uint16	segment);

t_status		ia32_ldt_build_selector(t_ia32_ldt*	table,
						t_uint16	segment,
						t_ia32_prvl	privilege,
						t_uint16*	selector);


/*
 * eop
 */

#endif
