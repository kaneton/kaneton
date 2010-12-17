/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/snprintf.c
 *
 * created       julien quintard   [thu dec 16 11:16:41 2010]
 * updated       julien quintard   [thu dec 16 11:41:34 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>
#include <modules/modules.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

char*			snprintf_buffer = NULL;
int			snprintf_index = 0;
int			snprintf_size = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

void			snprintf_character(char			c)
{
  if ((snprintf_buffer != NULL) &&
      (snprintf_index < snprintf_size))
    snprintf_buffer[snprintf_index++] = c;
}

int			snprintf(char*				buffer,
				 int				size,
				 char*				fmt,
				 ...)
{
  va_list		args;
  int			written;

  snprintf_buffer = buffer;
  snprintf_index = 0;
  snprintf_size = size;

  va_start(args, fmt);

  written = format(snprintf_character, NULL,
		   fmt, args);

  va_end(args);

  snprintf_buffer = NULL;
  snprintf_index = 0;
  snprintf_size = 0;

  return (written);
}
