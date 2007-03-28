/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sat oct 14 17:00:54 2006]
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
 *	../as_map_01/as_map_01.c
 *	../context_01/context_01.c
 *	../exception_01/exception_01.c
 *	../exception_02/exception_02.c
 *	../exception_03/exception_03.c
 *	../exception_errorcode_01/exception_errorcode_01.c
 *	../idt_01/idt_01.c
 *	../idtr_01/idtr_01.c
 *	../int_state_01/int_state_01.c
 *	../softint_01/softint_01.c
 */

/*
 * common.c
 */

void	check_event_common(void);

void	check_event_tests(void);


/*
 * ../as_map_01/as_map_01.c
 */

void		check_event_as_map_01(void);


/*
 * ../context_01/context_01.c
 */

void			check_event_context_01(void);


/*
 * ../exception_01/exception_01.c
 */

void		check_event_exception_01(void);


/*
 * ../exception_02/exception_02.c
 */

void		check_event_exception_02(void);


/*
 * ../exception_03/exception_03.c
 */

void		check_event_exception_03(void);


/*
 * ../exception_errorcode_01/exception_errorcode_01.c
 */

void		check_event_exception_errorcode_01(void);


/*
 * ../idt_01/idt_01.c
 */

void		check_event_idt_01(void);


/*
 * ../idtr_01/idtr_01.c
 */

void		check_event_idtr_01(void);


/*
 * ../int_state_01/int_state_01.c
 */

void		check_event_int_state_01(void);


/*
 * ../softint_01/softint_01.c
 */

void		check_event_softint_01(void);


/*
 * eop
 */

#endif /* !CHECK_EVENT_COMMON_H_ */
