/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Thu Jun  9 17:59:12 2005   mycure
 */

#ifndef KANETON_BOOTLOADER_H
#define KANETON_BOOTLOADER_H	1

/*
 * bootloader modules
 */

typedef struct			s_bmod
{
  char*				name;
  t_paddr			address;
  t_psize			size;
}				t_bmod;

typedef struct			s_bmods
{
  t_uint32			nbmods;
  t_bmod*			bmods;
}				t_bmods;

/*
 * bootloader areas
 */

typedef struct			s_barea
{
  t_paddr			address;
  t_psize			size;
}				t_barea;

typedef struct			s_bareas
{
  t_uint32			nbareas;
  t_barea*			bareas;
}				t_bareas;

/*
 * kernel areas
 */

typedef struct			s_karea
{
  t_paddr			address;
  t_psize			size;
}				t_karea;

typedef struct			s_kareas
{
  t_uint32			nkareas;
  t_karea*			kareas;
}				t_kareas;

/*
 * bootloader memory structure
 */

typedef struct			s_bmem
{
  t_paddr			mem;
  t_psize			memsz;

  t_paddr			kcode;
  t_psize			kcodesz;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			bmods;
  t_psize			bmodssz;

  t_paddr			bareas;
  t_psize			bareassz;

  t_paddr			kareas;
  t_psize			kareassz;

  machdep_include(bmem);
}				t_bmem;

#ifdef ___bootloader

/*
 * prototypes:     ../../bootloader/arch/machdep/bootloader.h
 *                 ../../bootloader/arch/machdep/elf.h
 *                 ../../bootloader/arch/machdep/machdep.h
 *                 ../../bootloader/arch/machdep/paging.h
 *                 ../../bootloader/arch/machdep/stdarg.h
 *                 ../../bootloader/arch/machdep/asm.h
 *                 ../../bootloader/arch/machdep/cons.h
 *                 ../../bootloader/arch/machdep/limits.h
 *                 ../../bootloader/arch/machdep/multiboot.h
 *                 ../../bootloader/arch/machdep/pmode.h
 *                 ../../bootloader/arch/machdep/types.h
 */

/*
 * ../../bootloader/arch/machdep/bootloader.h
 */

/*
 * ../../bootloader/arch/machdep/elf.h
 */

/*
 * ../../bootloader/arch/machdep/machdep.h
 */

/*
 * ../../bootloader/arch/machdep/paging.h
 */

/*
 * ../../bootloader/arch/machdep/stdarg.h
 */

/*
 * ../../bootloader/arch/machdep/asm.h
 */

/*
 * ../../bootloader/arch/machdep/cons.h
 */

/*
 * ../../bootloader/arch/machdep/limits.h
 */

/*
 * ../../bootloader/arch/machdep/multiboot.h
 */

/*
 * ../../bootloader/arch/machdep/pmode.h
 */

/*
 * ../../bootloader/arch/machdep/types.h
 */
/*
 * eop
 */

#endif

#endif
