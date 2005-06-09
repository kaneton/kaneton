/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/pmode/pmode.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [mon jan 30 22:31:13 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * protected mode structures and defines.
 *
 */

#ifndef IA32_PMODE_GDT_H
#define IA32_PMODE_GDT_H	1

/*                                                                  [cut] k2 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * misc
 */

#define GDT_MAX_ENTRIES		8191
#define LDT_MAX_ENTRIES		8191

/*
 * gdt flags
 */

#define GDT_FLAG_GRANULAR	(1 << 3)
#define GDT_FLAG_USE32		(1 << 2)
#define GDT_FLAG_USE16		(0 << 2)
#define GDT_FLAG_AVAILABLE	(1 << 0)

/*
 * system segment types
 */

#define SEG_TYPE_CODE           ((1 << 3) | (1 << 1))
#define SEG_TYPE_DATA           (1 << 1)
#define SEG_TYPE_LDT            (1 << 1)
#define SEG_TYPE_TSS            ((1 << 3) | (1 << 0))
#define SEG_GATE_CALL           ((1 << 3) | (1 << 2))
#define SEG_GATE_TRAP           ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define SEG_GATE_INTERRUPT      ((1 << 3) | (1 << 2) | (1 << 1))

/*
 * gdt type
 */

#define GDT_TYPE_SYS(Type)						\
  ((Type) & ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))

#define GDT_TYPE_SEG(Type)						\
  ((Type) & ((1 << 3) | (1 << 1)))

#define GDT_TYPE_S		(1 << 4)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
  * user segments types
 */
typedef enum
  {
	type_code = SEG_TYPE_CODE,
	type_data = SEG_TYPE_DATA,
  }	t_usr_segtype;

/*
 * system segments types
 */
typedef enum
  {
	type_ldt = SEG_TYPE_LDT,
	type_tss = SEG_TYPE_TSS,
	type_call_gate = SEG_GATE_CALL,
	type_trap_gate = SEG_GATE_TRAP,
	type_int_gate = SEG_GATE_INTERRUPT,
  }	t_sys_segtype;

typedef union
{
  t_usr_segtype	usr;
  t_sys_segtype	sys;
}		t_segtype;

/*
 * abstract segment descriptor
 */

typedef struct
{
  t_uint32	base;
  t_uint32	limit;
  t_prvl	privilege;
  t_uint8	is_system;
  t_segtype	type;
}		t_segment;

/*
 * gdt entry
 */

typedef struct
{
  t_uint16			limit_00_15;
  t_uint16			base_00_15;
  t_uint8			base_16_23;
  t_uint8			type;
  t_uint8			limit_16_19 : 4;
  t_uint8			flags : 4;
  t_uint8			base_24_31;
}				__attribute__ ((packed)) t_gdte;

/*
 * gdt
 */

typedef struct
{
  t_gdte			*descriptor;
  t_uint16			count;
}				t_gdt;

/*
 * gdt register
 */

typedef struct
{
  t_uint16			size;
  t_uint32			address;
}				__attribute__ ((packed)) t_gdtr;

/*
 * ldt entry
 */

typedef struct
{
  t_uint16			limit_00_15;
  t_uint16			base_00_15;
  t_uint8			base_16_23;
  t_uint8			type;
  t_uint8			limit_16_19 : 4;
  t_uint8			flags : 4;
  t_uint8			base_24_31;
}				__attribute__ ((packed)) t_ldte;

/*
 * ldt
 */

typedef struct
{
  t_ldte			*descriptor;
  t_uint16			count;
  t_uint16			gdt_entry;
}				t_ldt;

/*
 * ldt register
 */

typedef struct
{
  t_uint16			selector;
  t_uint16			size;
  t_uint32			address;
}				__attribute__ ((packed)) t_ldtr;

/*                                                                 [cut] /k2 */

#endif
