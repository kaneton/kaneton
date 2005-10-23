/*
 * this program takes one argument, the stack size to use when
 * calling a subroutine.
 *
 * try it with a very small stack size like 20 and you will see the
 * error.
 */

#include "call.h"

/*
 * the first example: startup()
 */

void		startup(int		vatican,
			int		faith)
{
  unsigned int	ebp;
  unsigned int	esp;

  printf("[startup] vatican: %d faith: %d\n", vatican, faith);
}

/*
 * the second example: chiche()
 */

void		chiche(int		arg1,
		       int		arg2)
{
  printf("[chiche] arg1: %d arg2: %d\n", arg1, arg2);
}

/*
 * the main function
 */

int		main(int		argc,
		     char**		argv)
{
  int		size;

  if (argc != 2)
    {
      fprintf(stderr, "usage: %s [stacksz]\n", argv[0]);
      exit(-1);
    }

  size = atoi(argv[1]);

  call(size, startup, 42, 84);

  call(size, chiche, 12, 21);

  return (0);
}
