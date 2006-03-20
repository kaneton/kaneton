/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/event/common/common.h
 *
 * created       renaud voltz   [sun mar 19 00:56:07 2006]
 * updated       renaud voltz   [sun mar 19 00:56:07 2006]
 */

#ifndef CHECK_EVENT_COMMON_H_
# define CHECK_EVENT_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../01/01.c
 *	../02/02.c
 *	../03/03.c
 */

/*
 * common.c
 */

void	check_event_common(void);

void	check_event_tests(void);

void	fake_event_handler(t_uint32				id);


/*
 * ../01/01.c
 */

void			check_event_01(void);


/*
 * ../02/02.c
 */

void                    check_event_02(void);


/*
 * ../03/03.c
 */

void                    check_event_03(void);


/*
 * eop
 */

#endif /* !CHECK_EVENT_COMMON_H_ */
