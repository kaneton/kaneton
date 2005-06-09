/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * pmode.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:55:06 2005   mycure
 * last update   Thu Jun  9 13:59:38 2005   mycure
 */

#ifndef IA32_PMODE_H
#define IA32_PMODE_H		1

/*
 * defines
 */

#define BOOTLOADER_PMODESZ	(1 * PAGESZ)

#define PMODE_GDT_ADDR		BOOTLOADER_PMODE

#define PMODE_GDT_ENTRIES	256
#define PMODE_GDT_VALID_ENTRIES	(1 + 8)

#define PMODE_GDT_TI_GDT	0x0
#define PMODE_GDT_TI_LDT	0x1

/*
 * options
 */

#define OPTS_ZERO		0x0
#define OPTS_PRESENT		0x1

/*
 * gdt entries
 */

#define PMODE_GDT_KERNEL_CS	0x1
#define PMODE_GDT_KERNEL_DS	0x2
#define PMODE_GDT_DRIVER_CS	0x3
#define PMODE_GDT_DRIVER_DS	0x4
#define PMODE_GDT_SERVICE_CS	0x5
#define PMODE_GDT_SERVICE_DS	0x6
#define PMODE_GDT_USER_CS	0x7
#define PMODE_GDT_USER_DS	0x8

/*
 * gdt flags
 */

#define PMODE_GDT_GRANULAR	0x8

#define PMODE_GDT_USE32		0x4
#define PMODE_GDT_USE16		0x0

#define PMODE_GDT_AVAILABLE	0x1

/*
 * gdt type
 */

#define PMODE_GDT_PRESENT	0x80

#define PMODE_GDT_DPL3		0x60
#define PMODE_GDT_DPL2		0x40
#define PMODE_GDT_DPL1		0x20
#define PMODE_GDT_DPL0		0x00

#define PMODE_GDT_S		0x10

#define PMODE_GDT_CODE		0x0a		/* execute and read only */
#define PMODE_GDT_DATA		0x02		/* read and write */

/*
 * types
 */

typedef struct			s_gdte
{
  t_uint16			limit_00_15;
  t_uint16			base_00_15;
  t_uint8			base_16_23;
  t_uint8			type;
  t_uint8			limit_16_19 : 4;
  t_uint8			flags : 4;
  t_uint8			base_24_31;
}				__attribute__ ((packed)) t_gdte;

typedef struct			s_gdtr
{
  t_uint16			size;
  t_uint32			address;
}				__attribute__ ((packed)) t_gdtr;

#endif
