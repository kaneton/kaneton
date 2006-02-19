/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/paging/pd.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:20 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:53:31 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage page directory.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * this    file   is    destinated   to    functions    dealing   with
 * page-directories. there are no restrictions about function names or
 * prototype, so feel free.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k2 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * active page directory.
 */

t_directory		pd;

/*                                                                 [cut] /k2 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * dumps a page directory and its tables.
 */

t_error			pd_dump(t_directory*			dir)
{
  t_uint32		i;
  t_pde*		d;

  if (!dir)
    d = pd;
  else
    d = *dir;

  for (i = 0; i < PD_MAX_ENTRIES; i++)
    {
      if (d[i] & PDE_FLAG_P)
	{
	  printf("entry %d\n", i);
	  pt_dump((t_pte*)(MK_BASE(d[i])));
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

t_error			pd_build(t_paddr			base,
				 t_directory*			directory,
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

  *directory = (t_directory)base;

  /*
   * 3)
   */

  if (clear)
    {
      memset((void*)base, 0, PD_MAX_ENTRIES * sizeof(t_pde));
    }

  return ERROR_NONE;
}

/*
 * gets the base of a pd.
 *
 */

t_error			pd_base(t_directory*			dir,
				t_paddr*			base)
{
  t_pde*		d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = pd;

  /*
   * 2)
   */

  *base = MK_BASE(d);

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

t_error			pd_activate(t_directory			dir)
{
  t_uint32		pdbr;

  /*
   * 1)
   */

  pdbr = ((t_uint32)dir & 0xfffffc18);

  /*
   * 2)
   */

  LCR3(pdbr);

  /*
   * 3)
   */

  pd = dir;

  return ERROR_UNKNOWN;
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

t_error			pd_add_table(t_directory*		dir,
				     t_uint16			entry,
				     t_table			table)
{
  t_pde*		d;
  t_uint32		opts = 0;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = pd;

  /*
   * 2)
   */

  if (table.present)
    opts |= PDE_FLAG_P;

  opts |= (table.rw ? PDE_FLAG_RW : PDE_FLAG_RO);

  opts |= (table.user ? PDE_FLAG_USER : PDE_FLAG_SUPERVISOR);

  opts |= PDE_FLAG_USED;

  /*
   * 3)
   */

  d[entry] = MK_BASE(table.entries) | opts;

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

t_error			pd_get_table(t_directory*		dir,
				     t_uint16			entry,
				     t_table*			table)
{
  t_directory		d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = pd;

  /*
   * 2)
   */

  if (!(d[entry] & PDE_FLAG_USED))
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  table->rw = !!(d[entry] & PDE_FLAG_RW);
  table->present = !!(d[entry] & PDE_FLAG_P);
  table->user = !!(d[entry] & PDE_FLAG_USER);
  table->entries = (void*)MK_BASE(d[entry]);

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

t_error			pd_delete_table(t_directory*		dir,
					t_uint16		entry)
{
  t_directory		d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = pd;

  /*
   * 2)
   */

  if (!(d[entry] & PDE_FLAG_USED))
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  d[entry] = 0;

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
