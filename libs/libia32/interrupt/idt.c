/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/idt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 19:47:59 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:48:35 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage interrupt descriptor table.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students  will  place  here   the  function  they  need  to  manage
 * idt. prototypes are not imposed.
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
t_error			idt_dump(t_idt*			dump_idt)
{

}

t_error			idt_size(t_idt*			table,
				 t_uint16		*size)
{
  if (!table)
    table = &idt;

  *size = table->count;

  return ERROR_NONE;
}


t_error			idt_build(t_uint16		entries,
				  t_paddr		base,
				  t_idt*		idt,
				  t_uint8		clear)
{
  if (entries > GDT_MAX_ENTRIES)
    return ERROR_UNKNOWN;

  if (base % sizeof(t_idte))
    base += sizeof(t_idte) - (base % sizeof(t_idte));

  idt->descriptor = (t_idte*)base;
  idt->count = entries;

  if (clear)
    memset(idt->descriptor, 0, entries * sizeof(t_idte));

  return ERROR_NONE;
}

t_error			idt_activate(t_idt		new_idt)
{
  t_idtr		idtr;

  idtr.address = (t_paddr)new_idt.descriptor;
  idtr.size = new_idt.count * sizeof(t_idte);
  LIDT(idtr);

  idt.descriptor = new_idt.descriptor;
  idt.count = new_idt.count;

  return ERROR_NONE;
}

t_error			idt_import(t_idt*		idt)
{
  t_idtr		sidtr;
  t_idte*		source;
  t_idte*		dest;


  SIDT(sgdtr);


  if (sidtr.size > idt->count * sizeof(t_idte))
    return  ERROR_UNKNOWN;

  source = (t_idte*)sidtr.address;
  dest = idt->descriptor;

  return ERROR_NONE;
}

t_error			idt_add_segment(t_idt*		table,
					t_uint16	segment,
					t_segment	descriptor)
{

}

t_error			idt_reserve_segment(t_idt*	table,
					    t_segment	descriptor,
					    t_uint16*	segment)
{

}

t_error			idt_get_segment(t_idt*		table,
					t_uint16	index,
					t_segment	segment)
{

}

t_error			gdt_delete_segment(t_gdt*	table,
					   t_uint16	segment)
{

  if (!table)
    table = &idt;

  if (segment >= table->count)
    return ERROR_UNKNOWN;

  table->decriptor[segment].type &= ~IDT_TYPE_PRESENT;

  return ERROR_UNKNOWN;
}

t_error			gdt_build_selector(t_uint16	segment,
					   t_prvl	privilege,
					   t_uint16*	selector)
{

}
*/
/*                                                                 [cut] /k2 */
