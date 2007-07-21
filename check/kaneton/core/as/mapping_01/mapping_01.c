/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/mapping_01/mapping_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:29:42 2007]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern t_init*	init;
extern i_as	kasid;
extern m_as*	as;

/* XXX gerer les address space */

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
  t_uint32	pages = 0;

  paddr = phys;

  pde_start = IA32_PDE_ENTRY(start);
  pde_end = IA32_PDE_ENTRY(start + size);
  pte_start = IA32_PTE_ENTRY(start);
  pte_end = IA32_PTE_ENTRY(start + size);

  for (br = 0, pde = pde_start; !br && pde <= pde_end; pde++)
    {
      if (ia32_pd_get_table(NULL, pde, &pt) != ERROR_NONE)
	{
	  printf("region @ %d badly mapped (no page table)\n", start);
	  break;
	}

      pt.entries = IA32_ENTRY_ADDR(IA32_PD_MIRROR, pde);

      for (pte = (pde == pde_start ? pte_start : 0);
	   pte < (pde == pde_end ? pte_end : IA32_PT_MAX_ENTRIES);
	   pte++)
	{
	  if (ia32_pt_get_page(&pt, pte, &pg) != ERROR_NONE)
	    {
	      printf("region @ %d badly mapped (no pte for %p)\n",
		     start, IA32_ENTRY_ADDR(pde, pte));
	      br = 1;
	      break;
	    }
	  if (pg.addr != paddr)
	    {
	      printf("region @ %d badly mapped (%p mapped to %p,"
		     "awaited address: %p\n",
		     start, IA32_ENTRY_ADDR(pde, pte), pg.addr, paddr);
	      br = 1;
	      break;
	    }
	  pages++;
	  paddr += PAGESZ;
	}
    }

  if (0 && start < IA32_ENTRY_ADDR(IA32_PD_MIRROR, 0))
    for (; size > 0; size--, p++)
      *p = *p;
}


void		check_as_mapping_01(void)
{
  o_as*		oas;
  o_segment*	oseg;
  o_region*	oreg;
  t_iterator	it;
  t_state	st;
  t_iterator	it2;
  t_state	st2;

  TEST_ENTER();

  set_foreach(SET_OPT_FORWARD, as->ass, &it, st)
    {
      if (set_object(as->ass, it, (void**)&oas) != ERROR_NONE)
	{
	  printf("error set_object\n");
	  TEST_LEAVE();
	}

      set_foreach(SET_OPT_FORWARD, oas->regions, &it2, st2)
	{
	  if (set_object(oas->regions, it2, (void**)&oreg) != ERROR_NONE)
	    {
	      printf("error set_object\n");
	      TEST_LEAVE();
	    }
	  if (segment_get(oreg->segid, &oseg) != ERROR_NONE)
	    {
	      printf("error segment_get\n");
	      TEST_LEAVE();
	    }
//	  printf("check mapping for region %qd of as %qd\n",
//		 oreg->regid, oas->asid);
	  check_mapping(oreg->address, oreg->size, oseg->address);
	}
    }

  TEST_LEAVE();
}
