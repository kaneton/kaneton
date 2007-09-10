/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/bootloader/arch/ia32-virtual/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sat oct 14 16:45:43 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;

typedef struct
{
  t_uint16			size;
  t_uint32			base;
}				__attribute__ ((packed)) t_gdtr;

#define CHECK_SEG(Id)							\
  if ((init->segments[(Id)].address % PAGESZ) ||			\
      (init->segments[(Id)].size % PAGESZ) ||				\
      !init->segments[(Id)].size ||					\
      init->segments[(Id)].perms <= 0 ||				\
      init->segments[(Id)].perms > 8)					\
    printf("error in segment %d: bad alignment or perms\n", (Id))

#define CHECK_REG(Id)							\
  if ((init->regions[(Id)].address % PAGESZ) ||				\
      (init->regions[(Id)].size % PAGESZ) ||				\
      !init->regions[(Id)].size ||					\
      (init->regions[(Id)].offset % PAGESZ))				\
    printf("error in region %d: bad alignment\n", (Id))

#define LOOKUP_SEG(Addr,Sz,Desc)					\
  for (i = 0; i < INIT_SEGMENTS; i++)					\
    if (init->segments[i].address == (t_paddr)(Addr))			\
      break;								\
  if (i == INIT_SEGMENTS)						\
    printf("no segment for " Desc "\n");				\
  else									\
    {									\
      if (init->segments[i].size != (Sz))				\
	printf("bad segment size for " Desc "\n");			\
    }

#define LOOKUP_REG(Addr,Sz,Desc)					\
  for (i = 0; i < INIT_REGIONS; i++)					\
    if (init->regions[i].address == (t_vaddr)(Addr))			\
      break;								\
  if (i == INIT_REGIONS)						\
    printf("no region for " Desc "\n");					\
  else									\
    {									\
      if (init->regions[i].size != (Sz))				\
	printf("bad region size for " Desc "\n");			\
    }

/*
 * segments and regions test
 */

void			check_ia32_virtual_02(void)
{
  t_uint32		i;
  register t_uint32	cr3;
  t_gdtr		gdtr;

  TEST_ENTER();

  for (i = 0; i < INIT_SEGMENTS; i++)
    {
      CHECK_SEG(i);
    }
  for (i = 0; i < INIT_REGIONS; i++)
    {
      CHECK_REG(i);
    }

  /*
   * search a segment + region for segments, regions and modules.
   */

  LOOKUP_SEG(init->segments, init->segmentssz, "init->segments");
  LOOKUP_SEG(init->regions, init->regionssz, "init->regions");
  LOOKUP_SEG(init->modules, init->modulessz, "init->modules");

  /*
   * search a segment + region for GDT, PD, kcode and kstack, init and alloc.
   */

  asm volatile("movl %%cr3, %0\n\t"
	       "sgdt %1\n\t"
	       : "=r" (cr3), "=g" (gdtr));

  LOOKUP_SEG(init->kstack, init->kstacksz, "kernel stack");
  LOOKUP_REG(init->kstack, init->kstacksz, "kernel stack");
  LOOKUP_SEG(init->kcode, init->kcodesz, "kernel code");
  LOOKUP_REG(init->kcode, init->kcodesz, "kernel code");
  LOOKUP_SEG(init->init, init->initsz, "init");
  LOOKUP_REG(init->init, init->initsz, "init");
  LOOKUP_SEG(init->alloc, init->allocsz, "survey area");
  LOOKUP_REG(init->alloc, init->allocsz, "survey area");

  LOOKUP_SEG(cr3, PAGESZ, "page directory");
  LOOKUP_REG(cr3, PAGESZ, "page directory");
  LOOKUP_SEG(gdtr.base, PAGESZ, "gdt");
  LOOKUP_REG(gdtr.base, PAGESZ, "gdt");

  TEST_LEAVE();
}
