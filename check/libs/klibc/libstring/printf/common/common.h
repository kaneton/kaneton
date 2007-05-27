/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:17:30 2006]
 */

#ifndef CHECK_PRINTF_COMMON_H_
# define CHECK_PRINTF_COMMON_H_

# include <klibc.h>
# include "../../../../../check/common/common.h"

#define CHECK_PRINTF(Exp,Len)						\
    if ((Exp) != (Len))							\
      printf("invalid length\n");

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 *      ../02/02.c
 *      ../03/03.c
 *      ../04/04.c
 *      ../05/05.c
 *      ../06/06.c
 *      ../07/07.c
 *      ../08/08.c
 *      ../09/09.c
 *      ../10/10.c
 */

/*
 * common.c
 */

void	check_printf_common(void);

void	check_printf_tests(void);


/*
 * ../01/01.c
 */

void check_printf_01(void);


/*
 * ../02/02.c
 */

void	check_printf_02(void);


/*
 * ../03/03.c
 */

void	check_printf_03(void);


/*
 * ../04/04.c
 */

void	check_printf_04(void);


/*
 * ../05/05.c
 */

void	check_printf_05(void);


/*
 * ../06/06.c
 */

void	check_printf_06(void);


/*
 * ../07/07.c
 */

void	check_printf_07(void);


/*
 * ../08/08.c
 */

void	check_printf_08(void);


/*
 * ../09/09.c
 */

void	check_printf_09(void);


/*
 * ../10/10.c
 */

void	check_printf_10(void);


/*
 * eop
 */

#endif /* !CHECK_PRINTF_COMMON_H_ */
