/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/modules/forward/include/forward.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [fri dec 10 14:12:31 2010]
 */

#ifndef MODULES_FORWARD_FORWARD_H
#define MODULES_FORWARD_FORWARD_H	1

/*
 * ---------- structures ------------------------------------------------------
 */

/*
 * the module manager.
 */

typedef struct
{
  char			buffer[128];
  t_uint32		size;
}			mm_forward;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../forward.c
 */

/*
 * ../forward.c
 */

t_status		module_forward_send(char*		message,
					    t_uint32		length);

t_status		module_forward_flush(void);

void			module_forward_character(char		c);

t_status		module_forward_load(void);

t_status		module_forward_unload(void);


/*
 * eop
 */

#endif
