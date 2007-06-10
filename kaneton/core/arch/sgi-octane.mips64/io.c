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
 * updated       enguerrand raymond   [sun jan 14 11:16:16 2007]
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
#include <libc.h>
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

int	io_cons_print_char(char	c)
{
  return (write(c));
}

void	io_cons_attr(t_uint8	attr)
{
}

int	io_cons_init(void)
{
  printf_init(io_cons_print_char, io_cons_attr);
  return (0);
}
