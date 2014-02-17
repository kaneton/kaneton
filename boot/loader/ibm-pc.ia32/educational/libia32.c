/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...loader/ibm-pc.ia32/educational/libia32.c
 *
 * created       matthieu bucchianeri   [mon jun 25 22:52:22 2007]
 * updated       julien quintard   [thu apr  7 15:37:37 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "loader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * gdt table pointer
 */

t_ia32_gdt	ia32_gdt;

/*
 * active page directory.
 */

t_ia32_directory	ia32_pd;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * sets code/data/stack segment registers.
 */

t_status		pmode_set_segment_registers(t_uint16	seg_code,
					    t_uint16	seg_data)
{
  asm volatile("pushl %0\n\t"
	       "pushl $1f\n\t"
	       "lret\n\t"
	       "1:\n\t"
	       "movw %1, %%ax\n\t"
	       "movw %%ax, %%ds\n\t"
	       "movw %%ax, %%ss\n\t"
	       "movw %%ax, %%es\n\t"
	       "movw %%ax, %%fs\n\t"
	       "movw %%ax, %%gs\n\t"
	       :
	       : "g" (seg_code), "g" (seg_data)
	       : "memory", "%eax"
	       );

  return STATUS_OK;
}

/*
 * builds a global descriptor table with specified number of entries.
 *
 * steps:
 *
 * 1) checks the number of entries.
 * 2) aligns the address (for best performances).
 * 3) builds the gdt structure.
 * 4) clears the table if necessary.
 */

t_status			gdt_build(t_uint16		entries,
				  t_paddr		base,
				  t_ia32_gdt*		gdt,
				  t_uint8		clear)
{

  /*
   * 1)
   */

  if (entries > IA32_GDT_MAX_ENTRIES)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (base % sizeof(t_ia32_gdte))
    {
      base += sizeof(t_ia32_gdte) - (base % sizeof(t_ia32_gdte));
    }

  /*
   * 3)
   */

  gdt->descriptor = (t_ia32_gdte*)base;
  gdt->count = entries;

  /*
   * 4)
   */

  if (clear)
    {
      memset(gdt->descriptor, 0, entries * sizeof(t_ia32_gdte));
    }

  return STATUS_OK;
}

/*
 * activates a gdt.
 *
 * steps:
 *
 * 1) builds and loads the new gdt register.
 * 2) updates global gdt record.
 */

t_status			gdt_activate(t_ia32_gdt		new_gdt)
{
  t_ia32_gdtr		gdtr;

  /*
   * 1)
   */

  gdtr.address = (t_paddr)new_gdt.descriptor;
  gdtr.size = new_gdt.count * sizeof(t_ia32_gdte);
  LGDT(gdtr);

  /*
   * 2)
   */

  ia32_gdt.descriptor = new_gdt.descriptor;
  ia32_gdt.count = new_gdt.count;

  return STATUS_OK;
}

/*
 * adds a segment, erasing previous segment present.
 *
 * steps:
 *
 * 1) retrieves the global pointer if needed.
 * 2) checks the size of the table.
 * 3) sets the type field.
 * 4) sets the base field.
 * 5) sets the flags.
 * 6) sets the limit field.
 */

t_status			gdt_add_segment(t_ia32_gdt*	table,
					t_uint16	segment,
					t_ia32_segment	descriptor)
{
  t_uint32		size;

  /*
   * 1)
   */

  if (!table)
    table = &ia32_gdt;

  /*
   * 2)
   */

  if (segment >= table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  table->descriptor[segment].type = IA32_DESC_TYPE_PRESENT |
    IA32_DESC_MK_DPL(descriptor.privilege);
  if (descriptor.is_system)
    table->descriptor[segment].type |= descriptor.type.sys;
  else
    table->descriptor[segment].type |= IA32_GDT_TYPE_S | descriptor.type.usr;

  /*
   * 4)
   */

  table->descriptor[segment].base_00_15 = (t_uint16)descriptor.base;
  table->descriptor[segment].base_16_23 = (descriptor.base >> 16);
  table->descriptor[segment].base_24_31 = (descriptor.base >> 24);

  /*
   * 5)
   */

  table->descriptor[segment].flags = IA32_GDT_FLAG_USE32;

  if (descriptor.limit >= 4096)
    {
      table->descriptor[segment].flags |= IA32_GDT_FLAG_GRANULAR;
      size = descriptor.limit / 4096;
    }
  else
    {
      size = descriptor.limit;
    }

  /*
   * 6)
   */

  table->descriptor[segment].limit_00_15 = (t_uint16)size;
  table->descriptor[segment].limit_16_19 = size >> 16;

  return STATUS_OK;
}

/*
 * builds a selector with specified segment and privileges.
 *
 * steps:
 *
 * 1) checks segment entry boundary.
 * 2) builds the selector.
 */

t_status			gdt_build_selector(t_uint16	segment,
					   t_ia32_prvl	privilege,
					   t_uint16*	selector)
{

  /*
   * 1)
   */

  if (segment >= ia32_gdt.count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  *selector = IA32_SEGSEL_TI_GDT | IA32_SEGSEL_MK_RPL(privilege) |
    IA32_SEGSEL_MK_INDEX(segment);

  return STATUS_OK;
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

t_status			pd_build(t_paddr			base,
				 t_ia32_directory*		directory,
				 t_uint8			clear)
{

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return STATUS_UNKNOWN_ERROR;

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

  return STATUS_OK;
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

t_status			pd_activate(t_ia32_directory	dir,
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

  return STATUS_OK;
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


t_status			pd_add_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table	table)
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

  /*
   * 3)
   */

  d[entry] = IA32_MK_BASE(table.entries) | opts;

  return STATUS_OK;
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

t_status			pd_get_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table*	table)
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

  if (!(d[entry] & IA32_PDE_FLAG_P))
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
  table->entries = IA32_MK_BASE(d[entry]);

  return STATUS_OK;
}

/*
 * builds a new page table.
 *
 * steps:
 *
 * 1) checks address encoding.
 * 2) initializes the structure.
 * 3) clears the table if necessary.
 */

t_status			pt_build(t_paddr		base,
				 t_ia32_table*		table,
				 t_uint8		clear)
{

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  table->entries = base;

  /*
   * 3)
   */

  if (clear)
    {
      memset((void*)base, 0, IA32_PT_MAX_ENTRIES * sizeof(t_ia32_pte));
    }

  return STATUS_OK;
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

t_status			pt_add_page(t_ia32_table*	tab,
				    t_uint16		entry,
				    t_ia32_page		page)
{
  t_ia32_pte*		t;
  t_uint32		opts = 0;

  /*
   * 1)
   */

  if (IA32_MK_BASE(page.addr) != (t_uint32)page.addr)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  t = (t_ia32_pte*)tab->entries;

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

  t[entry] = IA32_MK_BASE(page.addr) | opts;

  return STATUS_OK;
}
