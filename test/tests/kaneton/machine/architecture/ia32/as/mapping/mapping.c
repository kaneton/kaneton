/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/architecture/ia32/as/mapping/mapping.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:20:04 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "mapping.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_as		_as;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_as_mapping(void)
{
  o_as*			oas;
  o_segment*		oseg;
  o_region*		oreg;
  s_iterator		it;
  t_state		st;
  s_iterator		it2;
  t_state		st2;

  TEST_ENTER();
 
  set_foreach(SET_OPTION_FORWARD, _as.ass, &it, st)
    {
      if (set_object(_as.ass, it, (void**)&oas) != ERROR_OK)
	TEST_ERROR("[set_object] error");

      set_foreach(SET_OPTION_FORWARD, oas->regions, &it2, st2)
        {
	  t_vaddr	start;
	  t_vsize	size;
	  t_paddr	phys;
	  at_pt		pt;
	  at_pdei	pde_start;
	  at_pdei	pde_end;
	  at_ptei	pte_start;
	  at_ptei	pte_end;
	  at_pdei	pde;
	  at_ptei	pte;
	  t_uint32	br;
	  t_paddr	paddr;
	  t_uint8*	p;
	  t_uint32	pages;

          if (set_object(oas->regions, it2, (void**)&oreg) != ERROR_OK)
	    TEST_ERROR("[set_object] error");

	  start = oreg->address;
	  size = oreg->size;

	  p = (t_uint8*)start;

	  pages = 0;

	  if (oreg->segment == ID_UNUSED)
	    continue;

          if (segment_get(oreg->segment, &oseg) != ERROR_OK)
	    TEST_ERROR("[segment_get] error");

	  phys = oseg->address;
	  paddr = phys;

	  pde_start = ARCHITECTURE_PD_INDEX(start);
	  pde_end = ARCHITECTURE_PD_INDEX(start + size);

	  pte_start = ARCHITECTURE_PT_INDEX(start);
	  pte_end = ARCHITECTURE_PT_INDEX(start + size);

	  for (br = 0, pde = pde_start; !br && pde <= pde_end; pde++)
	    {
	      if (!(_as.machine.pd[pde] & ARCHITECTURE_PDE_PRESENT))
		TEST_ERROR("the region at 0x%x seems to be incorrectly "
			   "mapped i.e no page table",
			   start);

	      pt = (at_pt)ARCHITECTURE_PAGING_ADDRESS(ARCHITECTURE_PD_MIRROR,
						      pde);

	      for (pte = (pde == pde_start ?
			  pte_start :
			  0);
		   pte < (pde == pde_end ?
			  pte_end :
			  ARCHITECTURE_PT_SIZE);
		   pte++)
		{
		  if (ARCHITECTURE_PTE_ADDRESS(pt[pte]) != paddr)
		    TEST_ERROR("the region at 0x%x seems to be incorrectly "
			       "mapped i.e 0x%x mapped to 0x%x, expecting "
			       "address 0x%x",
			       start,
			       ARCHITECTURE_PAGING_ADDRESS(pde, pte),
			       ARCHITECTURE_PTE_ADDRESS(pt[pte]),
			       paddr);

		  pages++;
		  paddr += ___kaneton$pagesz;
		}
	    }
        }
    }

  TEST_SIGNATURE(90fi2ksoorpgk3);

  TEST_LEAVE();
}
