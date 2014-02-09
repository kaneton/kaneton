/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/pd.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:20 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:34:11 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage page directory.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * active page directory.
 */

t_ia32_directory	ia32_pd;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dumps a page directory and its tables.
 */

t_status		ia32_pd_dump(t_ia32_directory*		dir)
{

  /*							    [block::pd_dump] */

  t_uint32		i;
  t_ia32_pde*		d;

  if (dir == IA32_PD_CURRENT)
    d = ia32_pd;
  else
    d = *dir;

  for (i = 0; i < IA32_PD_MAX_ENTRIES; i++)
    {
      if (d[i] & IA32_PDE_FLAG_P)
	{
	  printf("entry %d\n", i);
	  ia32_pt_dump((t_ia32_pte*)(IA32_MK_BASE(d[i])));
	}
    }
  /*							 [endblock::pd_dump] */

  return STATUS_OK;
}

/*							   [block::pd_build] */

/*
 * builds a new page directory.
 *
 * steps:
 *
 * 1) checks address alignement.
 * 2) fills the record.
 */

t_status		ia32_pd_build(t_paddr			base,
				      t_ia32_directory*		directory)
{
  assert(directory != NULL);

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  *directory = (t_ia32_directory)base;

  return STATUS_OK;
}

/*							[endblock::pd_build] */

/*							    [block::pd_base] */

/*
 * gets the base of a pd.
 *
 */

t_status		ia32_pd_base(t_ia32_directory*		dir,
				     t_paddr*			base)
{
  t_ia32_pde*		d;

  assert(base != NULL);

  /*
   * 1)
   */

  if (dir != IA32_PD_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  *base = IA32_MK_BASE(d);

  return STATUS_OK;
}

/*							 [endblock::pd_base] */

/*
 * activates a directory.
 *						  [block::pd_activate::comment]
 *
 * steps:
 *
 * 1) computes the pdbr value.
 * 2) loads the pdbr.
 * 3) sets the global variable.
 *					       [endblock::pd_activate::comment]
 */

t_status		ia32_pd_activate(t_ia32_directory	dir,
					 t_uint32		cached,
					 t_uint32		writeback)
{
  /*							[block::pd_activate] */
  t_uint32		pdbr;
  t_uint32		mask = 0xfffff000;

  assert(cached == IA32_PD_CACHED || cached == IA32_PD_NOTCACHED);
  assert(writeback == IA32_PD_WRITEBACK || writeback == IA32_PD_WRITETHROUGH);

  /*
   * 1)
   */

  if (cached == IA32_PD_NOTCACHED)
    mask |= (1 << 4);
  if (writeback == IA32_PD_WRITETHROUGH)
    mask |= (1 << 3);

  pdbr = ((t_uint32)dir & mask);

  /*
   * 2)
   */

  LCR3(pdbr);

  /*
   * 3)
   */

  ia32_pd = dir;

  /*						     [endblock::pd_activate] */

  return STATUS_OK;
}

/*
 * get the page-directory base register corresponding to a page-directory.
 */

t_status		ia32_pd_get_cr3(t_uint32*		cr3,
					t_ia32_directory	dir,
					t_uint32		cached,
					t_uint32		writeback)
{
  /*							 [block::pd_get_cr3] */
  t_uint32		mask = 0xfffff000;

  assert(cr3 != NULL);
  assert(cached == IA32_PD_CACHED || cached == IA32_PD_NOTCACHED);
  assert(writeback == IA32_PD_WRITEBACK || writeback == IA32_PD_WRITETHROUGH);

  if (cached == IA32_PD_NOTCACHED)
    mask |= (1 << 4);
  if (writeback == IA32_PD_WRITETHROUGH)
    mask |= (1 << 3);

  *cr3 = ((t_uint32)dir & mask);

  /*						      [endblock::pd_get_cr3] */

  return (STATUS_OK);
}

/*
 * adds a table to a directory.
 *						 [block::pd_add_table::comment]
 *
 * steps:
 *
 * 1) gets the directory address.
 * 2) setups the entry.
 * 3) adds the entry.
 *					      [endblock::pd_add_table::comment]
 */


t_status		ia32_pd_add_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table		table)
{
  /*						       [block::pd_add_table] */

  t_ia32_pde*		d;
  t_uint32		opts = 0;

  assert(entry < IA32_PD_MAX_ENTRIES);

  /*
   * 1)
   */

  if (dir != IA32_PD_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (table.present)
    opts |= IA32_PDE_FLAG_P;

  if (table.cached == IA32_PT_NOTCACHED)
    opts |= IA32_PDE_FLAG_CD;
  if (table.writeback == IA32_PT_WRITETHROUGH)
    opts |= IA32_PDE_FLAG_WT;

  opts |= (table.rw == IA32_PT_WRITABLE ? IA32_PDE_FLAG_RW : IA32_PDE_FLAG_RO);

  opts |= (table.user == IA32_PT_USER ? IA32_PDE_FLAG_USER : IA32_PDE_FLAG_SUPERVISOR);

  opts |= IA32_PDE_FLAG_USED;

  /*
   * 3)
   */

  d[entry] = IA32_MK_BASE(table.paddr) | opts;

  /*						    [endblock::pd_add_table] */

  return STATUS_OK;
}

/*
 * gets a table entry from a directory.
 *						 [block::pd_get_table::comment]
 *
 * steps:
 *
 * 1) gets the directory address.
 * 2) checks entry validity.
 * 3) fills the page record.
 *					      [endblock::pd_get_table::comment]
 */

t_status		ia32_pd_get_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table*		table)
{
  /*						       [block::pd_get_table] */

  t_ia32_directory	d;

  assert(table != NULL);

  /*
   * 1)
   */

  if (dir != IA32_PD_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PDE_FLAG_USED))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  table->rw = (d[entry] & IA32_PDE_FLAG_RW) ?
    IA32_PT_WRITABLE : IA32_PT_READONLY;
  table->present = !!(d[entry] & IA32_PDE_FLAG_P);
  table->user = (d[entry] & IA32_PDE_FLAG_USER) ?
    IA32_PT_USER : IA32_PT_PRIVILEGED;
  table->writeback = (d[entry] & IA32_PDE_FLAG_WT) ?
    IA32_PT_WRITETHROUGH : IA32_PT_WRITEBACK;
  table->cached = (d[entry] & IA32_PDE_FLAG_CD) ?
    IA32_PT_NOTCACHED : IA32_PT_CACHED;
  table->paddr = IA32_MK_BASE(d[entry]);

  /*						    [endblock::pd_get_table] */

  return STATUS_OK;
}

/*
 * deletes an entry.
 *					      [block::pd_delete_table::comment]
 *
 * steps:
 *
 * 1) gets the directory pointer.
 * 2) checks if the entry is a valid one.
 * 3) resets the entry.
 *					   [endblock::pd_delete_table::comment]
 */

t_status		ia32_pd_delete_table(t_ia32_directory*	dir,
					     t_uint16		entry)
{
  /*						    [block::pd_delete_table] */

  t_ia32_directory	d;

  /*
   * 1)
   */

  if (dir != IA32_PD_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PDE_FLAG_USED))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  d[entry] = 0;

  /*						 [endblock::pd_delete_table] */

  return STATUS_OK;
}
