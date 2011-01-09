/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/include/idt.h
 *
 * created       renaud voltz   [fri feb 10 16:36:20 2006]
 * updated       julien quintard   [fri jan  7 19:22:50 2011]
 */

#ifndef ARCHITECTURE_IDT_H
#define ARCHITECTURE_IDT_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro defines the maximum of entries an IDT can hold.
 */

#define ARCHITECTURE_IDT_SIZE			256

/*
 * these macros define the base entry and the number of entries for the
 * several types of gate: IRQ, exception, IPI or syscall.
 *
 * note that 200 syscalls could be set up but the kernel limits itself
 * to ten.
 */

#define ARCHITECTURE_IDT_EXCEPTION_BASE		0
#define ARCHITECTURE_IDT_EXCEPTION_SIZE		32

#define ARCHITECTURE_IDT_IRQ_BASE		32
#define ARCHITECTURE_IDT_IRQ_SIZE		16

#define ARCHITECTURE_IDT_IPI_BASE		48
#define ARCHITECTURE_IDT_IPI_SIZE		8

#define ARCHITECTURE_IDT_SYSCALL_BASE		56
#define ARCHITECTURE_IDT_SYSCALL_SIZE		10

/*
 * these macros define the types of IDT gates.
 */

#define ARCHITECTURE_IDT_TYPE_TASK		((1 << 3) |		\
						 (1 << 2))
#define ARCHITECTURE_IDT_TYPE_TRAP		((1 << 3) |		\
						 (1 << 2) |		\
						 (1 << 1) |		\
						 (1 << 0))
#define ARCHITECTURE_IDT_TYPE_INTERRUPT		((1 << 3) |		\
						 (1 << 2) |		\
						 (1 << 1))

/*
 * type definitions.
 */

#define ARCHITECTURE_IDT_TYPE_PRESENT		(1 << 7)

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this macro-function returns the system type according to the
 * hardware type field.
 */

#define ARCHITECTURE_IDT_TYPE(_type_)					\
  ((_type_) & ((1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 <<  0)))

/*
 * this macro-function computes the hardware DPL - Descriptor Privilege
 * Level according to a system-based privilege value.
 *
 * the GET macro does the opposite, return the system privilege based on
 * the IDT entry's type.
 */

#define ARCHITECTURE_IDT_DPL_SET(_privilege_)				\
  ((_privilege_) << 5)

#define ARCHITECTURE_IDT_DPL_GET(_type_)				\
  (((_type_) >> 5) & 0x3)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure defines an IDT entry composed of an offset i.e the
 * handler to be triggered for instance, a segment selector and a type.
 */

typedef struct
{
  t_uint16		offset_00_15;
  t_uint16		selector;
  t_uint8		reserved;
  t_uint8		type;
  t_uint16		offset_16_31;
}			__attribute__ ((packed)) as_idt_entry;

/*
 * this structure represents an in-memory IDT descriptor.
 */

typedef struct
{
  as_idt_entry*		table;
  t_uint16		size;
}			as_idt_descriptor;

/*
 * this structure represents the IDTR - IDT register which is the
 * hardware structure for locating the current IDT.
 */

typedef struct
{
  t_uint16		size;
  t_paddr		address;
}			__attribute__ ((packed)) as_idt_register;

/*
 * this type indicates the type of the IDT entry.
 */

typedef t_uint8		at_idt_type;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../idt.c
 */

/*
 * ../idt.c
 */

t_error			architecture_idt_dump(void);

t_error			architecture_idt_build(t_paddr		base,
					       t_uint16		size,
					       as_idt_descriptor* descriptor);

t_error			architecture_idt_import(as_idt_descriptor* descriptor);

t_error			architecture_idt_export(as_idt_descriptor* descriptor);

t_error			architecture_idt_insert(t_uint16	index,
						t_vaddr		offset,
						t_uint16	selector,
						at_privilege	privilege,
						at_idt_type	type);

t_error			architecture_idt_delete(t_uint16	index);


/*
 * eop
 */

#endif
