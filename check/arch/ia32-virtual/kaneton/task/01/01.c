/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/task/01/01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [thu apr  6 11:51:03 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;
extern t_asid	kasid;

void		check_task_01(void)
{
  t_ia32_table	pt;
  t_ia32_page	pg;
  t_ia32_pde	pde_start;
  t_ia32_pde	pde_end;
  t_ia32_pte	pte_start;
  t_ia32_pte	pte_end;
  t_ia32_pde	pde;
  t_ia32_pte	pte;
  o_segment*	oseg;
  t_segid	seg;
  o_region*	oreg;
  t_regid	reg;
  t_uint32	i;
  t_uint32	br;
  t_paddr	paddr;

  TEST_ENTER;

  for (i = 0; i < init->nsegments; i++)
    {
      if (segment_get((t_segid)init->segments[i].address, &oseg) !=
	  ERROR_NONE)
	{
	  printf("segment %d not injected correctly\n");
	}
    }

  for (i = 0; i < init->nregions; i++)
    {
      if (region_get(kasid, (t_regid)init->regions[i].address, &oreg) !=
	  ERROR_NONE)
	{
	  printf("region %d of init not added correctly\n");
	}
    }

  for (i = 0; i < init->nregions; i++)
    {
      if (region_get(kasid, (t_regid)init->regions[i].address, &oreg) !=
	  ERROR_NONE)
	{
	  printf("skipping region %d\n", i);
	  continue;
	}

      paddr = (t_paddr)oreg->segid;

      pde_start = PDE_ENTRY(init->regions[i].address +
			    init->regions[i].offset);
      pde_end = PDE_ENTRY(init->regions[i].address +
			  init->regions[i].offset +
			  init->regions[i].size);
      pte_start = PTE_ENTRY(init->regions[i].address +
			    init->regions[i].offset);
      pte_end = PDE_ENTRY(init->regions[i].address +
			  init->regions[i].offset +
			  init->regions[i].size);

      for (br = 0, pde = pde_start; !br && pde <= pde_end; pde++)
	{
	  if (pd_get_table(NULL, pde, &pt) != ERROR_NONE)
	    {
	      printf("region %d of init badly mapped (no page table)\n");
	      break;
	    }

	  for (pte = (pde == pde_start ? pte_start : 0);
	       pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
	       pte++)
	    {
	      if (pt_get_page(&pt, pte, &pg) != ERROR_NONE)
		{
		  printf("region %d of init badly mapped (no pte for %p)\n",
			 ENTRY_ADDR(pde, pte));
		  br = 1;
		  break;
		}
	      if (pg.addr != paddr)
		{
		  printf("region %d of init badly mapped (%p mapped to %p,"
			 "awaited address: %p\n",
			 ENTRY_ADDR(pde, pte), pg.addr, paddr);
		  br = 1;
		  break;
		}
	      paddr += PAGESZ;
	    }
	}
    }

  TEST_LEAVE;
}
