/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/ldt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 20:13:58 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:58:02 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage global descriptor table.
 *
 */

/*                                                                  [cut] k2 */

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

/*                                                                 [cut] /k2 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * this is the good place to put code dealing with ldt.
 *
 */


/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * returns the base pointer of a ldt.
 */

t_error			ldt_base(t_ldt*		table,
				 t_paddr*	addr)
{
  *addr = (t_paddr)table->descriptor;

  return ERROR_NONE;
}

/*
 * returns size of a ldt.
 */

t_error			ldt_size(t_ldt*		table,
				 t_uint16	*size)
{
  *size = table->count;

  return ERROR_NONE;
}

/*
 * dumps a ldt
 */

t_error			ldt_dump(t_ldt*		table)
{
  t_gdt			n;

  n.count = table->count;
  n.descriptor = (t_gdte*)table->descriptor;

  return gdt_dump(&n);
}

/*
 * activates a ldt.
 */

t_error			ldt_activate(t_ldt	table)
{
  LLDT(table.gdt_entry);

  return ERROR_NONE;
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

t_error			ldt_build(t_uint16	entries,
				  t_paddr	base,
				  t_ldt*	ldt,
				  t_uint8	clear)
{
  t_segment		seg;

  /*
   * 1)
   */

  if (entries > LDT_MAX_ENTRIES)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */
/* XXX
   - on a malloc le pointeur base, donc si on s'amuse a le changer c'est bof


  if (base % sizeof(t_ldte))
    {
      base += sizeof(t_ldte) - (base % sizeof(t_ldte));
    }
*/
  /*
   * 3)
   */

  ldt->descriptor = (t_ldte*)base;
  ldt->count = entries;

  /*
   * 4)
   */

  seg.base = base;
  seg.limit = entries * sizeof(t_ldte);
  seg.privilege = prvl_supervisor;
  seg.is_system = 1;
  seg.type.sys = type_ldt;

  if (gdt_reserve_segment(NULL, seg, &ldt->gdt_entry) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 5)
   */

  if (clear)
    {
      memset(ldt->descriptor, 0, entries * sizeof(t_ldte));
    }

  return ERROR_NONE;
}

/*
 * destoys a ldt
 */

t_error			ldt_destroy(t_ldt	*ldt)
{
  return gdt_delete_segment(NULL, ldt->gdt_entry);
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

t_error			ldt_add_segment(t_ldt*		table,
					t_uint16	segment,
					t_segment	descriptor)
{
  t_uint32		size;

  /*
   * 1)
   */

  if (segment >= table->count || descriptor.is_system)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  table->descriptor[segment].type = GDT_TYPE_PRESENT |
    GDT_TYPE_MK_DPL(descriptor.privilege);
  table->descriptor[segment].type |= GDT_TYPE_S | descriptor.type.usr;

  /*
   * 3)
   */

  table->descriptor[segment].base_00_15 = (t_uint16)descriptor.base;
  table->descriptor[segment].base_16_23 = (descriptor.base >> 16);
  table->descriptor[segment].base_24_31 = (descriptor.base >> 24);

  /*
   * 4)
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
   * 5)
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
 * 1) searchs for an empty place.
 * 2) adds the segment.
 * 3) sets the reserved index.
 */

t_error			ldt_reserve_segment(t_ldt*	table,
					    t_segment	descriptor,
					    t_uint16*	segment)
{
  t_uint16		look;

  /*
   * 1)
   */

  look = 1;
  while (look < table->count &&
	 (table->descriptor[look].type & GDT_TYPE_PRESENT))
    look++;

  if (look == table->count)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (ldt_add_segment(table, look, descriptor) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  *segment = look;

  return ERROR_NONE;
}

/*
 * gets an entry.
 *
 * XXX
 */

t_error			ldt_get_segment(t_ldt*		table,
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
 * 1) checks boundary.
 * 2) mark the segment as non-present.
 */

t_error			ldt_delete_segment(t_ldt*	table,
					   t_uint16	segment)
{

  /*
   * 1)
   */

  if (segment >= table->count)
    return ERROR_UNKNOWN;

  /*
   * 2)
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

t_error			ldt_build_selector(t_ldt*	table,
					   t_uint16	segment,
					   t_prvl	privilege,
					   t_uint16*	selector)
{

  /*
   * 1)
   */

  if (segment >= table->count)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  *selector = SEGSEL_TI_LDT | SEGSEL_MK_RPL(privilege) |
    SEGSEL_MK_INDEX(segment);

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
