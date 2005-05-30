/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Mon May 30 14:10:03 2005   mycure
 * last update   Mon May 30 17:06:58 2005   mycure
 */

#ifndef IA32_KANETON_H
#define IA32_KANETON_H		1

/*
 * defines
 */

#define KANETON_KERNEL_CODE	0x200000
#define KANETON_KERNEL_CODESZ	0x200000

#define KANETON_KERNEL_STACK	(KANETON_KERNEL_CODE + KANETON_KERNEL_CODESZ)
#define KANETON_KERNEL_STACKSZ	0x2000

#define KANETON_MODULES		(KANETON_KERNEL_STACK + KANETON_KERNEL_STACKSZ)

#endif
