/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:40:07 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * just builds a new gdt from the previous one.
 */

void		check_pmode_02(void)
{
  t_gdtr	gdtr;
  t_gdt		new;
  void		*base;
  t_uint16	size;
  t_segment	seg;
  t_segment	orig;
  int		i;

  LIBIA32_TEST_ENTER;

  SGDT(gdtr);

  if (gdt_size(NULL, &size) != ERROR_NONE)
    {
      printf("error in gdt_size\n");
      LIBIA32_TEST_LEAVE;
    }

  base = malloc(size * sizeof(t_gdte));

  if (gdt_build(size, (t_paddr)base, &new, 1) != ERROR_NONE)
    {
      printf("error in gdt_build\n");
      free(base);
      LIBIA32_TEST_LEAVE;
    }

  if (gdt_get_segment(&new, 0, &seg) == ERROR_NONE)
    {
      printf("table is not clear !\n");
      free(base);
      LIBIA32_TEST_LEAVE;
    }

  if (gdt_import(&new) != ERROR_NONE)
    {
      printf("error in gdt_import\n");
      free(base);
      LIBIA32_TEST_LEAVE;
    }

  for (i = 0; i < size; i++)
    {
      if (gdt_get_segment(NULL, i, &orig) == ERROR_NONE)
	{
	  if (gdt_get_segment(&new, i, &seg) != ERROR_NONE)
	    {
	      printf("table badly imported\n");
	      free(base);
	      LIBIA32_TEST_LEAVE;
	    }
	  if (memcmp(&orig, &seg, sizeof(t_segment)))
	    {
	      printf("table badly imported\n");
	      free(base);
	      LIBIA32_TEST_LEAVE;
	    }
	}
      else
	if (gdt_get_segment(&new, i, &seg) == ERROR_NONE)
	  {
	    printf("table badly imported\n");
	    free(base);
	    LIBIA32_TEST_LEAVE;
	  }
    }

  if (gdt_activate(new) != ERROR_NONE)
    {
      printf("error in gdt_activate\n");
      free(base);
      LIBIA32_TEST_LEAVE;
    }

  printf("new gdt successfully installed !\n");

  LGDT(gdtr);

  free(base);
  LIBIA32_TEST_LEAVE;
}
