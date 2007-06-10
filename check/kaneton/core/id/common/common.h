/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check-clean/kaneton/core/id/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [sun may 27 20:13:37 2007]
 */

#ifndef CHECK_ID_COMMON_H_
# define CHECK_ID_COMMON_H_

# include <libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../simple_01/simple_01.c
 *	../multiple_01/multiple_01.c
 *	../clone_01/clone_01.c
 */

/*
 * common.c
 */

void	check_id_common(void);

void	check_id_tests(void);


/*
 * ../simple_01/simple_01.c
 */

void		check_id_simple_01(void);


/*
 * ../multiple_01/multiple_01.c
 */

void		check_id_multiple_01(void);


/*
 * ../clone_01/clone_01.c
 */

void		check_id_clone_01(void);


/*
 * eop
 */

#endif /* !CHECK_ID_COMMON_H_ */
