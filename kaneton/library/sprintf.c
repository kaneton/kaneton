/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/sprintf.c
 *
 * created       julien quintard   [thu dec 16 11:16:41 2010]
 * updated       julien quintard   [thu dec 16 11:40:17 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>
#include <modules/modules.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

char*			sprintf_buffer = NULL;
int			sprintf_index = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

void			sprintf_character(char			c)
{
  if (sprintf_buffer != NULL)
    sprintf_buffer[sprintf_index++] = c;
}

int			sprintf(char*				buffer,
				char*				fmt,
			       ...)
{
  va_list		args;
  int			written;

  sprintf_buffer = buffer;
  sprintf_index = 0;

  va_start(args, fmt);

  written = format(sprintf_character, NULL,
		   fmt, args);

  va_end(args);

  sprintf_buffer = NULL;
  sprintf_index = 0;

  return (written);
}
