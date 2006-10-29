/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootstrap/arch/sgi-octane.mips64/bootloader.h
 *
 * created       enguerrand raymond   [thu oct 26 18:21:41 2006]
 * updated       enguerrand raymond   [thu oct 26 18:22:04 2006]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	bootloader.c
 */

/*
 * bootloader.c
 */

int	bootloader_cons_print_char(char	c);

void	bootloader_cons_attr(u_int8_t	attr);

int	bootloader_cons_init(void);

void		bootloader(void);


/*
 * eop
 */

#endif
