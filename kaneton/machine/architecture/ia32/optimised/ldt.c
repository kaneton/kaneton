/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/ldt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 20:13:58 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:36:44 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage local descriptor table.
 *
 * ldt_build reserves a new ldt segment in the current gdt. ldt_destroy removes
 * this entry.
 *
 * ldt_add_segment adds a new segment given its index while ldt_reserve_segment
 * does not requires an index and takes the first free slot.
 *
 * ldt_build_selector is used to build a segment selector (for loading in a
 * segment register).
 *
 * ldt_activate sets a ldt as the active one.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * returns the base pointer of a ldt.
 */

t_status		ia32_ldt_base(t_ia32_ldt*		table,
				      t_paddr*			addr)
{
  assert(addr != NULL);

  *addr = (t_paddr)table->descriptor;

  return STATUS_OK;
}

/*
 * returns size of a ldt.
 */

t_status		ia32_ldt_size(t_ia32_ldt*		table,
				      t_uint16*			size)
{
  assert(size != NULL);

  *size = table->count;

  return STATUS_OK;
}

/*
 * dumps a ldt
 */

t_status		ia32_ldt_dump(t_ia32_ldt*		table)
{
  t_ia32_gdt		n;

  assert(table != NULL);

  n.count = table->count;
  n.descriptor = (t_ia32_gdte*)table->descriptor;

  return ia32_gdt_dump(&n);
}

/*
 * activates a ldt.
 */

t_status		ia32_ldt_activate(t_ia32_ldt		table)
{
  LLDT(table.gdt_entry);

  return STATUS_OK;
}

/*
 * builds a local descriptor table with specified number of entries.
 *
 * steps:
 *
 * 1) checks the number of entries.
 * 2) aligns the address (for best performances).
 * 3) builds the ldt structure.
 * 4) adds the ldt segment to the gdt.
 * 5) clears the table if necessary.
 */

t_status		ia32_ldt_build(t_uint16			entries,
				       t_paddr			base,
				       t_ia32_ldt*		ldt,
				       t_uint8			clear)
{
  t_ia32_segment	seg;

  assert(ldt != NULL);

  /*
   * 1)
   */

  if (entries > IA32_LDT_MAX_ENTRIES)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (base % sizeof (t_ia32_ldte))
    {
      base += sizeof (t_ia32_ldte) - (base % sizeof (t_ia32_ldte));
    }
  /*
   * 3)
   */

  ldt->descriptor = (t_ia32_ldte*)base;
  ldt->count = entries;

  /*
   * 4)
   */

  seg.base = base;
  seg.limit = entries * sizeof (t_ia32_ldte);
  seg.privilege = ia32_prvl_supervisor;
  seg.is_system = 1;
  seg.type.sys = ia32_type_ldt;

  if (ia32_gdt_reserve_segment(IA32_GDT_CURRENT, seg,
			       &ldt->gdt_entry) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 5)
   */

  if (clear)
    {
      memset(ldt->descriptor, 0, entries * sizeof (t_ia32_ldte));
    }

  return STATUS_OK;
}

/*
 * destoys a ldt
 */

t_status		ia32_ldt_destroy(t_ia32_ldt		*ldt)
{
  assert(ldt != NULL);

  return ia32_gdt_delete_segment(IA32_GDT_CURRENT, ldt->gdt_entry);
}

/*
 * adds a segment, erasing previous segment present.
 *
 * steps:
 *
 * 1) checks the size of the table and the requested type of segment.
 * 2) sets the type field.
 * 3) sets the base field.
 * 4) sets the flags.
 * 5) sets the limit field.
 */

t_status		ia32_ldt_add_segment(t_ia32_ldt*	table,
					     t_uint16		segment,
					     t_ia32_segment	descriptor)
{
  t_uint32		size;

  /*
   * 1)
   */

  if (segment >= table->count || descriptor.is_system)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  table->descriptor[segment].type = IA32_DESC_TYPE_PRESENT |
    IA32_DESC_MK_DPL(descriptor.privilege);
  table->descriptor[segment].type |= IA32_GDT_TYPE_S | descriptor.type.usr;

  /*
   * 3)
   */

  table->descriptor[segment].base_00_15 = (t_uint16)descriptor.base;
  table->descriptor[segment].base_16_23 = (descriptor.base >> 16);
  table->descriptor[segment].base_24_31 = (descriptor.base >> 24);

  /*
   * 4)
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
   * 5)
   */

  table->descriptor[segment].limit_00_15 = (t_uint16)size;
  table->descriptor[segment].limit_16_19 = size >> 16;

  return STATUS_OK;
}

/*
 * reserve a segment. the first free segment entry is taken.
 *
 * steps:
 *
 * 1) searchs for an empty place.
 * 2) adds the segment.
 * 3) sets the reserved index.
 */

t_status		ia32_ldt_reserve_segment(t_ia32_ldt*	table,
						 t_ia32_segment	descriptor,
						 t_uint16*	segment)
{
  t_uint16		look;

  assert(segment != NULL);

  /*
   * 1)
   */

  look = 1;
  while (look < table->count &&
	 (table->descriptor[look].type & IA32_DESC_TYPE_PRESENT))
    look++;

  if (look == table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (ia32_ldt_add_segment(table, look, descriptor) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  *segment = look;

  return STATUS_OK;
}

/*
 * gets an entry.
 *
 * XXX
 */

t_status		ia32_ldt_get_segment(t_ia32_ldt*	table,
					     t_uint16		index,
					     t_ia32_segment*	segment)
{
  return STATUS_UNKNOWN_ERROR;
}

/*
 * deletes an entry.
 *
 * steps:
 *
 * 1) checks boundary.
 * 2) mark the segment as non-present.
 */

t_status		ia32_ldt_delete_segment(t_ia32_ldt*	table,
						t_uint16	segment)
{

  /*
   * 1)
   */

  if (segment >= table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  table->descriptor[segment].type &= ~IA32_DESC_TYPE_PRESENT;

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

t_status		ia32_ldt_build_selector(t_ia32_ldt*	table,
						t_uint16	segment,
						t_ia32_prvl	privilege,
						t_uint16*	selector)
{
  assert(selector != NULL);

  /*
   * 1)
   */

  if (segment >= table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  *selector = IA32_SEGSEL_TI_LDT | IA32_SEGSEL_MK_RPL(privilege) |
    IA32_SEGSEL_MK_INDEX(segment);

  return STATUS_OK;
}
