/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/libs/libia32/interrupt/common/common.h
 *
 * created       renaud voltz   [mon mar 13 12:33:02 2006]
 * updated       renaud voltz   [mon mar 13 12:33:02 2006]
 */

#ifndef CHECK_INTERRUPT_COMMON_H_
# define CHECK_INTERRUPT_COMMON_H_

# include <klibc.h>
# include "../../common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 *	../02/02.c
 *	../03/03.c
 */

/*
 * common.c
 */

void	check_interrupt_common(void);

void	check_interrupt_tests(void);


/*
 * ../01/01.c
 */

void			check_interrupt_01(void);


/*
 * ../02/02.c
 */

void			check_interrupt_02(void);


/*
 * ../03/03.c
 */

void			restore(void);

/*
 * XXX EVENT
 */

void			check_interrupt_03(void);

void			restore(void);


/*
 * eop
 */

#endif /* !CHECK_INTERRUPT_COMMON_H_ */
