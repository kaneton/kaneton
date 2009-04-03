/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...loader/qemu-mips.mips64/R4000/libR4000.h
 *
 * created          [wed apr  1 09:34:19 2009]
 * updated          [thu apr  2 13:47:49 2009]
 */

#ifndef LIBR4000_H
#define LIBR4000_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "cp0.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	opmode.c
 *	mmu.c
 *	interrupt.c
 */

/*
 * opmode.c
 */

void	enable_error_mode(void);

void	disable_error_mode(void);

void	enable_exception_mode(void);

void	disable_interrupt(void);

void	disable_bootstrap_mode(void);

void	set_kernel_mode(void);


/*
 * mmu.c
 */

void	set_page_size(int page_mask);

void	tlb_flush(void);

void	cache_flush(void);

void	kernel_mem_space_64(void);

void	supervisor_mem_space_64(void);

void	user_mem_space_64(void);


/*
 * interrupt.c
 */

void	clear_ip_bit(unsigned int value);

void	clear_cause_register(void);


/*
 * eop
 */


#endif
