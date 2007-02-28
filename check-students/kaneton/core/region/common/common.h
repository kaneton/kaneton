/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/region/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sat oct 14 17:42:23 2006]
 */

#ifndef CHECK_REGION_COMMON_H_
# define CHECK_REGION_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../reserve_opt_none/reserve_opt_none.c
 *	../reserve_opt_force/reserve_opt_force.c
 *	../multiple_flush/multiple_flush.c
 *	../simple_flush/simple_flush.c
 *	../simple_write/simple_write.c
 */

/*
 * common.c
 */

void	check_region_common(void);

void	check_region_tests(void);


/*
 * ../reserve_opt_none/reserve_opt_none.c
 */

void		check_region_reserve_opt_none(void);


/*
 * ../reserve_opt_force/reserve_opt_force.c
 */

void		check_region_reserve_opt_force(void);


/*
 * ../multiple_flush/multiple_flush.c
 */

void		check_region_multiple_flush(void);


/*
 * ../simple_flush/simple_flush.c
 */

void		check_region_simple_flush(void);


/*
 * ../simple_write/simple_write.c
 */

void		check_region_simple_write(void);


/*
 * eop
 */

#endif /* !CHECK_REGION_COMMON_H_ */
