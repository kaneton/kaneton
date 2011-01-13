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
 * updated       julien quintard   [wed jan 12 03:48:21 2011]
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

#define ARCHITECTURE_REGISTER_CR3_PCD	(1 << 4)
#define ARCHITECTURE_REGISTER_CR3_PCE	(0 << 4)

/*
 * these macro defines whether the caching mechanism is PWT - Page-level
 * Write Through or PWB - Page-level Write Back.
 */
#define ARCHITECTURE_REGISTER_CR3_PWT	(1 << 3)
#define ARCHITECTURE_REGISTER_CR3_PWB	(0 << 3)

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
