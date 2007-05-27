/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:10:54 2007]
 */

#ifndef CHECK_MALLOC_COMMON_H_
# define CHECK_MALLOC_COMMON_H_

# include <klibc.h>
# include "../../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 *      ../02/02.c
 *      ../03/03.c
 *      ../04/04.c
 *      ../05/05.c
 */

/*
 * common.c
 */

void	check_malloc_common(void);

void	check_malloc_tests(void);

void		*check_init_memory(char *s, size_t sz);

void		check_memory(char *s, size_t sz);


/*
 * ../01/01.c
 */

void	check_malloc_01(void);


/*
 * ../02/02.c
 */

void		check_malloc_02(void);


/*
 * ../03/03.c
 */

void		check_malloc_03(void);


/*
 * ../04/04.c
 */

void		check_malloc_04(void);


/*
 * ../05/05.c
 */

void		check_malloc_05(void);


/*
 * eop
 */

#endif /* !CHECK_MALLOC_COMMON_H_ */
