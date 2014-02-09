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
 * updated       julien quintard   [sun feb  6 13:48:48 2011]
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
 * these macros define the types of IDT gates.
 */

#define ARCHITECTURE_IDTE_TASK			((1LL << 42) |		\
						 (1LL << 40))
#define ARCHITECTURE_IDTE_TRAP			((1LL << 42) |		\
						 (1LL << 41) |		\
						 (1LL << 40))
#define ARCHITECTURE_IDTE_INTERRUPT		((1LL << 42) |		\
						 (1LL << 41))

/*
 * type definitions.
 */

#define ARCHITECTURE_IDTE_32BIT			(1LL << 43)
#define ARCHITECTURE_IDTE_16BIT			(0LL << 43)

#define ARCHITECTURE_IDTE_PRESENT		(1LL << 47)

/*
 * these macros define the base entry and the number of entries for the
 * several types of gate: IRQ, exception, IPI or syscall.
 *
 * note that 200 syscalls could be set up but the kernel limits itself
 * to ten which is enough for a microkernel.
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
 * these macro define some of the exception handler sources.
 */

#define ARCHITECTURE_IDT_EXCEPTION_DE					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 0
#define ARCHITECTURE_IDT_EXCEPTION_DB					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 1
#define ARCHITECTURE_IDT_EXCEPTION_BP					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 3
#define ARCHITECTURE_IDT_EXCEPTION_OF					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 4
#define ARCHITECTURE_IDT_EXCEPTION_BR					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 5
#define ARCHITECTURE_IDT_EXCEPTION_UD					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 6
#define ARCHITECTURE_IDT_EXCEPTION_NM					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 7
#define ARCHITECTURE_IDT_EXCEPTION_DF					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 8
#define ARCHITECTURE_IDT_EXCEPTION_TS					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 10
#define ARCHITECTURE_IDT_EXCEPTION_NP					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 11
#define ARCHITECTURE_IDT_EXCEPTION_SS					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 12
#define ARCHITECTURE_IDT_EXCEPTION_GP					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 13
#define ARCHITECTURE_IDT_EXCEPTION_PF					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 14
#define ARCHITECTURE_IDT_EXCEPTION_MF					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 16
#define ARCHITECTURE_IDT_EXCEPTION_AC					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 17
#define ARCHITECTURE_IDT_EXCEPTION_MC					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 18
#define ARCHITECTURE_IDT_EXCEPTION_XM					\
  ARCHITECTURE_IDT_EXCEPTION_BASE + 19

/*
 * these macro define some of the IRQ handler sources.
 */

#define ARCHITECTURE_IDT_IRQ_PIT					\
  ARCHITECTURE_IDT_IRQ_BASE + 0
#define ARCHITECTURE_IDT_IRQ_KEYBOARD					\
  ARCHITECTURE_IDT_IRQ_BASE + 1
#define ARCHITECTURE_IDT_IRQ_CASCADE					\
  ARCHITECTURE_IDT_IRQ_BASE + 2
#define ARCHITECTURE_IDT_IRQ_COM2					\
  ARCHITECTURE_IDT_IRQ_BASE + 3
#define ARCHITECTURE_IDT_IRQ_COM1					\
  ARCHITECTURE_IDT_IRQ_BASE + 4
#define ARCHITECTURE_IDT_IRQ_FLOPPY					\
  ARCHITECTURE_IDT_IRQ_BASE + 6
#define ARCHITECTURE_IDT_IRQ_SPURIOUS					\
  ARCHITECTURE_IDT_IRQ_BASE + 7
#define ARCHITECTURE_IDT_IRQ_RTC					\
  ARCHITECTURE_IDT_IRQ_BASE + 8
#define ARCHITECTURE_IDT_IRQ_ATA1					\
  ARCHITECTURE_IDT_IRQ_BASE + 14
#define ARCHITECTURE_IDT_IRQ_ATA2					\
  ARCHITECTURE_IDT_IRQ_BASE + 15

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this macro-function computes the hardware DPL - Descriptor Privilege
 * Level according to a system-based privilege value.
 *
 * the GET() macro-function does the opposite, return the system privilege
 * based on the IDT entry's type.
 */

#define ARCHITECTURE_IDTE_DPL_SET(_privilege_)				\
  (((at_idte)(_privilege_) & 0x3) << 45)

#define ARCHITECTURE_IDTE_DPL_GET(_idte_)				\
  (((_idte_) >> 45) & 0x3)

/*
 * this macro-function returns the system type according to the
 * hardware type field.
 */

#define ARCHITECTURE_IDTE_TYPE(_type_)					\
  ((_type_) &								\
   (ARCHITECTURE_IDTE_TASK |						\
    ARCHITECTURE_IDTE_TRAP |						\
    ARCHITECTURE_IDTE_INTERRUPT))

/*
 * this macro-function sets the offset in its IDT entry form.
 */

#define ARCHITECTURE_IDTE_OFFSET_SET(_offset_)				\
  (at_idte)((((at_idte)(_offset_) & 0x0000ffff) << 00) |		\
	    (((at_idte)(_offset_) & 0xffff0000) << 32))

/*
 * this macro-function returns the offset contained in the given
 * IDT entry.
 */

#define ARCHITECTURE_IDTE_OFFSET_GET(_idte_)				\
  (t_vaddr)((((_idte_) >> 00) & 0x0000ffff) |				\
	    (((_idte_) >> 32) & 0xffff0000))

/*
 * this macro-function sets the selector in its IDT entry form.
 */

#define ARCHITECTURE_IDTE_SELECTOR_SET(_selector_)			\
  (at_idte)(((at_idte)(_selector_) & 0x0000ffff) << 16)

/*
 * this macro-function returns the selector contained in the given
 * IDT entry.
 */

#define ARCHITECTURE_IDTE_SELECTOR_GET(_idte_)				\
  (t_vaddr)(((_idte_) >> 16) & 0x0000ffff)

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

typedef t_uint64	at_idte;

/*
 * this structure represents an in-memory IDT descriptor.
 */

typedef struct
{
  at_idte*		table;
  t_uint16		size;
}			as_idt;

/*
 * this structure represents the IDTR - IDT register which is the
 * hardware structure for locating the current IDT.
 */

typedef struct
{
  t_uint16		size;
  t_paddr		address;
}			__attribute__ ((packed)) as_idtr;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../idt.c
 */

/*
 * ../idt.c
 */

t_status		architecture_idt_dump(void);

t_status		architecture_idt_build(t_paddr		base,
					       t_uint16		size,
					       as_idt*		idt);

t_status		architecture_idt_import(as_idt*		idt);

t_status		architecture_idt_export(as_idt*		idt);

t_status		architecture_idt_insert(t_uint16	index,
						t_vaddr		offset,
						t_uint16	selector,
						t_flags		flags);

t_status		architecture_idt_delete(t_uint16	index);


/*
 * eop
 */

#endif
