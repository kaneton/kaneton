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
 * updated       julien quintard   [sun jan 16 01:02:49 2011]
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

#define ARCHITECTURE_GDTE_CODE			((1LL << 43) |		\
						 (1LL << 41))
#define ARCHITECTURE_GDTE_DATA			(1LL << 41)

/*
 * the system segment types.
 */

#define ARCHITECTURE_GDTE_LDT			(1LL << 41)
#define ARCHITECTURE_GDTE_TSS			((1LL << 43) |		\
						 (1LL << 40))
#define ARCHITECTURE_GDTE_TSS_AVAILABLE		ARCHITECTURE_GDTE_TSS
#define ARCHITECTURE_GDTE_TSS_BUSY		((1LL << 43) |		\
						 (1LL << 41) |		\
						 (1LL << 40))

#define ARCHITECTURE_GDTE_CALL			((1LL << 43) |		\
						 (1LL << 42))
#define ARCHITECTURE_GDTE_TRAP			((1LL << 43) |		\
						 (1LL << 42) |		\
						 (1LL << 41) |		\
						 (1LL << 40))
#define ARCHITECTURE_GDTE_INTERRUPT		((1LL << 43) |		\
						 (1LL << 42) |		\
						 (1LL << 41))

/*
 * type definitions.
 *
 * the S bit indicates that the segment is not a system one. the present
 * flag indicates that the segment is a valid one and must therefore be
 * considered by the CPU.
 */

#define ARCHITECTURE_GDTE_S			(1LL << 44)
#define ARCHITECTURE_GDTE_PRESENT		(1LL << 47)

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

#define ARCHITECTURE_GDTE_AVAILABLE		(1LL << 52)
#define ARCHITECTURE_GDTE_16BIT			(0LL << 54)
#define ARCHITECTURE_GDTE_32BIT			(1LL << 54)
#define ARCHITECTURE_GDTE_GRANULARITY		(1LL << 55)

/*
 * these values indicates the table in which is located the selector's target.
 */

#define ARCHITECTURE_GDT_SELECTOR_TI_GDT	(0LL << 2)
#define ARCHITECTURE_GDT_SELECTOR_TI_LDT	(1LL << 2)

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
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function computes the hardware DPL - Descriptor Privilege
 * Level according to a system-based privilege value.
 *
 * the GET macro does the opposite, return the system privilege based on
 * the GDT entry's type.
 */

#define ARCHITECTURE_GDTE_DPL_SET(_privilege_)				\
  (((at_gdte)(_privilege_) & 0x3) << 45)

#define ARCHITECTURE_GDTE_DPL_GET(_gdte_)				\
  (((_gdte_) >> 45) & 0x3)

/*
 * these two macro-function returns a system type value according to
 * the hardware 8-bit field.
 *
 * the first macro-function must be used if the segment is a system one
 * while the second must be used for regular code/data segments.
 */

#define ARCHITECTURE_GDTE_TYPE_SYSTEM(_gdte_)				\
  ((_gdte_) &								\
   (ARCHITECTURE_GDTE_LDT |						\
    ARCHITECTURE_GDTE_TSS |						\
    ARCHITECTURE_GDTE_CALL |						\
    ARCHITECTURE_GDTE_TRAP |						\
    ARCHITECTURE_GDTE_INTERRUPT))

#define ARCHITECTURE_GDTE_TYPE_SEGMENT(_gdte_)				\
  ((_gdte_) &								\
   (ARCHITECTURE_GDTE_CODE |						\
    ARCHITECTURE_GDTE_DATA))

/*
 * this macro-function sets the base address in its GDT entry form.
 */

#define ARCHITECTURE_GDTE_BASE_SET(_base_)				\
  (at_gdte)((((at_gdte)(_base_) & 0x0000ffff) << 16) |			\
	    (((at_gdte)(_base_) & 0x00ff0000) << 16) |			\
	    (((at_gdte)(_base_) & 0xff000000) << 32))

/*
 * this macro-function returns the base address contained in the given
 * GDT entry.
 */

#define ARCHITECTURE_GDTE_BASE_GET(_gdte_)				\
  (t_paddr)((((_gdte_) >> 16) & 0x0000ffff) |				\
	    (((_gdte_) >> 16) & 0x00ff0000) |				\
	    (((_gdte_) >> 32) & 0xff000000))

/*
 * this macro-function sets the limit address in its GDT entry form.
 */

#define ARCHITECTURE_GDTE_LIMIT_GET(_gdte_)				\
  (t_paddr)((((_gdte_) >> 00) & 0x0000ffff) |				\
	    (((_gdte_) >> 32) & 0x000f0000))

/*
 * this macro-function returns the limit address contained in the given
 * GDT entry.
 */
 
#define ARCHITECTURE_GDTE_LIMIT_SET(_limit_)				\
  (at_gdte)((((at_gdte)(_limit_) & 0x0000ffff) << 00) |			\
	    (((at_gdte)(_limit_) & 0x00ff0000) << 32))

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

typedef t_uint64	at_gdte;

/*
 * the GTD descriptor i.e the structure for managing a GDT.
 *
 * this structure contains a pointer to the GDT entries along with the
 * current size i.e number of allocated entries.
 */

typedef struct
{
  at_gdte*		table;
  t_uint16		size;
}			as_gdt;

/*
 * this structure represents the GDTR - GDT Register which is the hardware
 * structure locating the current GDT.
 */

typedef struct
{
  t_uint16		size;
  t_paddr		address;
}			__attribute__ ((packed)) as_gdtr;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../gdt.c
 */

/*
 * ../gdt.c
 */

t_status		architecture_gdt_dump(void);

t_status		architecture_gdt_build(t_paddr		base,
					       t_psize		size,
					       as_gdt*		gdt);

t_status		architecture_gdt_import(as_gdt*		gdt);

t_status		architecture_gdt_export(as_gdt*		gdt);

t_status		architecture_gdt_insert(t_uint16	index,
						t_paddr		base,
						t_paddr		limit,
						t_flags		flags);

t_status		architecture_gdt_reserve(t_paddr	base,
						 t_paddr	limit,
						 t_flags	flags,
						 t_uint16*	index);

t_status		architecture_gdt_delete(t_uint16	index);

t_status		architecture_gdt_selector(t_uint16	index,
						  t_privilege	privilege,
						  t_uint16*	selector);


/*
 * eop
 */

#endif
