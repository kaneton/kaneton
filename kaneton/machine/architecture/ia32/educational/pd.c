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

#include <libc.h>
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

t_error			ia32_pd_dump(t_ia32_directory*		dir)
{
  t_uint32		i;
  t_ia32_pde*		d;

  if (!dir)
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

  return ERROR_NONE;
}

/*
 * builds a new page directory.
 *
 * steps:
 *
 * 1) checks address alignement.
 * 2) fills the record.
 * 3) clears the directory if needed.
 */

t_error			ia32_pd_build(t_paddr			base,
				      t_ia32_directory*		directory,
				      t_uint8			clear)
{

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  *directory = (t_ia32_directory)base;

  /*
   * 3)
   */

  if (clear)
    {
      memset((void*)base, 0, IA32_PD_MAX_ENTRIES * sizeof(t_ia32_pde));
    }

  return ERROR_NONE;
}

/*
 * gets the base of a pd.
 *
 */

t_error			ia32_pd_base(t_ia32_directory*		dir,
				     t_paddr*			base)
{
  t_ia32_pde*		d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  *base = IA32_MK_BASE(d);

  return ERROR_NONE;
}

/*
 * activates a directory.
 *
 * steps:
 *
 * 1) computes the pdbr value.
 * 2) loads the pdbr.
 * 3) sets the global variable.
 */

t_error			ia32_pd_activate(t_ia32_directory	dir,
					 t_uint32		cached,
					 t_uint32		writeback)
{
  t_uint32		pdbr;
  t_uint32		mask = 0xfffff000;

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

  return ERROR_NONE;
}

/*
 * get the page-directory base register corresponding to a page-directory.
 */

t_error			ia32_pd_get_cr3(t_uint32*		cr3,
					t_ia32_directory	dir,
					t_uint32		cached,
					t_uint32		writeback)
{
  t_uint32		mask = 0xfffff000;

  if (cached == IA32_PD_NOTCACHED)
    mask |= (1 << 4);
  if (writeback == IA32_PD_WRITETHROUGH)
    mask |= (1 << 3);

  *cr3 = ((t_uint32)dir & mask);

  return (ERROR_NONE);
}

/*
 * adds a table to a directory.
 *
 * steps:
 *
 * 1) gets the directory address.
 * 2) setups the entry.
 * 3) adds the entry.
 */


t_error			ia32_pd_add_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table		table)
{
  t_ia32_pde*		d;
  t_uint32		opts = 0;

  /*
   * 1)
   */

  if (dir)
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

  d[entry] = IA32_MK_BASE(table.entries) | opts;

  return ERROR_NONE;
}

/*
 * gets a table entry from a directory.
 *
 * steps:
 *
 * 1) gets the directory address.
 * 2) checks entry validity.
 * 3) fills the page record.
 */

t_error			ia32_pd_get_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table*		table)
{
  t_ia32_directory	d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PDE_FLAG_USED))
    return ERROR_UNKNOWN;

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
  table->entries = IA32_MK_BASE(d[entry]);

  return ERROR_NONE;
}

/*
 * deletes an entry.
 *
 * steps:
 *
 * 1) gets the directory pointer.
 * 2) checks if the entry is a valid one.
 * 3) resets the entry.
 */

t_error			ia32_pd_delete_table(t_ia32_directory*	dir,
					     t_uint16		entry)
{
  t_ia32_directory	d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PDE_FLAG_USED))
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  d[entry] = 0;

  return ERROR_NONE;
}
