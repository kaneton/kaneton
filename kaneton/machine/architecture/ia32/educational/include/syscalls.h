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

t_ia32_context*		ia32_context_of(i_thread	thread);

void			ia32_syshandler_register(void);

void			ia32_syshandler_send(void);

void			ia32_syshandler_transmit(void);

void			ia32_syshandler_receive(void);

void			ia32_syshandler_poll(void);

t_error			ia32_syscall_set_code(i_thread		thread,
					      t_error		error);

t_error			ia32_syscall_set_info(i_thread		thread,
					      t_error		error,
					      t_vsize		size,
					      i_node		sender);

t_error			ia32_syscalls_init(void);

t_error			ia32_syscalls_clean(void);


/*
 * eop
 */

#endif
