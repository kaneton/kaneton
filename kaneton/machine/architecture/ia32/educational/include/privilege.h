/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       
 *
 * file          /home/mycure/kane...ure/ia32/educational/include/privilege.h
 *
 * created       julien quintard   [fri jan  7 11:50:11 2011]
 * updated       julien quintard   [sat jan 15 17:52:25 2011]
 */

#ifndef ARCHITECTURE_PRIVILEGE_H
#define ARCHITECTURE_PRIVILEGE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * the hardware privilege levels.
 */

#define ARCHITECTURE_PRIVILEGE_RING0		0
#define ARCHITECTURE_PRIVILEGE_RING1		1
#define ARCHITECTURE_PRIVILEGE_RING2		2
#define ARCHITECTURE_PRIVILEGE_RING3		3

/*
 * the system privilege levels for the different task classes.
 */

#define ARCHITECTURE_PRIVILEGE_KERNEL		ARCHITECTURE_PRIVILEGE_RING0
#define ARCHITECTURE_PRIVILEGE_DRIVER		ARCHITECTURE_PRIVILEGE_RING1
#define ARCHITECTURE_PRIVILEGE_SERVICE		ARCHITECTURE_PRIVILEGE_RING2
#define ARCHITECTURE_PRIVILEGE_GUEST		ARCHITECTURE_PRIVILEGE_RING3

#endif
