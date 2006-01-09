/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:40:20 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../init/init.h"

/*
 * builds some selectors
 */

void		check_pmode_03(void)
{
  t_uint16	size;
  t_uint16	sel;

  LIBIA32_TEST_ENTER;

  if (gdt_size(NULL, &size) != ERROR_NONE)
    {
      printf("error in gdt_size\n");
      LIBIA32_TEST_LEAVE;
    }

  if (gdt_build_selector(GDT_MAX_ENTRIES + 1, prvl_supervisor, &sel) ==
      ERROR_NONE)
    printf("built a selector to a out-of-bound segment\n");

  if (gdt_build_selector(SEGSEL_NULL, prvl_supervisor, &sel) != ERROR_NONE)
    printf("error in gdt_build_selector\n");
  else
    printf("%d\n", sel);

  if (gdt_build_selector(1, prvl_supervisor, &sel) != ERROR_NONE)
    printf("error in gdt_build_selector\n");
  else
    printf("%d\n", sel);

  if (gdt_build_selector(4, prvl_user, &sel) != ERROR_NONE)
    printf("error in gdt_build_selector\n");
  else
    printf("%d\n", sel);

  if (gdt_build_selector(size - 1, prvl_service, &sel) != ERROR_NONE)
    printf("error in gdt_build_selector\n");
  else
    printf("%d\n", sel);

  LIBIA32_TEST_LEAVE;
}
