/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/bootloader/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [fri mar 31 16:07:25 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../check/common/common.h"

extern t_init*	init;

/*
 * modules test
 */

void		check_bootloader_03(void)
{
  t_modules*	mods;
  t_module*	mod;
  Elf32_Ehdr*	elf;
  char*		name;
  t_uint32	i;
  t_uint32	sz;

  TEST_ENTER;

  mods = init->modules;
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

  TEST_LEAVE;
}
