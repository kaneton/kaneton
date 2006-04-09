/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/08/08.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:37:03 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;
extern t_asid	kasid;
extern m_as*	as;

static void check_mapping(t_vaddr start, t_vsize size, t_paddr phys)
{
  t_ia32_table	pt;
  t_ia32_page	pg;
  t_ia32_pde	pde_start;
  t_ia32_pde	pde_end;
  t_ia32_pte	pte_start;
  t_ia32_pte	pte_end;
  t_ia32_pde	pde;
  t_ia32_pte	pte;
  t_uint32	br;
  t_paddr	paddr;
  t_uint8*	p = (t_uint8*)start;

  paddr = phys;

  pde_start = PDE_ENTRY(start);
  pde_end = PDE_ENTRY(start + size);
  pte_start = PTE_ENTRY(start);
  pte_end = PTE_ENTRY(start + size);

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
	}
    }

  if (start < ENTRY_ADDR(PD_MIRROR, 0))
    for (; size > 0; size--, p++)
      *p = *p;
}


void		check_as_08(void)
{
  o_as*		oas;
  o_segment*	oseg;
  o_region*	oreg;
  t_iterator	it;
  t_state	st;
  t_iterator	it2;
  t_state	st2;

  TEST_ENTER;

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
	  if (segment_get(oreg->segid, &oseg) != ERROR_NONE)
	    {
	      printf("error segment_get\n");
	      TEST_LEAVE;
	    }
/*	  printf("check mapping for region %qd of as %qd\n",
		 oreg->regid, oas->asid);
*/	  check_mapping(oreg->address, oreg->size, oseg->address);
	}
    }

  TEST_LEAVE;
}
