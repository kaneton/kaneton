/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ture/ia32/educational/include/register.h
 *
 * created       julien quintard   [wed jan 12 03:44:59 2011]
 * updated       julien quintard   [sun jan 16 12:19:46 2011]
 */

#ifndef ARCHITECTURE_REGISTER_H
#define ARCHITECTURE_REGISTER_H

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the PCD - Page-level Cache disable and PCE - Page-level Cache Enable
 * control whether caching is activated.
 */

#define ARCHITECTURE_REGISTER_CR3_PCD		(1 << 4)
#define ARCHITECTURE_REGISTER_CR3_PCE		(0 << 4)

/*
 * these macro defines whether the caching mechanism is PWT - Page-level
 * Write Through or PWB - Page-level Write Back.
 */

#define ARCHITECTURE_REGISTER_CR3_PWT		(1 << 3)
#define ARCHITECTURE_REGISTER_CR3_PWB		(0 << 3)

/*
 * this flag enables the maskable interrupts to be delivered.
 */

#define ARCHITECTURE_REGISTER_EFLAGS_IF		(1 << 9)

/*
 * this flag enables CPU's single step mode.
 */

#define ARCHITECTURE_REGISTER_EFLAGS_TF		(1 << 8)

/*
 * this flag enables resuming from a single step trap.
 */

#define ARCHITECTURE_REGISTER_EFLAGS_RF		(1 << 16)

/*
 * the bit 1 of the eflags must always be set.
 */

#define ARCHITECTURE_REGISTER_EFLAGS_01		(1 << 1)

/*
 * ---------- macro-functions -------------------------------------------------
 */

/*
 * this macro-function extracts the page directory address from a CR3
 * register.
 */

#define ARCHITECTURE_REGISTER_CR3_ADDRESS_GET(_cr3_)			\
  (_cr3_) & 0xfffff000

/*
 * this macro-function sets the IOPL - I/O Privilege Level in the
 * EFLAGS register.
 */

#define ARCHITECTURE_REGISTER_EFLAGS_IOPL_SET(_privilege_)		\
  (((_privilege_) & 0x3 )<< 12)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <architecture/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * these types represent the CR registers.
 */

typedef t_uint32		at_cr0;
typedef t_uint32		at_cr1;
typedef t_uint32		at_cr2;
typedef t_uint32		at_cr3;
typedef t_uint32		at_cr4;

#endif
