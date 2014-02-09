/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...cture/ia32/educational/include/virtual.h
 *
 * created       julien quintard   [wed jun  6 16:25:05 2007]
 * updated       matthieu bucchianeri   [sat jun 16 17:41:12 2007]
 */

#ifndef ARCHITECTURE_VIRTUAL_H
#define ARCHITECTURE_VIRTUAL_H	1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../virtual.c
 */

/*
 * ../virtual.c
 */

t_status		ia32_kernel_as_init(i_as	asid);

t_status		ia32_kernel_as_finalize(void);

t_status		ia32_task_as_init(i_as		asid);

t_status		ia32_segmentation_init(void);


/*
 * eop
 */

#endif
