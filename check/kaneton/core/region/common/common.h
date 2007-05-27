/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check-clean/kaneton/core/region/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sun may 27 16:50:49 2007]
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
 *	../inject_01/inject_01.c
 *	../multiple_flush_01/multiple_flush_01.c
 *	../multiple_reserve_01/multiple_reserve_01.c
 *	../multiple_reserve_release_01/multiple_reserve_release_01.c
 *	../multiple_resize_01/multiple_resize_01.c
 *	../reserve_opt_force_01/reserve_opt_force_01.c
 *	../reserve_opt_none_01/reserve_opt_none_01.c
 *	../resize_error_01/resize_error_01.c
 *	../simple_flush_01/simple_flush_01.c
 *	../simple_get_01/simple_get_01.c
 *	../simple_resize_01/simple_resize_01.c
 *	../simple_write_01/simple_write_01.c
 *	../split_coalesce_01/split_coalesce_01.c
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
 * ../multiple_reserve/multiple_reserve.c
 */

void		check_region_multiple_reserve(void);


/*
 * ../simple_get/simple_get.c
 */

void		check_region_simple_get(void);


/*
 * ../simple_resize/simple_resize.c
 */

void		check_region_simple_resize(void);


/*
 * ../multiple_resize/multiple_resize.c
 */

void		check_region_multiple_resize(void);


/*
 * ../resize_error/resize_error.c
 */

void		check_region_resize_error(void);


/*
 * ../split_coalesce/split_coalesce.c
 */

void		check_region_split_coalesce(void);


/*
 * ../inject_01/inject_01.c
 */

void		check_region_inject_01(void);


/*
 * eop
 */

#endif /* !CHECK_REGION_COMMON_H_ */
