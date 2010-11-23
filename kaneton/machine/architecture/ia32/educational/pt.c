/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/pt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:48 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:34:39 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage page tables.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dumps a page table.
 */

t_error			ia32_pt_dump(t_ia32_pte*		tab,
				     int			pde)
{
  /*							    [block::pt_dump] */

  t_uint32		i;

  if (ia32_map_pd(&tab) != ERROR_OK)
    return ERROR_KO;

  for (i = 0; i < IA32_PAGE_TABLE_MAX_ENTRIES; i++)
    {
      if (tab[i] & IA32_PAGE_TABLE_ENTRY_FLAG_USED)
	{
	  printf(" sub-entry %d: ", i);
	  printf("0x%x -> base 0x%x, rw %d, usr %d, a %d d %d\n",
		 pde * IA32_PAGE_TABLE_MAX_ENTRIES * PAGESZ +
		 i * PAGESZ,
		 IA32_BASE(tab[i]),
		 !!(tab[i] & IA32_PAGE_TABLE_ENTRY_FLAG_RW),
		 !!(tab[i] & IA32_PAGE_TABLE_ENTRY_FLAG_USER),
		 !!(tab[i] & IA32_PAGE_TABLE_ENTRY_FLAG_A),
		 !!(tab[i] & IA32_PAGE_TABLE_ENTRY_FLAG_D));
	}
    }

  if (ia32_unmap_chunk(tab) != ERROR_OK)
    return ERROR_KO;

  /*							 [endblock::pt_dump] */

  return ERROR_OK;
}

/*
 * builds a new page table.
 *						     [block::pt_build::comment]
 *
 * steps:
 *
 * 1) checks address encoding.
 * 2) initializes the structure.
 *						  [endblock::pt_build::comment]
 */

t_error			ia32_pt_build(t_paddr			base,
				      t_ia32_table*		table)
{
  /*							   [block::pt_build] */

  assert(table != NULL);

  /*
   * 1)
   */

  if (IA32_BASE(base) != base)
    return ERROR_KO;

  /*
   * 2)
   */

  table->paddr = base;
  table->vaddr = 0;

  /*							[endblock::pt_build] */

  return ERROR_OK;
}

/*
 * adds a page to a table.
 *						  [block::pt_add_page::comment]
 *
 * steps:
 *
 * 1) checks the address of the page.
 * 2) gets the table pointer.
 * 3) setups the entry.
 *					       [endblock::pt_add_page::comment]
 */

t_error			ia32_pt_add_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page		page)
{
  /*							[block::pt_add_page] */

  t_ia32_pte*		t;
  t_uint32		opts = 0;

  assert(entry < IA32_PAGE_TABLE_MAX_ENTRIES);

  /*
   * 1)
   */

  if (IA32_BASE(page.addr) != (t_uint32)page.addr)
    return ERROR_KO;

  /*
   * 2)
   */

  assert(tab->vaddr != 0);
  t = (t_ia32_pte*)tab->vaddr;

  /*
   * 3)
   */

  if (page.present)
    opts |= IA32_PAGE_TABLE_ENTRY_FLAG_P;
  else
    printf("warning: adding non-present page\n");

  if (page.cached == IA32_PAGE_NOTCACHED)
    opts |= IA32_PAGE_DIRECTORY_ENTRY_FLAG_CD;
  if (page.writeback == IA32_PAGE_WRITETHROUGH)
    opts |= IA32_PAGE_DIRECTORY_ENTRY_FLAG_WT;

  opts |= (page.rw == IA32_PAGE_WRITABLE ?
	   IA32_PAGE_TABLE_ENTRY_FLAG_RW :
	   IA32_PAGE_TABLE_ENTRY_FLAG_RO);
  opts |= (page.user == IA32_PAGE_USER ?
	   IA32_PAGE_TABLE_ENTRY_FLAG_USER :
	   IA32_PAGE_TABLE_ENTRY_FLAG_SUPERVISOR);

  opts |= (page.global == IA32_PAGE_GLOBAL ? IA32_PAGE_TABLE_ENTRY_FLAG_G : 0);

  opts |= IA32_PAGE_TABLE_ENTRY_FLAG_USED;

  t[entry] = IA32_BASE(page.addr) | opts;

  /*						     [endblock::pt_add_page] */

  return ERROR_OK;
}

/*
 * gets an entry in a table.
 *						  [block::pt_get_page::comment]
 *
 * steps:
 *
 * 1) gets the table.
 * 2) checks whether an entry is valid or not.
 * 3) fills the page structure.
 *					       [endblock::pt_get_page::comment]
 */

t_error			ia32_pt_get_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page*		page)
{
  /*							[block::pt_get_page] */

  t_ia32_pte*		t;

  assert(page != NULL);

  /*
   * 1)
   */

  assert(tab->vaddr != 0);
  t = (t_ia32_pte*)tab->vaddr;

  /*
   * 2)
   */

  if (!(t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_USED))
    return ERROR_KO;

  /*
   * 3 )
   */

  page->rw = (t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_RW) ?
    IA32_PAGE_WRITABLE : IA32_PAGE_READONLY;
  page->present = !!(t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_P);
  page->user = (t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_USER) ?
    IA32_PAGE_USER : IA32_PAGE_PRIVILEGED;
  page->global = (t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_G) ?
    IA32_PAGE_GLOBAL : IA32_PAGE_NONGLOBAL;
  page->writeback = (t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_WT) ?
    IA32_PAGE_WRITETHROUGH : IA32_PAGE_WRITEBACK;
  page->cached = (t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_CD) ?
    IA32_PAGE_NOTCACHED : IA32_PAGE_CACHED;
  page->addr = IA32_BASE(t[entry]);

  /*						     [endblock::pt_get_page] */

  return ERROR_OK;
}

/*
 * deletes a page entry.
 *					       [block::pt_delete_page::comment]
 *
 * steps:
 *
 * 1) gets the table.
 * 2) checks whether an entry is valid or not.
 * 3) resets the entry.
 *					    [endblock::pt_delete_page::comment]
 */

t_error			ia32_pt_delete_page(t_ia32_table*	tab,
					    t_uint16		entry)
{
  /*						     [block::pt_delete_page] */

  t_ia32_pte*		t;

  /*
   * 1)
   */

  assert(tab->vaddr != 0);
  t = (t_ia32_pte*)tab->vaddr;

  /*
   * 2)
   */

  if (!(t[entry] & IA32_PAGE_TABLE_ENTRY_FLAG_USED))
    return ERROR_KO;

  /*
   * 3)
   */

  t[entry] = 0;

  /*						  [endblock::pt_delete_page] */

  return ERROR_OK;
}
