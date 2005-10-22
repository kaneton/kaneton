/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * pmode.h
 * 
 * path          /home/mycure/kaneton/papers/lectures/development-environment
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:55:06 2005   mycure
 * last update   Sat Oct 22 21:05:58 2005   mycure
 */

#ifndef IA32_IA32_PMODE_H
#define IA32_IA32_PMODE_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/ia32/ia32/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * general
 */

#define PMODE_GDT_ENTRIES	256

#define PMODE_TI_GDT		0x0
#define PMODE_TI_LDT		0x1

/*
 * gdt entries
 */

#define PMODE_GDT_KERNEL_CS	0x1
#define PMODE_GDT_KERNEL_DS	0x2
#define PMODE_GDT_DRIVER_CS	0x3
#define PMODE_GDT_DRIVER_DS	0x4
#define PMODE_GDT_SERVICE_CS	0x5
#define PMODE_GDT_SERVICE_DS	0x6
#define PMODE_GDT_PROGRAM_CS	0x7
#define PMODE_GDT_PROGRAM_DS	0x8

/*
 * gdt flags
 */

#define PMODE_GDT_GRANULAR	(1 << 3)
#define PMODE_GDT_USE32		(1 << 2)
#define PMODE_GDT_USE16		(0 << 2)
#define PMODE_GDT_AVAILABLE	(1 << 0)

/*
 * gdt type
 */

#define PMODE_GDT_PRESENT	(1 << 7)
#define PMODE_GDT_DPL3		((1 << 6) | (1 << 5))
#define PMODE_GDT_DPL2		(1 << 6)
#define PMODE_GDT_DPL1		(1 << 5)
#define PMODE_GDT_DPL0		(0 << 5)
#define PMODE_GDT_S		(1 << 4)

#define PMODE_GDT_CODE		((1 << 3) | (1 << 1))
#define PMODE_GDT_DATA		(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint16			limit_00_15;
  t_uint16			base_00_15;
  t_uint8			base_16_23;
  t_uint8			type;
  t_uint8			limit_16_19 : 4;
  t_uint8			flags : 4;
  t_uint8			base_24_31;
}				__attribute__ ((packed)) t_gdte;

typedef struct
{
  t_uint16			size;
  t_uint32			address;
}				__attribute__ ((packed)) t_gdtr;

#endif
