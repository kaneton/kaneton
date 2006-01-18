/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [thu dec 29 18:54:08 2005]
 */

#ifndef CHECK_PRINTF_INIT_H_
# define CHECK_PRINTF_INIT_H_

# include <klibc.h>
# include "../../../../../check/init/init.h"

#define CHECK_PRINTF(Exp,Len)						\
    if ((Exp) != (Len))							\
      printf("invalid length\n");

void	check_printf_init(void);
void	check_printf_tests(void);
void	check_printf_01(void);
void	check_printf_02(void);
void	check_printf_03(void);
void	check_printf_04(void);
void	check_printf_05(void);
void	check_printf_06(void);
void	check_printf_07(void);
void	check_printf_08(void);
void	check_printf_09(void);
void	check_printf_10(void);

#endif /* !CHECK_PRINTF_INIT_H_ */
