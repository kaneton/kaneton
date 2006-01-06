/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/include/pmode/pmode.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [fri jan  6 11:53:47 2006]
 */

#ifndef IA32_IA32_PMODE_H
#define IA32_IA32_PMODE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * privilege levels
 */

#define PRIV_RING0	0
#define PRIV_RING1	1
#define PRIV_RING2	2
#define PRIV_RING3	3

/*
 * null segment selector index
 */

#define SEGSEL_NULL		0

/*
 * segment selector table indicator
 */

#define SEGSEL_TI_GDT		(0 << 2)
#define SEGSEL_TI_LDT		(1 << 2)

/*
 * segment selector requested privilege level
 */

#define SEGSEL_MK_RPL(Entry)						\
  ((Entry) << 0)

/*
 * segment selector index
 */

#define SEGSEL_MK_INDEX(Entry)						\
  ((Entry) << 3)

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
 * gdt type
 */


#define GDT_TYPE_SYS(Type)						\
  ((Type) & ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))

#define GDT_TYPE_SEG(Type)						\
  ((Type) & (1 << 3))

#define GDT_TYPE_PRESENT	(1 << 7)
#define GDT_TYPE_S		(1 << 4)

#define GDT_TYPE_CODE		((1 << 3) | (1 << 1))
#define GDT_TYPE_DATA		(0 << 3)
#define GDT_TYPE_LDT		(1 << 1)
#define GDT_TYPE_TSS		((1 << 3) | (1 << 0))

/*
 * gates
 */

#define GDT_GATE_CALL		((1 << 3) | (1 << 2))
#define GDT_GATE_TRAP		((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define GDT_GATE_INTERRUPT	((1 << 3) | (1 << 2) | (1 << 1))

/*
 * gdt type, privilege levels
 */

#define GDT_TYPE_MK_DPL(Priv)						\
  ((Priv) << 5)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * privilege levels
 */

typedef enum
  {
	prvl_supervisor = PRIV_RING0,
	prvl_service = PRIV_RING1,
	prvl_user = PRIV_RING3,
  }	t_prvl;

/*
 * user segments types
 */
typedef enum
  {
	type_code = GDT_TYPE_CODE,
	type_data = GDT_TYPE_DATA,
  }	t_usr_segtype;

/*
 * system segments types
 */
typedef enum
  {
	type_ldt = GDT_TYPE_LDT,
	type_tss = GDT_TYPE_TSS,
	type_call_gate = GDT_GATE_CALL,
	type_trap_gate = GDT_GATE_TRAP,
	type_int_gate = GDT_GATE_INTERRUPT,
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

#endif
