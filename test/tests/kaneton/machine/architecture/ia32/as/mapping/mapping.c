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
 * updated       julien quintard   [sat apr  9 12:39:25 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "mapping.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;
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

  TEST_ENTER();
 
  if (as_get(_kernel.as, &oas) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  set_foreach(SET_OPTION_FORWARD, oas->regions, &it, st)
    {
      t_vaddr		start;
      t_vsize		size;
      at_pt		pt;
      at_pdei		pde_start;
      at_pdei		pde_end;
      at_ptei		pte_start;
      at_ptei		pte_end;
      at_pdei		pde;
      at_ptei		pte;
      t_paddr		paddr;

      if (set_object(oas->regions, it, (void**)&oreg) != STATUS_OK)
	TEST_ERROR("[set_object] error");

      start = oreg->address;
      size = oreg->size;

      if (oreg->segment == ID_UNUSED)
	continue;

      if (segment_get(oreg->segment, &oseg) != STATUS_OK)
	TEST_ERROR("[segment_get] error");

      paddr = oseg->address + oreg->offset;

      pde_start = ARCHITECTURE_PD_INDEX(start);
      pde_end = ARCHITECTURE_PD_INDEX(start + size);

      pte_start = ARCHITECTURE_PT_INDEX(start);
      pte_end = ARCHITECTURE_PT_INDEX(start + size);

      for (pde = pde_start; pde <= pde_end; pde++)
	{
	  // XXX[this test requires _as.machine.pd to exist which is not
	  //     the case for students]
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

	      paddr += ___kaneton$pagesz;
	    }
	}
    }

  TEST_SIGNATURE(90fi2ksoorpgk3);

  TEST_LEAVE();
}
