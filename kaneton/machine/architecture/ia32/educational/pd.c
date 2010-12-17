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
 * ---------- XXX -------------------------------------------------------------
 */

extern m_kernel*	_kernel;

// XXX rajouter option qui reserve ou non une region
t_error			XXX_ia32_page_map(t_paddr		paddr,
					  t_vaddr*		vaddr)
{
  void*			region;

  region = malloc(sizeof(o_region));

  if (region_space(_kernel->as, ___kaneton$pagesz, vaddr) != ERROR_OK) // XXX kasid?
    return (ERROR_KO);

  if (ia32_map_chunk(*vaddr, paddr, region) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}

t_error			XXX_ia32_page_unmap(t_vaddr		vaddr)
{
  return (ia32_unmap_chunk(vaddr)); // XXX
}

t_error			XXX_ia32_directory_dump(t_paddr		paddr)
{
  t_uint32		i;
  t_vaddr		vaddr;
  t_ia32_directory	directory;

  if (XXX_ia32_page_map(paddr, &vaddr) != ERROR_OK)
    return (ERROR_KO);

  module_call(console, print,
	      "[page directory] paddr=0x%08x, vaddr=0x%08x\n",
	      paddr, vaddr);

  directory = (t_ia32_directory)vaddr;

  for (i = 0; i < IA32_PAGE_DIRECTORY_MAX_ENTRIES; i++)
    {
      if (directory[i] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_P)
	XXX_ia32_table_dump(directory, i);
    }

  if (XXX_ia32_page_unmap(vaddr) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}

t_error			XXX_ia32_table_dump(t_ia32_directory	directory,
					    t_uint32		index)
{
  t_paddr		paddr;
  t_vaddr		vaddr;
  t_uint32		i;
  t_ia32_pte*		table; // XXX should be t_ia32_table

  paddr = IA32_BASE(directory[index]);

  if (XXX_ia32_page_map(paddr, &vaddr) != ERROR_OK)
    return (ERROR_KO);

  module_call(console, print,
	      "  [page table %4u] paddr=0x%08x, vaddr=0x%08x range=[0x%x - 0x%x]\n",
	 index,
	 paddr, vaddr,
	 index * IA32_PAGE_DIRECTORY_MAX_ENTRIES * ___kaneton$pagesz,
	 (index + 1) * IA32_PAGE_DIRECTORY_MAX_ENTRIES * ___kaneton$pagesz - 1);

  table = (t_ia32_pte*)vaddr;

  for (i = 0; i < IA32_PAGE_TABLE_MAX_ENTRIES; i++)
    {
      if (table[i] & IA32_PAGE_TABLE_ENTRY_FLAG_USED)
	{
	  module_call(console, print,
		      "    [page table entry %4u] "
		      "vaddr=0x%08x, paddr=0x%08x, r/w=%d, user/supervisor=%d, accessed=%d dirty=%d\n",
		 i,
		 index * IA32_PAGE_TABLE_MAX_ENTRIES * ___kaneton$pagesz + i * ___kaneton$pagesz,
		 IA32_BASE(table[i]),
		 !!(table[i] & IA32_PAGE_TABLE_ENTRY_FLAG_RW),
		 !!(table[i] & IA32_PAGE_TABLE_ENTRY_FLAG_USER),
		 !!(table[i] & IA32_PAGE_TABLE_ENTRY_FLAG_A),
		 !!(table[i] & IA32_PAGE_TABLE_ENTRY_FLAG_D));
	}
    }

  if (XXX_ia32_page_unmap(vaddr) != ERROR_OK)
    return (ERROR_KO);

  return (ERROR_OK);
}

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dumps a page directory and its tables.
 */

t_error			ia32_pd_dump(t_ia32_directory*		dir)
{

  /*							    [block::pd_dump] */

  t_uint32		i;
  t_ia32_pde*		d;

  if (dir == IA32_PAGE_DIRECTORY_CURRENT)
    d = ia32_pd;
  else
    d = *dir;

  for (i = 0; i < IA32_PAGE_DIRECTORY_MAX_ENTRIES; i++)
    {
      if (d[i] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_P)
	{
	  module_call(console, print,
		      "entry %d\n", i);

	  ia32_pt_dump((t_ia32_pte*)(IA32_BASE(d[i])),
		       i);
	}
    }

  /*							 [endblock::pd_dump] */

  return ERROR_OK;
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

t_error			ia32_pd_build(t_paddr			base,
				      t_ia32_directory*		directory)
{
  assert(directory != NULL);

  /*
   * 1)
   */

  if (IA32_BASE(base) != base)
    return ERROR_KO;

  /*
   * 2)
   */

  *directory = (t_ia32_directory)base;

  return ERROR_OK;
}

/*							[endblock::pd_build] */

/*							    [block::pd_base] */

/*
 * gets the base of a pd.
 *
 */

t_error			ia32_pd_base(t_ia32_directory*		dir,
				     t_paddr*			base)
{
  t_ia32_pde*		d;

  assert(base != NULL);

  /*
   * 1)
   */

  if (dir != IA32_PAGE_DIRECTORY_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  *base = IA32_BASE(d);

  return ERROR_OK;
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

t_error			ia32_pd_activate(t_ia32_directory	dir,
					 t_uint32		cached,
					 t_uint32		writeback)
{
  /*							[block::pd_activate] */
  t_uint32		pdbr;
  t_uint32		mask = 0xfffff000;

  assert(cached == IA32_PAGE_DIRECTORY_CACHED ||
	 cached == IA32_PAGE_DIRECTORY_NOTCACHED);
  assert(writeback == IA32_PAGE_DIRECTORY_WRITEBACK ||
	 writeback == IA32_PAGE_DIRECTORY_WRITETHROUGH);

  /*
   * 1)
   */

  if (cached == IA32_PAGE_DIRECTORY_NOTCACHED)
    mask |= (1 << 4);
  if (writeback == IA32_PAGE_DIRECTORY_WRITETHROUGH)
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

  return ERROR_OK;
}

/*
 * get the page-directory base register corresponding to a page-directory.
 */

t_error			ia32_pd_get_cr3(t_uint32*		cr3,
					t_ia32_directory	dir,
					t_uint32		cached,
					t_uint32		writeback)
{
  /*							 [block::pd_get_cr3] */
  t_uint32		mask = 0xfffff000;

  assert(cr3 != NULL);
  assert(cached == IA32_PAGE_DIRECTORY_CACHED ||
	 cached == IA32_PAGE_DIRECTORY_NOTCACHED);
  assert(writeback == IA32_PAGE_DIRECTORY_WRITEBACK ||
	 writeback == IA32_PAGE_DIRECTORY_WRITETHROUGH);

  if (cached == IA32_PAGE_DIRECTORY_NOTCACHED)
    mask |= (1 << 4);
  if (writeback == IA32_PAGE_DIRECTORY_WRITETHROUGH)
    mask |= (1 << 3);

  *cr3 = ((t_uint32)dir & mask);

  /*						      [endblock::pd_get_cr3] */

  return (ERROR_OK);
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


t_error			ia32_pd_add_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table		table)
{
  /*						       [block::pd_add_table] */

  t_ia32_pde*		d;
  t_uint32		opts = 0;

  assert(entry < IA32_PAGE_DIRECTORY_MAX_ENTRIES);

  /*
   * 1)
   */

  if (dir != IA32_PAGE_DIRECTORY_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (table.present)
    opts |= IA32_PAGE_DIRECTORY_ENTRY_FLAG_P;

  if (table.cached == IA32_PAGE_TABLE_NOTCACHED)
    opts |= IA32_PAGE_DIRECTORY_ENTRY_FLAG_CD;
  if (table.writeback == IA32_PAGE_TABLE_WRITETHROUGH)
    opts |= IA32_PAGE_DIRECTORY_ENTRY_FLAG_WT;

  opts |= (table.rw == IA32_PAGE_TABLE_WRITABLE ?
	   IA32_PAGE_DIRECTORY_ENTRY_FLAG_RW :
	   IA32_PAGE_DIRECTORY_ENTRY_FLAG_RO);

  opts |= (table.user == IA32_PAGE_TABLE_USER ?
	   IA32_PAGE_DIRECTORY_ENTRY_FLAG_USER :
	   IA32_PAGE_DIRECTORY_ENTRY_FLAG_SUPERVISOR);

  opts |= IA32_PAGE_DIRECTORY_ENTRY_FLAG_USED;

  /*
   * 3)
   */

  d[entry] = IA32_BASE(table.paddr) | opts;

  /*						    [endblock::pd_add_table] */

  return ERROR_OK;
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

t_error			ia32_pd_get_table(t_ia32_directory*	dir,
					  t_uint16		entry,
					  t_ia32_table*		table)
{
  /*						       [block::pd_get_table] */

  t_ia32_directory	d;

  assert(table != NULL);

  /*
   * 1)
   */

  if (dir != IA32_PAGE_DIRECTORY_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_USED))
    return ERROR_KO;

  /*
   * 3)
   */

  table->rw = (d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_RW) ?
    IA32_PAGE_TABLE_WRITABLE : IA32_PAGE_TABLE_READONLY;
  table->present = !!(d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_P);
  table->user = (d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_USER) ?
    IA32_PAGE_TABLE_USER : IA32_PAGE_TABLE_PRIVILEGED;
  table->writeback = (d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_WT) ?
    IA32_PAGE_TABLE_WRITETHROUGH : IA32_PAGE_TABLE_WRITEBACK;
  table->cached = (d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_CD) ?
    IA32_PAGE_TABLE_NOTCACHED : IA32_PAGE_TABLE_CACHED;
  table->paddr = IA32_BASE(d[entry]);

  /*						    [endblock::pd_get_table] */

  return ERROR_OK;
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

t_error			ia32_pd_delete_table(t_ia32_directory*	dir,
					     t_uint16		entry)
{
  /*						    [block::pd_delete_table] */

  t_ia32_directory	d;

  /*
   * 1)
   */

  if (dir != IA32_PAGE_DIRECTORY_CURRENT)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PAGE_DIRECTORY_ENTRY_FLAG_USED))
    return ERROR_KO;

  /*
   * 3)
   */

  d[entry] = 0;

  /*						 [endblock::pd_delete_table] */

  return ERROR_OK;
}
