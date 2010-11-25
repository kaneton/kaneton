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
 * updated       julien quintard   [wed nov 24 13:44:24 2010]
 */

#ifndef MODULES_FORWARD_FORWARD_H
#define MODULES_FORWARD_FORWARD_H	1

/*
 * ---------- structures ------------------------------------------------------
 */

/*
 * the module structure.
 */

typedef struct
{
  char			buffer[128];
  t_uint32		size;
}			m_module_forward;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../forward.c
 */

/*
 * ../forward.c
 */

t_error			module_forward_send(char*		message,
					    t_uint32		length);

t_error			module_forward_flush(void);

int			module_forward_write(char			c);

t_error			module_forward_initialize(void);

t_error			module_forward_clean(void);


/*
 * eop
 */

#endif
