/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/bootloader.c
 *
 * created       enguerrand raymond   [wed oct 18 14:21:40 2006]
 * updated       enguerrand raymond   [fri oct 20 19:43:54 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>
#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */
typedef unsigned long FILE;
extern FILE *stdin;
extern FILE *stdout;
static FILE arc_stdin = ARC_STDIN;
FILE *stdin = &arc_stdin;

static FILE arc_stdout = ARC_STDOUT;
FILE *stdout = &arc_stdout;

/*
 * the init variable.
 */

extern	t_init		init;

/*
 * these three variable are globals to avoid them to be lost when the
 * bootloader changes its stack.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_init*);

static int		t_printf_char(char	c)
{
  unsigned long	count = 0;

  ArcWrite(*stdout, &c, 1, &count);
  return 0;
}

static void	t_printf_attr(u_int8_t	attr)
{
}

/*
 * TODO: Write Commentaries about bootloader function works.
 */
int		bootloader(t_uint32			magic)
{
  char i = 'E';
  printf_init(t_printf_char, t_printf_attr);
  printf("Enguerrand = %c", i);
  while (1)
    ;
  return 0;
}
