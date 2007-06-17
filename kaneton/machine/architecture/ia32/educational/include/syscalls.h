/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/include/syscalls.h
 *
 * created       matthieu bucchianeri   [fri jun 15 09:28:34 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:29:23 2007]
 */

#ifndef ARCHIRECTURE_SYSCALLS_H
#define ARCHIRECTURE_SYSCALLS_H

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../syscalls.c
 */

/*
 * ../syscalls.c
 */

t_error			ia32_setup_syscall_ret(i_thread		thread,
					       t_error		exit_value);

t_error			ia32_syscalls_init(void);

t_error			ia32_syscalls_clean(void);


/*
 * eop
 */

#endif
