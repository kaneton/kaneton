/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...n/boot/bootloader/qemu-mips.mips64/cp0.h
 *
 * created       enguerrand raymond   [mon mar 16 23:04:40 2009]
 * updated       enguerrand raymond   [sat apr 11 02:37:19 2009]
 */

#ifndef BOOTLOADER_CP0_H
#define BOOTLOADER_CP0_H

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * status register constants definition
 */

/*
 * constants to set a bit in the status register
 */

#define KX_SET		0x00000080
#define SX_SET		0x00000040
#define UX_SET		0x00000020
#define ERL_SET		0x00000004
#define EXL_SET		0x00000002
#define IE_SET		0x00000001

/*
 * constants to clear a bit in the status register
 */

#define IP2_CLEAR	0xfffffbffU
#define KX_CLEAR	0xffffff7fU
#define BEV_CLEAR	0xffbfffffU
#define ERL_CLEAR	0xfffffffbU
#define IE_CLEAR	0xfffffffeU
#define KSU_CLEAR	0xffffffe7U

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro function set the status register with
 * the parameter value by logical OR operation
 */

#define	set_status_register(value)	\
__asm__(".set noat\n\t"			\
	".set noreorder\n\t"		\
	"mfc0 $1, $12\n\t"		\
	"or $2, $1, %0\n\t"		\
	"mtc0 $2, $12\n\t"		\
	:				\
	:"r"(value)			\
	)

/*
 * this macro function clear the status register with
 * the parameter value by logical AND operation
 */

#define	clear_status_register(value)	\
__asm__(".set noat\n\t"			\
	".set noreorder\n\t"		\
	"mfc0 $1, $12\n\t"		\
	"and $2, $1, %0\n\t"		\
	"mtc0 $2, $12\n\t"		\
	:				\
	:"r"(value)			\
	)

/*
 * this macro function clear the cause register with
 * the paramter value by logical AND operation
 */

#define	clear_cause_register_bit(value)	\
__asm__(".set noat\n\t"			\
	".set noreorder\n\t"		\
	"mfc0 $1, $13\n\t"		\
	"and $2, $1, %0\n\t"		\
	"mtc0 $2, $13\n\t"		\
	:				\
	:"r"(value)			\
	)

/*
 * this macro copy the value in the register $s2
 */

#define	debug_register(value)			\
__asm__ __volatile__	(".set noat\n\t"	\
			".set noreorder\n\t"	\
			"move $24, %0\n\t"	\
			:			\
			:"r"(value)		\
			)

#endif
