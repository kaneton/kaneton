/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [tue dec 20 16:13:18 2005]
 */

#ifndef CHECK_SET_INIT_H_
# define CHECK_SET_INIT_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../init/init.h"

typedef void (*t_dispdata)(void *data);

void	check_many_add(t_setid setid, int nb, ...);
void	check_many_remove(t_setid setid, int nb, ...);
void	check_display_set(t_setid setid, t_dispdata disp);

#endif /* !CHECK_SET_INIT_H_ */
