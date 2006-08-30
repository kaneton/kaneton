/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/pt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:48 2005]
 * updated       matthieu bucchianeri   [wed aug 30 17:10:20 2006]
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

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k2 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dumps a page table.
 */

t_error			pt_dump(t_ia32_pte*			tab)
{
  t_uint32		i;

  for (i = 0; i < PT_MAX_ENTRIES; i++)
    {
      if (tab[i] & PTE_FLAG_USED)
	{
	  printf(" sub-entry %d: ", i);
	  printf("base %p, rw %d, usr %d, a %d d %d\n", MK_BASE(tab[i]),
		 !!(tab[i] & PTE_FLAG_RW), !!(tab[i] & PTE_FLAG_USER),
		 !!(tab[i] & PTE_FLAG_A), !!(tab[i] & PTE_FLAG_D));
	}
    }

  return ERROR_NONE;
}

/*
 * builds a new page table.
 *
 * steps:
 *
 * 1) checks address encoding.
 * 2) initialises the structure.
 * 3) clears the table if necessary.
 */

t_error			pt_build(t_paddr			base,
				 t_ia32_table*			table,
				 t_uint8			clear)
{

  /*
   * 1)
   */

  if (MK_BASE(base) != base)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  table->entries = base;

  /*
   * 3)
   */

  if (clear)
    {
      memset((void*)base, 0, PT_MAX_ENTRIES * sizeof(t_ia32_pte));
    }

  return ERROR_NONE;
}

/*
 * adds a page to a table.
 *
 * steps:
 *
 * 1) checks the address of the page.
 * 2) gets the table pointer.
 * 3) setups the entry.
 */

t_error			pt_add_page(t_ia32_table*		tab,
				    t_uint16			entry,
				    t_ia32_page			page)
{
  t_ia32_pte*		t;
  t_uint32		opts = 0;

  /*
   * 1)
   */

  if (MK_BASE(page.addr) != (t_uint32)page.addr)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  t = (t_ia32_pte*)tab->entries;

  /*
   * 3)
   */

  if (page.present)
    opts |= PTE_FLAG_P;
  else
    printf ("warning: adding non-present page\n");

  if (page.cached == PG_NOTCACHED)
    opts |= PDE_FLAG_CD;
  if (page.writeback == PG_WRITETHROUGH)
    opts |= PDE_FLAG_WT;

  opts |= (page.rw == PG_WRITABLE ? PTE_FLAG_RW : PTE_FLAG_RO);

  opts |= (page.user == PG_USER ? PTE_FLAG_USER : PTE_FLAG_SUPERVISOR);

  opts |= (page.global == PG_GLOBAL ? PTE_FLAG_G : 0);

  opts |= PTE_FLAG_USED;

  t[entry] = MK_BASE(page.addr) | opts;

  return ERROR_NONE;
}

/*
 * gets an entry in a table.
 *
 * steps:
 *
 * 1) gets the table.
 * 2) checks whether an entry is valid or not.
 * 3) fills the page structure.
 */

t_error			pt_get_page(t_ia32_table*		tab,
				    t_uint16			entry,
				    t_ia32_page*		page)
{
  t_ia32_pte*		t;

  /*
   * 1)
   */

  t = (t_ia32_pte*)tab->entries;

  /*
   * 2)
   */

  if (!(t[entry] & PTE_FLAG_USED))
    return ERROR_UNKNOWN;

  /*
   * 3 )
   */

  page->rw = (t[entry] & PTE_FLAG_RW) ? PG_WRITABLE : PG_READONLY;
  page->present = !!(t[entry] & PTE_FLAG_P);
  page->user = (t[entry] & PTE_FLAG_USER) ? PG_USER : PG_PRIVILEGED;
  page->global = (t[entry] & PTE_FLAG_G) ? PG_GLOBAL : PG_NONGLOBAL;
  page->writeback = (t[entry] & PTE_FLAG_WT) ? PG_WRITETHROUGH : PG_WRITEBACK;
  page->cached = (t[entry] & PTE_FLAG_CD) ? PG_NOTCACHED : PG_CACHED;
  page->addr = MK_BASE(t[entry]);

  return ERROR_NONE;
}

/*
 * deletes a page entry.
 *
 * steps:
 *
 * 1) gets the table.
 * 2) checks whether an entry is valid or not.
 * 3) resets the entry.
 */

t_error			pt_delete_page(t_ia32_table*		tab,
				       t_uint16			entry)
{
  t_ia32_pte*		t;

  /*
   * 1)
   */

  t = (t_ia32_pte*)tab->entries;

  /*
   * 2)
   */

  if (!(t[entry] & PTE_FLAG_USED))
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  t[entry] = 0;

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
