/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [tue jan  3 23:58:48 2006]
 */

#ifndef CHECK_SET_INIT_H_
# define CHECK_SET_INIT_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/init/init.h"

typedef void (*t_dispdata)(void *data);

void	check_many_add(t_setid setid, int nb, ...);
void	check_many_remove(t_setid setid, int nb, ...);
void	check_display_set(t_setid setid, t_dispdata disp);

void	check_set_init(void);
void	check_set_tests(void);
void	check_set_array_01(void);
void	check_set_array_02(void);
void	check_set_array_03(void);
void	check_set_array_04(void);
void	check_set_array_05(void);
void	check_set_array_06(void);
void	check_set_array_07(void);
void	check_set_array_08(void);
void	check_set_array_09(void);
void	check_set_array_10(void);
void	check_set_array_11(void);
void	check_set_array_12(void);
void	check_set_array_13(void);
void	check_set_ll_01(void);
void	check_set_ll_02(void);
void	check_set_ll_03(void);
void	check_set_ll_04(void);
void	check_set_ll_05(void);
void	check_set_ll_06(void);
void	check_set_ll_07(void);
void	check_set_ll_08(void);
void	check_set_ll_09(void);
void	check_set_ll_10(void);
void	check_set_ll_11(void);
void	check_set_ll_12(void);

#endif /* !CHECK_SET_INIT_H_ */
