/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/task/01/01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [tue apr 11 16:10:00 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;
extern t_asid	kasid;
extern m_as*	as;

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
  o_as*		oas;
  o_segment*	oseg;
  o_region*	oreg;
  t_uint32	i;
  t_uint32	br;
  t_paddr	paddr;
  t_uint32	total_mapped = 0;
  t_uint32	total_kernel = 0;
  t_uint32	total_as = 0;
  t_iterator	it;
  t_state	st;
  t_iterator	it2;
  t_state	st2;

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

      pde_start = PDE_ENTRY(init->regions[i].address);
      pde_end = PDE_ENTRY(init->regions[i].address +
			  init->regions[i].size);
      pte_start = PTE_ENTRY(init->regions[i].address);
      pte_end = PTE_ENTRY(init->regions[i].address +
			  init->regions[i].size);

      for (br = 0, pde = pde_start; !br && pde <= pde_end; pde++)
	{
	  if (pd_get_table(NULL, pde, &pt) != ERROR_NONE)
	    {
	      printf("region %d of init badly mapped (no page table)\n");
	      break;
	    }

	  pt.entries = ENTRY_ADDR(PD_MIRROR, pde);

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
	      total_kernel += 1;
	    }
	}
    }

  for (pde = 0; pde < PD_MAX_ENTRIES; pde++)
    {
      if (pde == PD_MIRROR)
	continue;
      if (pd_get_table(NULL, pde, &pt) == ERROR_NONE)
	{
	  pt.entries = ENTRY_ADDR(PD_MIRROR, pde);

	  for (pte = 0; pte < PT_MAX_ENTRIES; pte++)
	    if (pt_get_page(&pt, pte, &pg) == ERROR_NONE)
	      total_mapped += 1;
	}
    }

  set_foreach(SET_OPT_FORWARD, as->ass, &it, st)
    {
      if (set_object(as->ass, it, (void**)&oas) != ERROR_NONE)
	{
	  printf("error set_object\n");
	  TEST_LEAVE;
	}

      set_foreach(SET_OPT_FORWARD, oas->regions, &it2, st2)
	{
	  if (set_object(oas->regions, it2, (void**)&oreg) != ERROR_NONE)
	    {
	      printf("error set_object\n");
	      TEST_LEAVE;
	    }
	  if (oas->asid == kasid)
	    {
	      for (i = 0; i < init->nregions; i++)
		if (init->regions[i].address == oreg->address)
		  break;
	      if (i == init->nregions &&
		  oreg->address != ENTRY_ADDR(PD_MIRROR, 0))
		total_kernel += oreg->size / PAGESZ;
	    }
	  else
	    total_as += oreg->size / PAGESZ;
	}
    }

  if (total_mapped != total_kernel + total_as)
    {
      printf("mapped = %d\nkernel = %d\nas = %d\n",
	     total_mapped, total_kernel, total_as);
    }

  TEST_LEAVE;
}
