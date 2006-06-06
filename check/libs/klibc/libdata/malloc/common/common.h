/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:22:46 2005]
 * updated       matthieu bucchianeri   [sun jun  4 19:54:13 2006]
 */

#ifndef CHECK_MALLOC_COMMON_H_
# define CHECK_MALLOC_COMMON_H_

# include <klibc.h>
# include "../../../../../check/common/common.h"

# undef TEST_ENTER

# define TEST_ENTER(_check_leaks_)					\
  int __check_leaks = (_check_leaks_);					\
  t_leaks __memory_leaks;						\
  printf("Test %s\n", __FUNCTION__);					\
  check_leaks_init(&__memory_leaks)

# undef TEST_LEAVE

# define TEST_LEAVE							\
  if (__check_leaks)							\
    check_leaks_display(&__memory_leaks);				\
  printf("%s done.\n", __FUNCTION__);					\
  return

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 *      ../02/02.c
 *      ../03/03.c
 *      ../04/04.c
 *      ../05/05.c
 */

/*
 * common.c
 */

void	check_malloc_common(void);

void	check_malloc_tests(void);

void		*check_init_memory(char *s, size_t sz);

void		check_memory(char *s, size_t sz);


/*
 * ../01/01.c
 */

void	check_malloc_01(void);


/*
 * ../02/02.c
 */

void		check_malloc_02(void);


/*
 * ../03/03.c
 */

void		check_malloc_03(void);


/*
 * ../04/04.c
 */

void		check_malloc_04(void);


/*
 * ../05/05.c
 */

void		check_malloc_05(void);


/*
 * eop
 */

#endif /* !CHECK_MALLOC_COMMON_H_ */
