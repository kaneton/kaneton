/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/drivers/vga/vga.h
 *
 * created       matthieu bucchianeri   [tue aug 21 23:20:44 2007]
 * updated       matthieu bucchianeri   [tue sep 11 18:21:41 2007]
 */

#ifndef DRIVERS_VGA_H
#define DRIVERS_VGA_H

/*
 * ---------- macros ----------------------------------------------------------
 */

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA
#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * modes enum.
 */

typedef struct
{
  int32_t	width;
  int32_t	height;
  int32_t	bpp;
  void*		dump;
  int32_t	base;
}		t_vga_driver_modes;

/*
 * state variable.
 */

typedef struct
{
  int32_t	busy;
}		t_vga;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	vga.c
 */

/*
 * vga.c
 */

int			vga_driver_serve(void);

int		main();


/*
 * eop
 */

#endif
