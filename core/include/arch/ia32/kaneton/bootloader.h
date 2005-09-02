/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Tue Aug 30 15:41:25 2005   mycure
 */

#ifndef IA32_KANETON_BOOTLOADER_H
#define IA32_KANETON_BOOTLOADER_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/ia32/ia32/pmode.h>
#include <arch/ia32/ia32/paging.h>
#include <arch/ia32/services/cons.h>
#include <arch/ia32/ia32/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_gdte*			gdt;
  t_pde*			pd;
  t_cons			cons;
}				d_init;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * machdep traps
 */

#define		machdep_data_init()					\
  d_init			machdep

#endif
