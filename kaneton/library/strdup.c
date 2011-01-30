/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/strdup.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:56:48 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strdup(const char*			from)
{
  char*			to;

  if ((to = malloc((strlen(from) + 1) * sizeof (char))) == NULL)
    return (NULL);

  strcpy(to, from);

  return (to);
}
