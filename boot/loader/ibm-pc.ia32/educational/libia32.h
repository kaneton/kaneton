/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...loader/ibm-pc.ia32/educational/libia32.h
 *
 * created       matthieu bucchianeri   [mon jun 25 23:02:50 2007]
 * updated       julien quintard   [thu apr  7 15:27:40 2011]
 */

#ifndef BOOTLOADER_LIBIA32_H
#define BOOTLOADER_LIBIA32_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * misc
 */

#define IA32_GDT_MAX_ENTRIES	8191
#define IA32_LDT_MAX_ENTRIES	8191

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
 * privilege levels
 */

#define IA32_PRIV_RING0		0
#define IA32_PRIV_RING1		1
#define IA32_PRIV_RING2		2
#define IA32_PRIV_RING3		3

/*
 * descriptor management defines
 */

#define IA32_DESC_TYPE_PRESENT	(1 << 7)

#define IA32_DESC_MK_DPL(Priv)						\
  ((Priv) << 5)

#define IA32_DESC_GET_DPL(Type)						\
  (((Type) >> 5) & 0x3)

/*
 * segment selector defines
 */

/*
 * null segment selector index
 */

#define IA32_SEGSEL_NULL             0

/*
 * segment selector table indicator
 */

#define IA32_SEGSEL_TI_GDT           (0 << 2)
#define IA32_SEGSEL_TI_LDT           (1 << 2)

/*
 * build a segment selector
 */

#define IA32_SEGSEL(_index_, _rpl_)					\
  ((t_uint16)(((_index_) << 3) + (_rpl_)))

/*
 * segment selector requested privilege level
 */

#define IA32_SEGSEL_MK_RPL(Entry)					\
  ((Entry) << 0)

/*
 * segment selector index
 */

#define IA32_SEGSEL_MK_INDEX(Entry)					\
  ((Entry) << 3)

/*
 * the page size
 */

#define PAGESZ			4096

// XXX
#define ROUND_SIZE(_size_)					\
  ((_size_) % PAGESZ ?						\
   (_size_) + PAGESZ - (_size_) % PAGESZ :		\
   (_size_))

/*
 * paging functions flags
 */

#define IA32_PD_MAX_ENTRIES	1024
#define IA32_PT_MAX_ENTRIES	1024
#define IA32_PD_NOTCACHED	0
#define IA32_PD_CACHED		1
#define IA32_PD_WRITETHROUGH	0
#define IA32_PD_WRITEBACK	1

#define IA32_PT_NOTCACHED	0
#define IA32_PT_CACHED		1
#define IA32_PT_WRITETHROUGH	0
#define IA32_PT_WRITEBACK	1
#define IA32_PT_READONLY	0
#define IA32_PT_WRITABLE	1
#define IA32_PT_PRIVILEGED	0
#define IA32_PT_USER		1

#define IA32_PG_NOTCACHED	0
#define IA32_PG_CACHED		1
#define IA32_PG_WRITETHROUGH	0
#define IA32_PG_WRITEBACK	1
#define IA32_PG_READONLY	0
#define IA32_PG_WRITABLE	1
#define IA32_PG_PRIVILEGED	0
#define IA32_PG_USER		1
#define IA32_PG_NONGLOBAL	0
#define IA32_PG_GLOBAL		1

/*
 * page directory entry flags
 */

#define IA32_PDE_FLAG_A			(1 << 5)
#define IA32_PDE_FLAG_CD		(1 << 4)
#define IA32_PDE_FLAG_WT		(1 << 3)
#define IA32_PDE_FLAG_USER		(1 << 2)
#define IA32_PDE_FLAG_SUPERVISOR	(0 << 2)
#define IA32_PDE_FLAG_RO		(0 << 1)
#define IA32_PDE_FLAG_RW		(1 << 1)
#define IA32_PDE_FLAG_P			(1 << 0)

/*
 * page table entry flags
 */

#define IA32_PTE_FLAG_G			(1 << 8)
#define IA32_PTE_FLAG_D			(1 << 6)
#define IA32_PTE_FLAG_A			(1 << 5)
#define IA32_PTE_FLAG_CD		(1 << 4)
#define IA32_PTE_FLAG_WT		(1 << 3)
#define IA32_PTE_FLAG_USER		(1 << 2)
#define IA32_PTE_FLAG_SUPERVISOR	(0 << 2)
#define IA32_PTE_FLAG_RO		(0 << 1)
#define IA32_PTE_FLAG_RW		(1 << 1)
#define IA32_PTE_FLAG_P			(1 << 0)

/*
 * ---------- macro functions -------------------------------------------------
 */

#define IA32_PDE_ENTRY(Paddr)						\
  (((Paddr) >> 22) & 0x3ff)

#define IA32_PTE_ENTRY(Paddr)						\
  (((Paddr) >> 12) & 0x3ff)

#define IA32_ENTRY_ADDR(D,T)						\
  ((((D) << 22) | ((T) << 12)))

#define IA32_MK_BASE(V)							\
  (((t_uint32)(V)) & 0xfffff000)

#define IA32_MK_OFFSET(V)						\
  (((t_uint32)(V)) & ~0xfffff000)

#define		LGDT(_var_)						\
  asm volatile("lgdt %0\n"						\
	       :							\
	       : "m" (_var_))

#define		LCR3(_var_)						\
  asm volatile("movl %0, %%eax\n"					\
	       "movl %%eax, %%cr3\n"					\
	       :							\
	       : "m" (_var_))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * privilege levels
 */

typedef enum
  {
    ia32_prvl_supervisor	= IA32_PRIV_RING0,
    ia32_prvl_privileged	= IA32_PRIV_RING1,
    ia32_prvl_user		= IA32_PRIV_RING3,
  }				t_ia32_prvl;

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
 * abstract record for page
 */

typedef struct
{
  t_paddr	addr;
  t_uint8	present;
  t_uint8	rw;
  t_uint8	user;
  t_uint8	global;
  t_uint8	cached;
  t_uint8	writeback;
}		t_ia32_page;

/*
 * abstract record for table
 */

typedef struct
{
  t_paddr	entries;
  t_uint8	present;
  t_uint8	rw;
  t_uint8	user;
  t_uint8	cached;
  t_uint8	writeback;
}		t_ia32_table;

typedef t_uint32		t_ia32_pde;
typedef t_uint32		t_ia32_pte;

/*
 * abstract record for directory
 */

typedef t_ia32_pde* t_ia32_directory;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	libia32.c
 */

/*
 * libia32.c
 */

t_status		pmode_set_segment_registers(t_uint16	seg_code,
					    t_uint16	seg_data);

t_status			gdt_build(t_uint16		entries,
				  t_paddr		base,
				  t_ia32_gdt*		gdt,
				  t_uint8		clear);

t_status			gdt_activate(t_ia32_gdt		new_gdt);

t_status			gdt_add_segment(t_ia32_gdt*	table,
					t_uint16	segment,
					t_ia32_segment	descriptor);

t_status			gdt_build_selector(t_uint16	segment,
					   t_ia32_prvl	privilege,
					   t_uint16*	selector);

t_status			pd_build(t_paddr			base,
				 t_ia32_directory*		directory,
				 t_uint8			clear);

t_status			pd_activate(t_ia32_directory	dir,
				    t_uint32		cached,
				    t_uint32		writeback);

t_status			pd_add_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table	table);

t_status			pd_get_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table*	table);

t_status			pt_build(t_paddr		base,
				 t_ia32_table*		table,
				 t_uint8		clear);

t_status			pt_add_page(t_ia32_table*	tab,
				    t_uint16		entry,
				    t_ia32_page		page);


/*
 * eop
 */

#endif
