/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/bootloader.h
 *
 * created       Enguerrand RAYMOND   [sun oct 15 20:57:09 2006]
 * updated       enguerrand raymond   [sun oct 22 21:43:32 2006]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	bootloader.c
 *	cons.c
 */

/*
 * bootloader.c
 */

void			bootloader_error(void);

int			bootloader(void);


/*
 * cons.c
 */

int	bootloader_cons_print_char(char	c);

void	bootloader_cons_attr(u_int8_t	attr);

int	bootloader_cons_init(void);

void			bootloader_cons_msg(char		indicator,
					    char*		fmt,
					    ...);


/*
 * eop
 */

#endif
