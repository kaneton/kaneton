/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * gdt.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:55:06 2005   mycure
 * last update   Sun May 29 15:23:14 2005   mycure
 */

#ifndef IA32_GDT_H
#define IA32_GDT_H		1

/*
 * defines
 */

#define GDT_ADDR		0x10000

#define GDT_ENTRIES		256
#define GDT_VALID_ENTRIES	(1 + 8)

#define GDT_TI_GDT		0x0
#define GDT_TI_LDT		0x1

/*
 * options
 */

#define OPTS_ZERO		0x0
#define OPTS_PRESENT		0x1

/*
 * gdt entries
 */

#define GDT_KERNEL_CS		0x1
#define GDT_KERNEL_DS		0x2
#define GDT_DRIVER_CS		0x3
#define GDT_DRIVER_DS		0x4
#define GDT_SERVICE_CS		0x5
#define GDT_SERVICE_DS		0x6
#define GDT_USER_CS		0x7
#define GDT_USER_DS		0x8

/*
 * gdt flags
 */

#define GDT_GRANULAR		0x8

#define GDT_USE32		0x4
#define GDT_USE16		0x0

#define GDT_AVAILABLE		0x1

/*
 * gdt type
 */

#define GDT_PRESENT		0x80

#define GDT_DPL3		0x60
#define GDT_DPL2		0x40
#define GDT_DPL1		0x20
#define GDT_DPL0		0x00

#define GDT_S			0x10

#define GDT_CODE		0x0a		/* execute and read only */
#define GDT_DATA		0x02		/* read and write */

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
}				__PACKED__ t_gdte;

typedef struct			s_gdtr
{
  t_uint16			size;
  t_uint32			address;
}				__PACKED__ t_gdtr;

typedef t_gdte*			t_gdt;

#endif
