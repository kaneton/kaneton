/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/init/init.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [tue dec 20 18:43:44 2005]
 */

#ifndef CHECK_INIT_H_
# define CHECK_INIT_H_

# include <klibc.h>

# define TEST_ENTER(Test)						\
  t_leaks __memory_leaks;						\
  printf("Test " #Test "\n");						\
  check_leaks_init(&__memory_leaks)

# define TEST_LEAVE(Test)						\
  check_leaks_display(&__memory_leaks);					\
  printf(#Test " done.\n")

typedef struct
{

  t_uint32	nb_alloc;
  t_uint32	nb_free;

}		t_leaks;

void	check_leaks_init(t_leaks*	l);
void	check_leaks_display(t_leaks*	l);

#endif /* !CHECK_INIT_H_ */
