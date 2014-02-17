/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/pmode/idt.h
 *
 * created       renaud voltz   [fri feb 10 16:36:20 2006]
 * updated       matthieu bucchianeri   [fri mar 16 21:18:50 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * protected mode structures and macros.
 *
 */

#ifndef ARCHITECTURE_IDT_H
#define ARCHITECTURE_IDT_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * misc
 */

#define IA32_IDT_MAX_ENTRIES	256

#define IA32_IDT_EXCEPTION_BASE	0
#define IA32_IDT_IRQ_BASE	32
#define IA32_IDT_IPI_BASE	48
#define IA32_IDT_SYSCALL_BASE	56

/*
 * gate types (for idt entries)
 */

#define IA32_GATE_TYPE_TASK          ((1 << 3) | (1 << 2))
#define IA32_GATE_TYPE_TRAP          ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define IA32_GATE_TYPE_INTERRUPT     ((1 << 3) | (1 << 2) | (1 << 1))

/*
 * gate type
 */

#define IA32_IDT_GET_TYPE(Type)						\
((Type) & ((1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 <<  0)))

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * gates types
 */
typedef enum
  {
	ia32_type_gate_task		= IA32_GATE_TYPE_TASK,
	ia32_type_gate_interrupt	= IA32_GATE_TYPE_INTERRUPT,
	ia32_type_gate_trap		= IA32_GATE_TYPE_TRAP,
  }	t_ia32_gate_type;

/*
 * abstract gate descriptor
 */

typedef struct
{
  t_uint32			offset;
  t_uint16			segsel;
  t_ia32_prvl			privilege;
  t_ia32_gate_type		type;
}				t_ia32_gate;

/*
 * idt entry
 */

typedef struct
{
  t_uint16			offset_00_15;
  t_uint16			segsel;
  t_uint8			reserved;
  t_uint8			type;
  t_uint16			offset_16_31;
}				__attribute__ ((packed)) t_ia32_idte;

/*
 * idt
 */

typedef struct
{
  t_ia32_idte			*descriptor;
  t_uint16			count;
}				t_ia32_idt;

/*
 * idt register
 */

typedef struct
{
  t_uint16			size;
  t_uint32			address;
}				__attribute__ ((packed)) t_ia32_idtr;


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../idt.c
 */

/*
 * ../idt.c
 */

t_status		ia32_idt_dump(t_ia32_idt*		table);

t_status		ia32_idt_size(t_ia32_idt*		table,
				      t_uint16*			size);

t_status		ia32_idt_build(t_uint16		entries,
				       t_paddr		base,
				       t_uint8		clear,
				       t_ia32_idt*	table);

t_status		ia32_idt_activate(t_ia32_idt*		table);

t_status		ia32_idt_import(t_ia32_idt*		table);

t_status		ia32_idt_add_gate(t_ia32_idt*		table,
					  t_uint16		index,
					  t_ia32_gate		gate);

t_status		ia32_idt_get_gate(t_ia32_idt*		table,
					  t_uint16		index,
					  t_ia32_gate*		gate);

t_status		ia32_idt_delete_gate(t_ia32_idt*	table,
					     t_uint16		gate_id);


/*
 * eop
 */

#endif
