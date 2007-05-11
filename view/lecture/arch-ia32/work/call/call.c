/*
 * this program takes one argument, the stack size to use when
 * calling a subroutine.
 *
 * try it with a very small stack size like 20 and you will see the
 * error.
 *
 * moreover, think about that functions like printf() and free() use
 * many calls to subroutines so a large stack is needed. a size of
 * 512 bytes is pretty sure.
 */

#include "call.h"

unsigned int	size = 0;

/*
 * the first example: startup() then another().
 *
 * each of the two functions will use its own stack.
 */

void		another(int		v1,
			int		v2)
{
  printf("[another] v1: %d v2: %d\n", v1, v2);
}

void		startup(int		vatican,
			int		faith)
{
  printf("[startup] vatican: %d faith: %d\n", vatican, faith);

  call(size, another, 1, 2);
}

/*
 * the second example: chiche() then another().
 *
 * the two functions called use the new stack.
 */

void		chiche(int		arg1,
		       int		arg2)
{
  printf("[chiche] arg1: %d arg2: %d\n", arg1, arg2);

  another(1, 2);
}

/*
 * the main function
 */

int		main(int		argc,
		     char**		argv)
{
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
