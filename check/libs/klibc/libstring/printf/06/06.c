/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libstring/printf/06/06.c
 *
 * created       matthieu bucchianeri   [mon dec 26 18:04:53 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:12:31 2007]
 */

#include <libc/libc.h>
#include "../common/common.h"

/*
 * string substitution
 */

void	check_printf_06(void)
{
  TEST_ENTER();

  CHECK_PRINTF(printf("%s%s toto %s%s", "test", "%s", "tata", "\n"), 17);

  CHECK_PRINTF(printf("Affiche une chaine %s et %s et encore une %s,"
		      " une petite derniere %s\n", "de caracteres",
		      "encore une autre", "autre", NULL), 101);

  TEST_LEAVE();
}
