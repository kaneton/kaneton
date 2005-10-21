/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strdup.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:59 2005   mycure
 * last update   Sun Aug 28 14:51:30 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strdup(const char*			from)
{
  char*			to;

  if ((to = malloc((strlen(from) + 1) * sizeof(char))) == NULL)
    return (NULL);

  strcpy(to, from);

  return (to);
}
