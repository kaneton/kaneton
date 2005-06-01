/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Mon May 30 14:10:03 2005   mycure
 * last update   Wed Jun  1 11:22:45 2005   mycure
 */

#ifndef IA32_BOOTLOADER_H
#define IA32_BOOTLOADER_H	1

/*
 * defines
 */

#define BOOTLOADER_KCODE	0x1000000
#define BOOTLOADER_KCODESZ	0x200000

#define BOOTLOADER_KSTACK	(BOOTLOADER_KCODE + BOOTLOADER_KCODESZ)
#define BOOTLOADER_KSTACKSZ	0x2000

#define BOOTLOADER_MODULES	(BOOTLOADER_KSTACK + BOOTLOADER_KSTACKSZ)

#endif
