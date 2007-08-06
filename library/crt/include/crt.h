/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/crt/include/crt.h
 *
 * created       matthieu bucchianeri   [mon aug  6 00:12:02 2007]
 * updated       matthieu bucchianeri   [mon aug  6 17:48:40 2007]
 */

#ifndef CRT_CRT_H
#define CRT_CRT_H

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * defines the first allocated type for CRT.
 */

#define MESSAGE_TYPE_CRT	32

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * extern declaration of the program entry point.
 */

extern int	main();

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../crt.c
 */

/*
 * ../crt.c
 */

int		_start(i_task		task,
		       i_as		as,
		       i_task		mod,
		       int		argc,
		       char**		argv,
		       char**		envp);

i_task	_crt_get_task_id(void);

i_as	_crt_get_as_id(void);

void	_crt_attach_cons_to(i_task	cons);

void	_crt_attach_cons(void);


/*
 * eop
 */

#endif
