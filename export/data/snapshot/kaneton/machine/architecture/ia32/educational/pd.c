/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/architecture/ia32/educational/pd.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:20 2005]
 * updated       julien quintard   [sat feb  5 15:04:19 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for managing PDs - Page Directories.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function builds a mapped page directory.
 *
 * steps:
 *
 * 1) initialize the page directory's entries making them all non-present.
 */

t_error			architecture_pd_build(at_pd		pd)
{
  /*
   * 1)
   */

  memset(pd, 0x0, ___kaneton$pagesz);

  MACHINE_LEAVE();
}

/*
 * this function inserts a page table reference in the given page directory.
 */

t_error			architecture_pd_insert(at_pd		pd,
					       at_pdei		index,
					       t_paddr		address,
					       t_flags		flags)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}

/*
 * this function deletes a page directory entry by resetting its memory.
 */

t_error			architecture_pd_delete(at_pd		pd,
					       at_pdei		index)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}

/*
 * this function maps a page directory in the kernel's address space, taking
 * care to register the page used to map it in the region manager.
 */

t_error			architecture_pd_map(t_paddr		paddr,
					    at_pd*		table)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}

/*
 * this function unmaps the given page directory.
 */

t_error			architecture_pd_unmap(at_pd		table)
{
  /* FIXME[complete the code] */

  MACHINE_LEAVE();
}
