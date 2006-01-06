/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pte.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:48 2005]
 * updated       matthieu bucchianeri   [fri dec 30 20:11:10 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage page tables.
 *
 * XXX voir ce qu'il y a de sympa a optimiser avec le flag global
 *
 * XXX gerer les pages en swap
 */

/*
 * ---------- assignments -----------------------------------------------------
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * XXX
 */

t_error			pt_dump(t_pte*	tab)
{
  t_uint32		i;

  for (i = 0; i < PT_MAX_ENTRIES; i++)
    {
      if (tab[i] & PTE_FLAG_P)
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
 */

t_error			pt_build(t_paddr	base,
				 t_table*	table,
				 t_uint8	clear)
{
  if (MK_BASE(base) != base)
    return ERROR_UNKNOWN;

  table->entries = (void*)base;

  if (clear)
    {
      memset((void*)base, 0, PT_MAX_ENTRIES * sizeof(t_pte));
    }

  return ERROR_NONE;
}

/*
 * XXX
 */

t_error			pt_add_page(t_table*	tab,
				    t_uint16	entry,
				    t_page	page)
{
  t_pte*		t;
  t_uint32		opts = 0;

  if (MK_BASE(page.addr) != (t_uint32)page.addr)
    return ERROR_UNKNOWN;

  t = tab->entries;

  if (page.present)
    opts |= PTE_FLAG_P;

  opts |= (page.rw ? PTE_FLAG_RW : PTE_FLAG_RO);

  opts |= (page.user ? PTE_FLAG_USER : PTE_FLAG_SUPERVISOR);

  // XXX opts |= PTE_FLAG_WT;

  t[entry] = MK_BASE(page.addr) | opts;

  return ERROR_NONE;
}

/*
 * XXX
 */

t_error			pt_get_page(t_table*	tab,
				    t_uint16	entry,
				    t_page*	page)
{
  return ERROR_UNKNOWN;
}

/*
 * XXX
 */

t_error			pt_delete_page(t_table*	tab,
				       t_uint16	entry)
{
  return ERROR_UNKNOWN;
}

/*                                                                 [cut] /k2 */
