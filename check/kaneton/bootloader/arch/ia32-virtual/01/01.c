/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/bootloader/arch/ia32-virtual/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sat jul 22 19:01:41 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;

/*
 * t_init test
 */

void		check_ia32_virtual_01(void)
{
  t_uint32	i;
  t_reg32	ebp;
  t_reg32	esp;

  TEST_ENTER;

  if (init->mem)
    printf("bad mem field\n");
  if (!init->memsz || init->memsz % 1024)
    printf("bad memsz field\n");

  if (init->kcode % PAGESZ || init->kcode > (t_paddr)check_ia32_virtual_01)
    printf("bad kcode field\n");
  if (!init->kcodesz || init->kcodesz % PAGESZ)
    printf("bad kcodesz field\n");

  if (init->initsz != PAGESZ)
    printf("bad initsz field\n");
  if (init->init != (t_paddr)init || init->init % PAGESZ)
    printf("bad init field\n");

  if (init->segmentssz < init->nsegments * sizeof(o_segment) ||
      (init->segmentssz % PAGESZ) ||
      !init->nsegments || init->nsegments != INIT_SEGMENTS ||
      ((t_paddr)init->segments % PAGESZ))
    printf("bad segments fields\n");
  else
    for (i = 1; i < init->nsegments; i++)
      {
	if (init->segments[i - 1].address > init->segments[i].address)
	  printf("badly sorted segments\n");
      }

  if (init->regionssz < init->nregions * sizeof(o_region) ||
      (init->regionssz % PAGESZ) ||
      !init->nregions || init->nregions != INIT_REGIONS ||
      ((t_paddr)init->regions % PAGESZ))
    printf("bad regions fields\n");
  else
    for (i = 1; i < init->nregions; i++)
      {
	if (init->regions[i - 1].address > init->regions[i].address)
	  printf("badly sorted regions\n");
      }

  if (!init->kstack || init->kstack % PAGESZ)
    printf("bad kstack field\n");
  if (!init->kstacksz || init->kstacksz % PAGESZ)
    printf("bad kstacksz field\n");

  asm volatile("movl %%esp, %0\n\t"
	       "movl %%ebp, %1\n\t"
	       : "=m" (esp), "=m" (ebp));

  if (esp % 4 || ebp % 4)
    printf("bad esp or ebp alignment\n");

  if (!init->alloc || init->alloc % PAGESZ)
    printf("bad alloc field\n");
  if (!init->allocsz || init->allocsz % PAGESZ || init->allocsz < 8 * PAGESZ)
    printf("bad allocsz field\n");

  TEST_LEAVE;
}
