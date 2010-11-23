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
 * updated       julien quintard   [tue nov 23 16:55:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "mapping.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		init;
extern i_as		kasid;
extern m_as*		as;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ia32_as_mapping(void)
{
  o_as*			oas;
  o_segment*		oseg;
  o_region*		oreg;
  t_iterator		it;
  t_state		st;
  t_iterator		it2;
  t_state		st2;

  TEST_ENTER();
 
  set_foreach(SET_OPT_FORWARD, as->ass, &it, st)
    {
      if (set_object(as->ass, it, (void**)&oas) != ERROR_NONE)
	TEST_ERROR("[set_object] error\n");

      set_foreach(SET_OPT_FORWARD, oas->regions, &it2, st2)
        {
	  t_vaddr	start;
	  t_vsize	size;
	  t_paddr	phys;
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
	  t_uint8*	p;
	  t_uint32	pages;

          if (set_object(oas->regions, it2, (void**)&oreg) != ERROR_NONE)
	    TEST_ERROR("[set_object] error\n");

	  start = oreg->address;
	  size = oreg->size;

	  p = (t_uint8*)start;

	  pages = 0;

          if (segment_get(oreg->segid, &oseg) != ERROR_NONE)
	    TEST_ERROR("[segment_get] error\n");

	  phys = oseg->address;
	  paddr = phys;

	  pde_start = IA32_PDE_ENTRY(start);
	  pde_end = IA32_PDE_ENTRY(start + size);
	  pte_start = IA32_PTE_ENTRY(start);
	  pte_end = IA32_PTE_ENTRY(start + size);

	  for (br = 0, pde = pde_start; !br && pde <= pde_end; pde++)
	    {
	      if (ia32_pd_get_table(NULL, pde, &pt) != ERROR_NONE)
		TEST_ERROR("[ia32_pd_get_table] error: the region at 0x%x "
			   "seems to be incorrectly mapped i.e no page "
			   "table\n",
			   start);

	      pt.vaddr = IA32_ENTRY_ADDR(IA32_PD_MIRROR, pde);

	      for (pte = (pde == pde_start ? pte_start : 0);
		   pte < (pde == pde_end ? pte_end : IA32_PT_MAX_ENTRIES);
		   pte++)
		{
		  if (ia32_pt_get_page(&pt, pte, &pg) != ERROR_NONE)
		    TEST_ERROR("[ia32_pt_get_page] error: the region at 0x%x "
			       "seems to be incorrectly mapped i.e no page "
			       "table entry for 0x%x\n",
			       start,
			       IA32_ENTRY_ADDR(pde, pte));

		  if (pg.addr != paddr)
		    TEST_ERROR("the region at 0x%x seems to be incorrectly "
			       "mapped i.e 0x%x mapped to 0x%x, expecting "
			       "address 0x%x\n",
			       start,
			       IA32_ENTRY_ADDR(pde, pte),
			       pg.addr,
			       paddr);

		  pages++;
		  paddr += PAGESZ;
		}
	    }

	  if (0 && start < IA32_ENTRY_ADDR(IA32_PD_MIRROR, 0))
	    for (; size > 0; size--, p++)
	      *p = *p;
        }
    }

  TEST_LEAVE();
}
