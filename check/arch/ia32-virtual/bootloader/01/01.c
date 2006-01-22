/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/arch/ia32-virtual/bootloader/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sat jan 21 02:44:44 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../check/common/common.h"

extern t_init*	init;

/*
 * t_init test XXX to be continued !
 */

void		check_bootloader_01(void)
{
  t_uint32	i;

  TEST_ENTER;

  if (!init->memsz)
    printf("bad memsz field\n");

  if (init->initsz != PAGESZ)
    printf("bad initsz field\n");
  if (init->init != (t_paddr)init)
    printf("bad init field\n");

  if (init->segmentssz < init->nsegments * sizeof(o_segment) ||
      init->segmentssz % PAGESZ ||
      !init->nsegments)
    printf("bad segments fields\n");
  for (i = 1; i < init->nsegments; i++)
    {
      if (init->segments[i - 1].address > init->segments[i].address)
	printf("badly sorted segments\n");
    }

  if (init->regionssz < init->nregions * sizeof(o_region) ||
      init->regionssz % PAGESZ ||
      !init->nregions)
    printf("bad regions fields\n");
  for (i = 1; i < init->nregions; i++)
    {
      if (init->regions[i - 1].address > init->regions[i].address)
	printf("badly sorted regions\n");
    }

  TEST_LEAVE;
}
