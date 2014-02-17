/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/gdt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:45:19 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:36:16 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage global descriptor table.
 *
 * gdt_import copies segment descriptors from the active gdt to a new one.
 * this is useful to create a new gdt not discarding old descriptors (for
 * example to create a larget table from an existing one).
 *
 * gdt_activate is used to set a gdt as new active gdt.
 *
 * gdt_add_segment adds a new segment given its index while gdt_reserve_segment
 * does not requires an index and takes the first free slot.
 *
 * gdt_build_selector is used to build a segment selector (for loading in a
 * segment register).
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
 * gdt table pointer
 */

t_ia32_gdt	ia32_gdt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dumps a gdt.
 *
 * steps:
 *
 * XXX
 */

t_status		ia32_gdt_dump(t_ia32_gdt*		dump_gdt)
{
  t_uint16		i;
  t_ia32_segment	seg;
  const char*		type;

  /*
   * 1)
   */

  if (dump_gdt == IA32_GDT_CURRENT)
    dump_gdt = &ia32_gdt;

  /*
   * 2)
   */

  for (i = 1; i < dump_gdt->count; i++)
    {
      if (ia32_gdt_get_segment(dump_gdt, i, &seg) != STATUS_OK)
	continue;

      type = NULL;
      if (seg.is_system)
	{
	  if (seg.type.sys == ia32_type_ldt)
	    type = "LDT";
	  if (seg.type.sys == ia32_type_tss)
	    type = "TSS";
	  if (seg.type.sys == ia32_type_call_gate)
	    type = "Call Gate";
	  if (seg.type.sys == ia32_type_int_gate)
	    type = "Interrupt Gate";
	  if (seg.type.sys == ia32_type_trap_gate)
	    type = "Trap Gate";
	}
      else
	{
	  if (seg.type.usr == ia32_type_code)
	    type = "Code";
	  if (seg.type.usr == ia32_type_data)
	    type = "Data";
	}
      if (!type)
	type = "Other";

      printf("segment %d, base = 0x%x limit = 0x%x, type = %s\n",
	     i, seg.base, seg.limit, type);
    }

  return STATUS_OK;
}

/*
 * returns size of a gdt.
 */

t_status		ia32_gdt_size(t_ia32_gdt*		table,
				      t_uint16*			size)
{
  assert(size != NULL);

  if (table == IA32_GDT_CURRENT)
    table = &ia32_gdt;

  *size = table->count;

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

t_status		ia32_gdt_build(t_uint16			entries,
				       t_paddr			base,
				       t_ia32_gdt*		gdt,
				       t_uint8			clear)
{
  assert(gdt != NULL);

  /*
   * 1)
   */

  if (entries > IA32_GDT_MAX_ENTRIES)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (base % sizeof (t_ia32_gdte))
    {
      base += sizeof (t_ia32_gdte) - (base % sizeof (t_ia32_gdte));
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
      memset(gdt->descriptor, 0, entries * sizeof (t_ia32_gdte));
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

t_status		ia32_gdt_activate(t_ia32_gdt		new_gdt)
{
  t_ia32_gdtr		gdtr;

  /*
   * 1)
   */

  gdtr.address = (t_paddr)new_gdt.descriptor;
  gdtr.size = new_gdt.count * sizeof (t_ia32_gdte);
  LGDT(gdtr);

  /*
   * 2)
   */

  ia32_gdt.descriptor = new_gdt.descriptor;
  ia32_gdt.count = new_gdt.count;

  return STATUS_OK;
}

/*
 * imports descriptors from previous gdt
 *
 * steps:
 *
 * 1) gets the current gdt register.
 * 2) checks size of the old and new gdt.
 * 3) copies to new gdt address.
 */

t_status		ia32_gdt_import(t_ia32_gdt*		gdt)
{
  t_ia32_gdtr		sgdtr;
  t_ia32_gdte*		source;
  t_ia32_gdte*		dest;

  /*
   * 1)
   */

  SGDT(sgdtr);

  /*
   * 2)
   */

  if (sgdtr.size > gdt->count * sizeof (t_ia32_gdte))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  source = (t_ia32_gdte*)sgdtr.address;
  dest = gdt->descriptor;

  memcpy(dest, source, sgdtr.size);

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

t_status		ia32_gdt_add_segment(t_ia32_gdt*	table,
					     t_uint16		segment,
					     t_ia32_segment	descriptor)
{
  t_uint32		size;

  /*
   * 1)
   */

  if (table == IA32_GDT_CURRENT)
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
 * reserve a segment. the first free segment entry is taken.
 *
 * steps:
 *
 * 1) gets the global pointer if needed.
 * 2) searchs for an empty place.
 * 3) adds the segment.
 * 4) sets the reserved index.
 */

t_status		ia32_gdt_reserve_segment(t_ia32_gdt*	table,
						 t_ia32_segment	descriptor,
						 t_uint16*	segment)
{
  t_uint16		look;

  assert(segment != NULL);

  /*
   * 1)
   */

  if (table == IA32_GDT_CURRENT)
    table = &ia32_gdt;

  /*
   * 2)
   */

  look = 1;
  while (look < table->count &&
	 (table->descriptor[look].type & IA32_DESC_TYPE_PRESENT))
    look++;

  if (look == table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  if (ia32_gdt_add_segment(table, look, descriptor) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 4)
   */

  *segment = look;

  return STATUS_OK;
}

/*
 * gets an entry.
 *
 * XXX
 */

t_status		ia32_gdt_get_segment(t_ia32_gdt*	table,
					     t_uint16		index,
					     t_ia32_segment*	segment)
{
  assert(segment != NULL);

  /*
   * 1)
   */

  if (table == IA32_GDT_CURRENT)
    table = &ia32_gdt;

  /*
   * 2)
   */

  if (index >= table->count ||
      !(table->descriptor[index].type & IA32_DESC_TYPE_PRESENT))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  segment->base = table->descriptor[index].base_00_15 |
    (table->descriptor[index].base_16_23 << 16) |
    (table->descriptor[index].base_24_31 << 24);

  segment->limit = table->descriptor[index].limit_00_15 |
    (table->descriptor[index].limit_16_19 << 16);

  if (table->descriptor[index].flags & IA32_GDT_FLAG_GRANULAR)
    segment->limit *= 4096;

  segment->privilege = IA32_DESC_GET_DPL(table->descriptor[index].type);

  segment->is_system = !(table->descriptor[index].type & IA32_GDT_TYPE_S);

  if (segment->is_system)
    {
      segment->type.sys = IA32_GDT_TYPE_SYS(table->descriptor[index].type);
    }
  else
    {
      segment->type.usr = IA32_GDT_TYPE_SEG(table->descriptor[index].type);
    }

  return STATUS_OK;
}

/*
 * deletes an entry.
 *
 * steps:
 *
 * 1) gets the global pointer if needed.
 * 2) checks boundary.
 * 3) mark the segment as non-present.
 */

t_status		ia32_gdt_delete_segment(t_ia32_gdt*	table,
						t_uint16	segment)
{

  /*
   * 1)
   */

  if (table == IA32_GDT_CURRENT)
    table = &ia32_gdt;

  /*
   * 2)
   */

  if (segment >= table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
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

t_status		ia32_gdt_build_selector(t_uint16	segment,
						t_ia32_prvl	privilege,
						t_uint16*	selector)
{
  assert(selector != NULL);

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
