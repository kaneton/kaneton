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
 * updated       julien quintard   [thu dec  2 21:13:27 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "mapping.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_as*		_as;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_as_mapping(void)
{
  o_as*			oas;
  o_segment*		oseg;
  o_region*		oreg;
  t_iterator		it;
  t_state		st;
  t_iterator		it2;
  t_state		st2;

  TEST_ENTER();
 
  set_foreach(SET_OPTION_FORWARD, _as->ass, &it, st)
    {
      if (set_object(_as->ass, it, (void**)&oas) != ERROR_OK)
	TEST_ERROR("[set_object] error\n");

      set_foreach(SET_OPTION_FORWARD, oas->regions, &it2, st2)
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

          if (set_object(oas->regions, it2, (void**)&oreg) != ERROR_OK)
	    TEST_ERROR("[set_object] error\n");

	  start = oreg->address;
	  size = oreg->size;

	  p = (t_uint8*)start;

	  pages = 0;

          if (segment_get(oreg->segment, &oseg) != ERROR_OK)
	    TEST_ERROR("[segment_get] error\n");

	  phys = oseg->address;
	  paddr = phys;

	  pde_start = IA32_PAGE_DIRECTORY_ENTRY_INDEX(start);
	  pde_end = IA32_PAGE_DIRECTORY_ENTRY_INDEX(start + size);
	  pte_start = IA32_PAGE_TABLE_ENTRY_INDEX(start);
	  pte_end = IA32_PAGE_TABLE_ENTRY_INDEX(start + size);

	  for (br = 0, pde = pde_start; !br && pde <= pde_end; pde++)
	    {
	      if (ia32_pd_get_table(NULL, pde, &pt) != ERROR_OK)
		TEST_ERROR("[ia32_pd_get_table] error: the region at 0x%x "
			   "seems to be incorrectly mapped i.e no page "
			   "table\n",
			   start);

	      pt.vaddr = IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR,
					    pde);

	      for (pte = (pde == pde_start ?
			  pte_start :
			  0);
		   pte < (pde == pde_end ?
			  pte_end :
			  IA32_PAGE_TABLE_MAX_ENTRIES);
		   pte++)
		{
		  if (ia32_pt_get_page(&pt, pte, &pg) != ERROR_OK)
		    TEST_ERROR("[ia32_pt_get_page] error: the region at 0x%x "
			       "seems to be incorrectly mapped i.e no page "
			       "table entry for 0x%x\n",
			       start,
			       IA32_ENTRY_ADDRESS(pde, pte));

		  if (pg.addr != paddr)
		    TEST_ERROR("the region at 0x%x seems to be incorrectly "
			       "mapped i.e 0x%x mapped to 0x%x, expecting "
			       "address 0x%x\n",
			       start,
			       IA32_ENTRY_ADDRESS(pde, pte),
			       pg.addr,
			       paddr);

		  pages++;
		  paddr += PAGESZ;
		}
	    }

	  if (0 && start < IA32_ENTRY_ADDRESS(IA32_PAGE_DIRECTORY_MIRROR, 0))
	    for (; size > 0; size--, p++)
	      *p = *p;
        }
    }

  TEST_SIGNATURE(90fi2ksoorpgk3);

  TEST_LEAVE();
}
