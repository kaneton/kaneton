/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/stats/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:01:32 2006]
 */

#ifndef CHECK_STATS_COMMON_H_
# define CHECK_STATS_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 *      ../02/02.c
 *      ../03/03.c
 */

/*
 * common.c
 */

void	check_stats_common(void);

void	check_stats_tests(void);


/*
 * ../01/01.c
 */

void		check_stats_01(void);


/*
 * ../02/02.c
 */

void		check_stats_02(void);


/*
 * ../03/03.c
 */

void		check_stats_03(void);


/*
 * eop
 */

#endif /* !CHECK_STATS_COMMON_H_ */
