/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/include/gdt.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       julien quintard   [fri jan  7 17:49:39 2011]
 */

#ifndef ARCHITECTURE_GDT_H
#define ARCHITECTURE_GDT_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro represents the maximum number of entries a GDT can contain.
 */

#define ARCHITECTURE_GDT_SIZE			256

/*
 * the code/data segment types.
 */

#define ARCHITECTURE_GDT_TYPE_CODE		((1 << 3) |		\
						 (1 << 1))
#define ARCHITECTURE_GDT_TYPE_DATA		(1 << 1)

/*
 * the system segment types.
 */

#define ARCHITECTURE_GDT_TYPE_LDT		(1 << 1)
#define ARCHITECTURE_GDT_TYPE_TSS		((1 << 3) |		\
						 (1 << 0))
#define ARCHITECTURE_GDT_TYPE_CALL		((1 << 3) |		\
						 (1 << 2))
#define ARCHITECTURE_GDT_TYPE_TRAP		((1 << 3) |		\
						 (1 << 2) |		\
						 (1 << 1) |		\
						 (1 << 0))
#define ARCHITECTURE_GDT_TYPE_INTERRUPT		((1 << 3) |		\
						 (1 << 2) |		\
						 (1 << 1))

/*
 * these macros indicate whether a system is to be used to store a system
 * structure such as a LDT, TSS etc. or as a regular segment i.e for code
 * or data.
 */

#define ARCHITECTURE_GDT_CLASS_SYSTEM		0x1
#define ARCHITECTURE_GDT_CLASS_SEGMENT		0x2

/*
 * type definitions.
 *
 * the S bit indicates that the segment is not a system one. the present
 * flag indicates that the segment is a valid one and must therefore be
 * considered by the CPU.
 */

#define ARCHITECTURE_GDT_TYPE_S			(1 << 4)
#define ARCHITECTURE_GDT_TYPE_PRESENT		(1 << 7)

/*
 * flags definitions.
 *
 * the available bit can be used by the software for its own purpose. the
 * 16bit/32bit flag indicates whether the system operates in 16/32 bit. the
 * meaning of this bit is actually more complicated than that. for more
 * information, please refer to the IA32 documentation. finally, the granular
 * bit indicates whether the limit address should be considered as a number
 * of bytes or a number of pages.
 */

#define ARCHITECTURE_GDT_FLAG_AVAILABLE		(1 << 0)
#define ARCHITECTURE_GDT_FLAG_16BIT		(0 << 2)
#define ARCHITECTURE_GDT_FLAG_32BIT		(1 << 2)
#define ARCHITECTURE_GDT_FLAG_GRANULARITY	(1 << 3)

/*
 * these definitions represents the indexes of the different system
 * segments, for the kernel, driver, service and guest task classes.
 */

#define ARCHITECTURE_GDT_INDEX_KERNEL_CODE	1
#define ARCHITECTURE_GDT_INDEX_KERNEL_DATA	2
#define ARCHITECTURE_GDT_INDEX_DRIVER_CODE	3
#define ARCHITECTURE_GDT_INDEX_DRIVER_DATA	4
#define ARCHITECTURE_GDT_INDEX_SERVICE_CODE	5
#define ARCHITECTURE_GDT_INDEX_SERVICE_DATA	6
#define ARCHITECTURE_GDT_INDEX_GUEST_CODE	7
#define ARCHITECTURE_GDT_INDEX_GUEST_DATA	8

/*
 * these values indicates the table in which is located the selector's target.
 */

#define ARCHITECTURE_GDT_SELECTOR_TI_GDT	(0 << 2)
#define ARCHITECTURE_GDT_SELECTOR_TI_LDT	(1 << 2)

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function computes the hardware DPL - Descriptor Privilege
 * Level according to a system-based privilege value.
 *
 * the GET macro does the opposite, return the system privilege based on
 * the GDT entry's type.
 */

#define ARCHITECTURE_GDT_DPL_SET(_privilege_)				\
  ((_privilege_) << 5)

#define ARCHITECTURE_GDT_DPL_GET(_type_)				\
  (((_type_) >> 5) & 0x3)

/*
 * these two macro-function returns a system type value according to
 * the hardware 8-bit field.
 *
 * the first macro-function must be used if the segment is a system one
 * while the second must be used for regular code/data segments.
 */

#define ARCHITECTURE_GDT_TYPE_SYSTEM(_type_)				\
  ((_type_) & ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)))

#define ARCHITECTURE_GDT_TYPE_SEGMENT(_type_)				\
  ((_type_) & ((1 << 3) | (1 << 1)))

/*
 * this macro-function computes the RPL - Request Privilege Level value
 * according to the given system privilege value.
 */

#define ARCHITECTURE_GDT_SELECTOR_RPL(_privilege_)			\
  ((_privilege_) << 0)

/*
 * this macro-function compute the selector's index field which indicates
 * the GDT entry attached to the selector.
 */

#define ARCHITECTURE_GDT_SELECTOR_INDEX(_index_)			\
  ((_index_) << 3)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <architecture/privilege.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the hardware-represented GDT entry which is composed of a base address,
 * a limit address, a type and some flags.
 *
 * for more information, please refer to the IA32 hardware documentation.
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
}			__attribute__ ((packed)) as_gdt_entry;

/*
 * the GTD descriptor i.e the structure for managing a GDT.
 *
 * this structure contains a pointer to the GDT entries along with the
 * current size i.e number of allocated entries.
 */

typedef struct
{
  as_gdt_entry*		table;
  t_uint16		size;
}			as_gdt_descriptor;

/*
 * this structure represents the GDTR - GDT Register which is the hardware
 * structure locating the current GDT.
 */

typedef struct
{
  t_uint16		size;
  t_paddr		address;
}			__attribute__ ((packed)) as_gdt_register;

/*
 * this type indicates whether a GDT entry represents a system object such
 * as a TSS, a LDT etc. or a code/data segment.
 */

typedef t_uint8		at_gdt_class;

/*
 * this type indicates the type of the segment.
 */

typedef t_uint8		at_gdt_type;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../gdt.c
 */

/*
 * ../gdt.c
 */

t_error			architecture_gdt_dump(void);

t_error			architecture_gdt_build(t_paddr		base,
					       t_psize		size,
					       as_gdt_descriptor* descriptor);

t_error			architecture_gdt_import(as_gdt_descriptor* descriptor);

t_error			architecture_gdt_export(as_gdt_descriptor* descriptor);

t_error			architecture_gdt_insert(t_uint16	index,
						t_paddr		base,
						t_paddr		limit,
						at_privilege	privilege,
						at_gdt_class	class,
						at_gdt_type	type);

t_error			architecture_gdt_reserve(t_paddr	base,
						 t_paddr	limit,
						 at_privilege	privilege,
						 at_gdt_class	class,
						 at_gdt_type	type,
						 t_uint16*	index);

t_error			architecture_gdt_delete(t_uint16	index);

t_error			architecture_gdt_selector(t_uint16	index,
						  at_privilege	privilege,
						  t_uint16*	selector);


/*
 * eop
 */

#endif
