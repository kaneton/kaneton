/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/bootloader/arch/ia32-virtual/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sat oct 14 16:46:44 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;
extern i_as	kasid;

/*
 * modules test
 */

void		check_ia32_virtual_03(void)
{
  t_modules*	mods;
  t_module*	mod;
  Elf32_Ehdr*	elf;
  char*		name;
  t_uint32	i;
  t_uint32	sz;
  i_region	reg;

  TEST_ENTER();

  if (region_reserve(kasid,
		     (i_segment)(t_uint32)init->modules,
		     0,
		     REGION_OPT_FORCE,
		     (t_vaddr)init->modules,
		     init->modulessz,
		     &reg) != ERROR_NONE)
    {
      printf("cannot map modules\n");
      TEST_LEAVE();
    }

  mods = (t_modules*)(t_uint32)reg;
  sz = sizeof (t_modules);
  if (mods->nmodules > 1024)
    printf("bad number of modules\n");
  else
    for (i = 0; i < mods->nmodules; i++)
      {
	mod = (t_module*)((char*)mods + sz);
	sz += sizeof (t_module);
	elf = (Elf32_Ehdr*)((char*)mods + sz);
	sz += mod->size;
	name = mod->name;
	sz += strlen(name) + 1;
      }
  sz += sizeof (t_modules);

  if (init->modulessz != sz + (PAGESZ - (sz % PAGESZ)))
    printf("bad modulessz field\n");

  region_release(kasid, reg);

  TEST_LEAVE();
}
