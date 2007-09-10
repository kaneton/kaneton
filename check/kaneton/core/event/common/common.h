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

# include <libc/libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *	../as_map_01/as_map_01.c
 *	../context_01/context_01.c
 *	../context_02/context_02.c
 *	../data_01/data_01.c
 *	../exception_01/exception_01.c
 *	../exception_02/exception_02.c
 *	../exception_03/exception_03.c
 *	../exception_04/exception_04.c
 *	../exception_errorcode_01/exception_errorcode_01.c
 *	../exception_errorcode_02/exception_errorcode_02.c
 *	../exception_errorcode_03/exception_errorcode_03.c
 *	../idt_01/idt_01.c
 *	../idtr_01/idtr_01.c
 *	../int_state_01/int_state_01.c
 *	../irq_01/irq_01.c
 *	../irq_02/irq_02.c
 *	../message_01/message_01.c
 *	../pic_01/pic_01.c
 *	../pic_02/pic_02.c
 *	../pic_03/pic_03.c
 *	../softint_01/softint_01.c
 *	../softint_02/softint_02.c
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
 * ../context_02/context_02.c
 */

void			check_event_context_02(void);


/*
 * ../data_01/data_01.c
 */

void		check_event_data_01(void);


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
 * ../exception_04/exception_04.c
 */

void		check_event_exception_04(void);


/*
 * ../exception_errorcode_01/exception_errorcode_01.c
 */

void		check_event_exception_errorcode_01(void);


/*
 * ../exception_errorcode_02/exception_errorcode_02.c
 */

void		check_event_exception_errorcode_02(void);


/*
 * ../exception_errorcode_03/exception_errorcode_03.c
 */

void		check_event_exception_errorcode_03(void);


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
 * ../irq_01/irq_01.c
 */

void		check_event_irq_01(void);


/*
 * ../irq_02/irq_02.c
 */

void		check_event_irq_02(void);


/*
 * ../message_01/message_01.c
 */

void		check_event_message_01(void);


/*
 * ../pic_01/pic_01.c
 */

void		check_event_pic_01(void);


/*
 * ../pic_02/pic_02.c
 */

void		check_event_pic_02(void);


/*
 * ../pic_03/pic_03.c
 */

void		check_event_pic_03(void);


/*
 * ../softint_01/softint_01.c
 */

void		check_event_softint_01(void);


/*
 * ../softint_02/softint_02.c
 */

void		check_event_softint_02(void);


/*
 * eop
 */

#endif /* !CHECK_EVENT_COMMON_H_ */
