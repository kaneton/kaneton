/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libdata/malloc/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [tue dec 20 16:13:01 2005]
 */

#ifndef CHECK_MALLOC_INIT_H_
# define CHECK_MALLOC_INIT_H_

# include <klibc.h>
# include "../../../../../init/init.h"

void		*check_init_memory(char *s, size_t sz);
void		check_memory(char *s, size_t sz);

#endif /* !CHECK_MALLOC_INIT_H_ */
