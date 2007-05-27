/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/2008-K1/kaneton/check/kaneton/core/map/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sat mar  3 19:50:37 2007]
 */

#ifndef CHECK_MAP_COMMON_H_
# define CHECK_MAP_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../simple_reserve_01/simple_reserve_01.c
 *	../simple_reserve_release_01/simple_reserve_release_01.c
 *	../multiple_reserve_01/multiple_reserve_01.c
 *	../multiple_reserve_release_01/multiple_reserve_release_01.c
 */

/*
 * common.c
 */

void	check_map_common(void);

void	check_map_tests(void);


/*
 * ../map_01/map_01.c
 */

void		check_map_map_01(void);


/*
 * ../simple_reserve/simple_reserve.c
 */

void		check_map_simple_reserve(void);


/*
 * eop
 */

#endif /* !CHECK_MAP_COMMON_H_ */
