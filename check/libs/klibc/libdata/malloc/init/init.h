/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libdata/malloc/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [thu dec 29 18:53:57 2005]
 */

#ifndef CHECK_MALLOC_INIT_H_
# define CHECK_MALLOC_INIT_H_

# include <klibc.h>
# include "../../../../../check/init/init.h"

void	*check_init_memory(char *s, size_t sz);
void	check_memory(char *s, size_t sz);

void	check_malloc_init(void);
void	check_malloc_tests(void);
void	check_malloc_01(void);
void	check_malloc_02(void);
void	check_malloc_03(void);
void	check_malloc_04(void);

#endif /* !CHECK_MALLOC_INIT_H_ */
