/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/gdt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:45:19 2005]
 * updated       matthieu bucchianeri   [fri dec 30 19:15:36 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage global descriptor table.
 *
 * gdt_refresh is used to updates local data with the content of the current
 * gdtr. XXX peut etre renommer en gdt_init.
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
 *
 * XXX lorsque un t_gdt* est passé en argument, la valeur NULL indique
 *     d'utiliser la GDT active. c'est un peu chiant mais ca permet d'utiliser
 *     les fonctions avant de switcher le gdtr pour preparer une nouvelle gdt.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * gdt table pointer
 */

t_gdt		gdt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * refreshes the current gdt pointer
 *
 * steps:
 *
 * 1) stores the gdtr.
 * 2) fills the global gdt structure.
 */

t_error			gdt_refresh(void)
{
  t_gdtr		gdtr;

  /*
   * 1)
   */

  SGDT(gdtr);

  /*
   * 2)
   */

  gdt.descriptor = (t_gdte*)gdtr.address;
  gdt.count = gdtr.size / sizeof(t_gdte);

  return ERROR_NONE;
}

/*
 * dumps a gdt.
 *
 * steps:
 *
 * XXX
 */

t_error			gdt_dump(t_gdt*       	dump_gdt)
{
  t_uint16		i;
  t_gdte*		entries;
  t_uint64		limit;

  /*
   * 1)
   */

  if (!dump_gdt)
    dump_gdt = &gdt;

  /*
   * 2)
   */

  entries = dump_gdt->descriptor;

  /*
   * 3)
   */

  for (i = 1; i < dump_gdt->count; i++)
    {
      if ((entries[i].type & GDT_TYPE_PRESENT))
	{
	  printf("entry %d ", i);
	  if ((entries[i].flags & GDT_FLAG_GRANULAR))
	    limit = (t_uint64)((entries[i].limit_00_15 |
		      (entries[i].limit_16_19 << 16)) + 1) * 4096;
	  else
	    limit = entries[i].limit_00_15 | (entries[i].limit_16_19 << 16);
	  printf("base %u, limit = %qu",
		 (t_uint32)(entries[i].base_00_15 |
			    (entries[i].base_16_23 << 16) |
			    (entries[i].base_24_31 << 24)),
		 limit);
	  if (entries[i].flags & GDT_FLAG_GRANULAR)
	    printf(" G");
	  if (!(entries[i].type & GDT_TYPE_S))
	    {
	      if (GDT_TYPE_SYS(entries[i].type) == GDT_TYPE_LDT)
		printf(" LDT");
	      else if (GDT_TYPE_SYS(entries[i].type) == GDT_TYPE_TSS)
		printf(" TSS");
	      else if (GDT_TYPE_SYS(entries[i].type) == GDT_GATE_CALL)
		printf(" Call Gate");
	      else if (GDT_TYPE_SYS(entries[i].type) == GDT_GATE_TRAP)
		printf(" Trap Gate");
	      else if (GDT_TYPE_SYS(entries[i].type) == GDT_GATE_INTERRUPT)
		printf(" Int. Gate");
	      else
		printf(" Oth.");
	    }
	  else
	    {
	      if (GDT_TYPE_SEG(entries[i].type) == GDT_TYPE_DATA)
		printf(" Data");
	      else
		printf(" Code");
	    }
	  printf("\n");
	}
    }

  return ERROR_NONE;
}

/*
 * returns size of a gdt.
 */

t_error			gdt_size(t_gdt*		table,
				 t_uint16	*size)
{
  if (!table)
    table = &gdt;

  *size = table->count;

  return ERROR_NONE;
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

t_error			gdt_build(t_uint16	entries,
				  t_paddr	base,
				  t_gdt*	gdt,
				  t_uint8	clear)
{

  /*
   * 1)
   */

  if (entries > GDT_MAX_ENTRIES)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (base % sizeof(t_gdte))
    {
      base += sizeof(t_gdte) - (base % sizeof(t_gdte));
    }

  /*
   * 3)
   */

  gdt->descriptor = (t_gdte*)base;
  gdt->count = entries;

  /*
   * 4)
   */

  if (clear)
    {
      memset(gdt->descriptor, 0, entries * sizeof(t_gdte));
    }

  return ERROR_NONE;
}

/*
 * activates a gdt.
 *
 * steps:
 *
 * 1) builds and loads the new gdt register.
 * 2) updates global gdt record.
 */

t_error			gdt_activate(t_gdt	new_gdt)
{
  t_gdtr		gdtr;

  /*
   * 1)
   */

  gdtr.address = (t_paddr)new_gdt.descriptor;
  gdtr.size = new_gdt.count * sizeof(t_gdte);
  LGDT(gdtr);

  /*
   * 2)
   */

  gdt.descriptor = new_gdt.descriptor;
  gdt.count = new_gdt.count;

  return ERROR_NONE;
}

/*
 * imports descriptors from previous gdt
 *
 * XXX plutot utiliser la globale gdt ?
 *
 * steps:
 *
 * 1) gets the current gdt register.
 * 2) checks size of the old and new gdt.
 * 3) copies to new gdt address.
 */

t_error			gdt_import(t_gdt*	gdt)
{
  t_gdtr		sgdtr;
  t_gdte*		source;
  t_gdte*		dest;

  /*
   * 1)
   */

  SGDT(sgdtr);

  /*
   * 2)
   */

  if (sgdtr.size > gdt->count * sizeof(t_gdte))
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  source = (t_gdte*)sgdtr.address;
  dest = gdt->descriptor;

  memcpy(dest, source, sgdtr.size);

  return ERROR_NONE;
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

t_error			gdt_add_segment(t_gdt*		table,
					t_uint16	segment,
					t_segment	descriptor)
{
  t_uint32		size;

  /*
   * 1)
   */

  if (!table)
    table = &gdt;

  /*
   * 2)
   */

  if (segment >= table->count)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  table->descriptor[segment].type = GDT_TYPE_PRESENT |
    GDT_TYPE_MK_DPL(descriptor.privilege);
  if (descriptor.is_system)
    table->descriptor[segment].type |= descriptor.type.sys;
  else
    table->descriptor[segment].type |= GDT_TYPE_S | descriptor.type.usr;

  /*
   * 4)
   */

  table->descriptor[segment].base_00_15 = (t_uint16)descriptor.base;
  table->descriptor[segment].base_16_23 = (descriptor.base >> 16);
  table->descriptor[segment].base_24_31 = (descriptor.base >> 24);

  /*
   * 5)
   */

  table->descriptor[segment].flags = GDT_FLAG_USE32;

  if (descriptor.limit >= 4096)
    {
      table->descriptor[segment].flags |= GDT_FLAG_GRANULAR;
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

  return ERROR_NONE;
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

t_error			gdt_reserve_segment(t_gdt*	table,
					    t_segment	descriptor,
					    t_uint16*	segment)
{
  t_uint16		look;

  /*
   * 1)
   */

  if (!table)
    table = &gdt;

  /*
   * 2)
   */

  look = 1;
  while (look < table->count &&
	 (table->descriptor[look].type & GDT_TYPE_PRESENT))
    look++;

  if (look == table->count)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if (gdt_add_segment(table, look, descriptor) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 4)
   */

  *segment = look;

  return ERROR_NONE;
}

/*
 * gets an entry.
 *
 * XXX
 */

t_error			gdt_get_segment(t_gdt*		table,
					t_uint16	index,
					t_segment*	segment)
{
  return ERROR_UNKNOWN;
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

t_error			gdt_delete_segment(t_gdt*	table,
					   t_uint16	segment)
{

  /*
   * 1)
   */

  if (!table)
    table = &gdt;

  /*
   * 2)
   */

  if (segment >= table->count)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  table->descriptor[segment].type &= ~GDT_TYPE_PRESENT;

  return ERROR_NONE;
}

/*
 * builds a selector with specified segment and privileges.
 *
 * steps:
 *
 * 1) checks segment entry boundary.
 * 2) builds the selector.
 */

t_error			gdt_build_selector(t_uint16	segment,
					   t_prvl	privilege,
					   t_uint16*	selector)
{

  /*
   * 1)
   */

  if (segment >= gdt.count)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  *selector = SEGSEL_TI_GDT | SEGSEL_MK_RPL(privilege) |
    SEGSEL_MK_INDEX(segment);

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
