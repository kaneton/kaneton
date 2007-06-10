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

# include <libc.h>
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
 * ../inject_01/inject_01.c
 */

void		check_region_inject_01(void);


/*
 * ../multiple_flush_01/multiple_flush_01.c
 */

void		check_region_multiple_flush_01(void);


/*
 * ../multiple_reserve_01/multiple_reserve_01.c
 */

void		check_region_multiple_reserve_01(void);


/*
 * ../multiple_reserve_release_01/multiple_reserve_release_01.c
 */

void		check_region_multiple_reserve_release_01(void);


/*
 * ../multiple_resize_01/multiple_resize_01.c
 */

void		check_region_multiple_resize_01(void);


/*
 * ../reserve_opt_force_01/reserve_opt_force_01.c
 */

void		check_region_reserve_opt_force_01(void);


/*
 * ../reserve_opt_none_01/reserve_opt_none_01.c
 */

void		check_region_reserve_opt_none_01(void);


/*
 * ../resize_error_01/resize_error_01.c
 */

void		check_region_resize_error_01(void);


/*
 * ../simple_flush_01/simple_flush_01.c
 */

void		check_region_simple_flush_01(void);


/*
 * ../simple_get_01/simple_get_01.c
 */

void		check_region_simple_get_01(void);


/*
 * ../simple_resize_01/simple_resize_01.c
 */

void		check_region_simple_resize_01(void);


/*
 * ../simple_write_01/simple_write_01.c
 */

void		check_region_simple_write_01(void);


/*
 * ../split_coalesce_01/split_coalesce_01.c
 */

void		check_region_split_coalesce_01(void);


/*
 * eop
 */

#endif /* !CHECK_REGION_COMMON_H_ */
