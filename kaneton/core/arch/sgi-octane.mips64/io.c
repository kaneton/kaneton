/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-o2.mips64/io.c
 *
 * created       enguerrand raymond   [wed oct 18 14:26:20 2006]
 * updated       enguerrand raymond   [sat jan 13 22:43:11 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the io manager dispatch.
 */

d_io		io_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

static int	io_cons_print_char(char	c)
{
  return (write(c));
}

static void	io_cons_attr(u_int8_t	attr)
{
}

int	io_cons_init(void)
{
  printf_init(io_cons_print_char, io_cons_attr);
  return (0);
}
