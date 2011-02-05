/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/architecture/ia32/educational/pt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:48 2005]
 * updated       julien quintard   [sat feb  5 15:04:58 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for manipulating PTs - Page Tables.
 *
 * note that the whole file is extremely similar to pd.c.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function builds a page table in the page given by the
 * virtual address.
 *
 * steps:
 *
 * 1) initialize the page table's entries making them all non-present.
 */

t_error			architecture_pt_build(at_pt		pt)
{
  /*
   * 1)
   */

  memset(pt, 0x0, ___kaneton$pagesz);

  MACHINE_LEAVE();
}

/*
 * this function inserts a mapping relation in the given page table so
 * that the 'address' page gets maps through the page table entry 'index'.
 */

t_error			architecture_pt_insert(at_pt		pt,
					       at_ptei		index,
					       t_paddr		address,
					       t_flags		flags)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}

/*
 * this function deletes the page table entry by resetting its memory.
 */

t_error			architecture_pt_delete(at_pt		pt,
					       at_ptei		index)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}

/*
 * this function maps a page table located at the given physical address.
 */

t_error			architecture_pt_map(t_paddr		paddr,
					    at_pt*		table)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}

/*
 * this function unmaps the given page table.
 */

t_error			architecture_pt_unmap(at_pt		table)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}
