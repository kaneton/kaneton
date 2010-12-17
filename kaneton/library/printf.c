/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/printf.c
 *
 * created       julien quintard   [thu dec 16 11:16:41 2010]
 * updated       julien quintard   [thu dec 16 11:35:16 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>
#include <modules/modules.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void			printf_character(char			c)
{
  module_call(console, character, c);
}

int			printf(char*				fmt,
			       ...)
{
  va_list		args;
  int			written;

  va_start(args, fmt);

  written = format(printf_character, NULL,
		   fmt, args);

  va_end(args);

  return (written);
}
