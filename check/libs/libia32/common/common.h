/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:43:05 2006]
 */

#ifndef CHECK_LIBIA32_COMMON_H_
# define CHECK_LIBIA32_COMMON_H_

# include <klibc.h>
# include "../../../check/common/common.h"

# define LIBIA32_TEST_ENTER						\
  TEST_ENTER;								\
  init_fault_handler();

# define LIBIA32_TEST_LEAVE						\
  TEST_LEAVE;								\
  clean_fault_handler();

void	check_pmode_tests(void);
void	check_paging_tests(void);

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 */

/*
 * common.c
 */

void	check_libia32_common(void);

void	check_libia32_tests(void);

void	fault_handler(void);

void	init_fault_handler(void);

void	clean_fault_handler(void);


/*
 * eop
 */

#endif /* !CHECK_LIBIA32_COMMON_H_ */
