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
 * updated       matthieu bucchianeri   [mon aug  6 00:24:55 2007]
 */

#ifndef CRT_CRT_H
#define CRT_CRT_H

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../crt.c
 */

/*
 * ../crt.c
 */

int	main(int argc, char** argv, char** envp);

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * entry point.
 *
 * the task & as id are passed and the task id of the service and driver
 * manager (mod).
 *
 * initialize the console.
 */

int	_start(i_task		task,
	       i_as		as,
	       i_task		mod);

i_task	_crt_get_task_id(void);

void	_crt_attach_cons(void);


/*
 * eop
 */

#endif
