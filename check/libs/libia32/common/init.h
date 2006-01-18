/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:41:26 2006]
 */

#ifndef CHECK_LIBIA32_INIT_H_
# define CHECK_LIBIA32_INIT_H_

# include <klibc.h>
# include "../../../check/init/init.h"

# define LIBIA32_TEST_ENTER						\
  TEST_ENTER;								\
  init_fault_handler();

# define LIBIA32_TEST_LEAVE						\
  TEST_LEAVE;								\
  clean_fault_handler();

void	check_libia32_init(void);
void	check_libia32_tests(void);
void	check_pmode_tests(void);
void	check_paging_tests(void);

void	init_fault_handler(void);
void	clean_fault_handler(void);
void	fault_handler();

#endif /* !CHECK_LIBIA32_INIT_H_ */
