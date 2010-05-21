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
 * updated       julien quintard   [mon may 17 13:03:08 2010]
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
}			m_forward;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../forward.c
 */

/*
 * ../forward.c
 */

t_error			forward_send(char*			message,
				     t_uint32			length);

t_error			forward_flush(void);

int			forward_write(char				c);

t_error			forward_initialize(void);

t_error			forward_clean(void);


/*
 * eop
 */

#endif
