/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pde.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:56:20 2005]
 * updated       matthieu bucchianeri   [fri dec 30 19:39:26 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage page directory.
 *
 * XXX modifier pour gerer les pages de 4M ?
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

t_pde*	pd;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * reads the current pdbr and refresh the global variable pd.
 */

t_error			pd_refresh(void)
{
  t_uint32		pdbr;

  SCR3(pdbr);

  pd = (t_pde*)(MK_BASE(pdbr));

  return ERROR_NONE;
}

/*
 * XXX
 */

t_error			pd_dump(t_directory*	dir)
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
 */

t_error			pd_build(t_paddr	base,
				 t_directory*	directory,
				 t_uint8	clear)
{
  if (MK_BASE(base) != base)
    return ERROR_UNKNOWN;

  *directory = (t_directory)base;

  if (clear)
    {
      memset((void*)base, 0, PD_MAX_ENTRIES * sizeof(t_pde));
    }

  return ERROR_NONE;
}

/*
 * XXX
 */

t_error			pd_activate(t_directory		dir)
{
  t_uint32		pdbr;

  pdbr = ((t_uint32)dir & 0xfffffc18);

  LCR3(pdbr);

  pd = dir;

  return ERROR_UNKNOWN;
}

/*
 * XXX
 */

t_error			pd_add_table(t_directory*	dir,
				     t_uint16		entry,
				     t_table		table)
{
  t_pde*		d;
  t_uint32		opts = 0;

  if (dir)
    d = *dir;
  else
    d = pd;

  if (table.present)
    opts |= PDE_FLAG_P;

  opts |= (table.rw ? PDE_FLAG_RW : PDE_FLAG_RO);

  opts |= (table.user ? PDE_FLAG_USER : PDE_FLAG_SUPERVISOR);

  d[entry] = MK_BASE(table.entries) | opts;

  return ERROR_NONE;
}

/*
 * XXX
 */

t_error			pd_get_table(t_directory*	dir,
				     t_uint16		entry,
				     t_table*		table)
{
  return ERROR_UNKNOWN;
}

/*
 * XXX
 */

t_error			pd_delete_table(t_directory*	dir,
					t_uint16	entry)
{
  return ERROR_UNKNOWN;
}

/*                                                                 [cut] /k2 */
