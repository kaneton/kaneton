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

t_status		ia32_pt_dump(t_ia32_pte*		tab)
{
  /*							    [block::pt_dump] */

  t_uint32		i;

  for (i = 0; i < IA32_PT_MAX_ENTRIES; i++)
    {
      if (tab[i] & IA32_PTE_FLAG_USED)
	{
	  printf(" sub-entry %d: ", i);
	  printf("base %p, rw %d, usr %d, a %d d %d\n", IA32_MK_BASE(tab[i]),
		 !!(tab[i] & IA32_PTE_FLAG_RW),
		 !!(tab[i] & IA32_PTE_FLAG_USER),
		 !!(tab[i] & IA32_PTE_FLAG_A),
		 !!(tab[i] & IA32_PTE_FLAG_D));
	}
    }

  /*							 [endblock::pt_dump] */

  return STATUS_OK;
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

t_status		ia32_pt_build(t_paddr			base,
				      t_ia32_table*		table)
{
  /*							   [block::pt_build] */

  assert(table != NULL);

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  table->paddr = base;
  table->vaddr = 0;

  /*							[endblock::pt_build] */

  return STATUS_OK;
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

t_status		ia32_pt_add_page(t_ia32_table*		tab,
					 t_uint16		entry,
					 t_ia32_page		page)
{
  /*							[block::pt_add_page] */

  t_ia32_pte*		t;
  t_uint32		opts = 0;

  assert(entry < IA32_PT_MAX_ENTRIES);

  /*
   * 1)
   */

  if (IA32_MK_BASE(page.addr) != (t_uint32)page.addr)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  assert(tab->vaddr != 0);
  t = (t_ia32_pte*)tab->vaddr;

  /*
   * 3)
   */

  if (page.present)
    opts |= IA32_PTE_FLAG_P;
  else
    printf ("warning: adding non-present page\n");

  if (page.cached == IA32_PG_NOTCACHED)
    opts |= IA32_PDE_FLAG_CD;
  if (page.writeback == IA32_PG_WRITETHROUGH)
    opts |= IA32_PDE_FLAG_WT;

  opts |= (page.rw == IA32_PG_WRITABLE ? IA32_PTE_FLAG_RW : IA32_PTE_FLAG_RO);
  opts |= (page.user == IA32_PG_USER ?
	   IA32_PTE_FLAG_USER : IA32_PTE_FLAG_SUPERVISOR);

  opts |= (page.global == IA32_PG_GLOBAL ? IA32_PTE_FLAG_G : 0);

  opts |= IA32_PTE_FLAG_USED;

  t[entry] = IA32_MK_BASE(page.addr) | opts;

  /*						     [endblock::pt_add_page] */

  return STATUS_OK;
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

t_status		ia32_pt_get_page(t_ia32_table*		tab,
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

  if (!(t[entry] & IA32_PTE_FLAG_USED))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3 )
   */

  page->rw = (t[entry] & IA32_PTE_FLAG_RW) ?
    IA32_PG_WRITABLE : IA32_PG_READONLY;
  page->present = !!(t[entry] & IA32_PTE_FLAG_P);
  page->user = (t[entry] & IA32_PTE_FLAG_USER) ?
    IA32_PG_USER : IA32_PG_PRIVILEGED;
  page->global = (t[entry] & IA32_PTE_FLAG_G) ?
    IA32_PG_GLOBAL : IA32_PG_NONGLOBAL;
  page->writeback = (t[entry] & IA32_PTE_FLAG_WT) ?
    IA32_PG_WRITETHROUGH : IA32_PG_WRITEBACK;
  page->cached = (t[entry] & IA32_PTE_FLAG_CD) ?
    IA32_PG_NOTCACHED : IA32_PG_CACHED;
  page->addr = IA32_MK_BASE(t[entry]);

  /*						     [endblock::pt_get_page] */

  return STATUS_OK;
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

t_status		ia32_pt_delete_page(t_ia32_table*	tab,
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

  if (!(t[entry] & IA32_PTE_FLAG_USED))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  t[entry] = 0;

  /*						  [endblock::pt_delete_page] */

  return STATUS_OK;
}
