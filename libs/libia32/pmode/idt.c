/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/idt.c
 *
 * created       renaud voltz   [sun feb 12 02:02:19 2006]
 * updated       matthieu bucchianeri   [sun apr  2 23:48:42 2006]
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
 * students  will  place  here   the  functions  they  need  to  manage
 * idt. prototypes are not imposed.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * idt table pointer
 */

t_ia32_idt           idt;

/*                                                                  [cut] /k3 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k3 */

/*
 * dumps an idt.
 *
 * steps:
 *
 * 1) retrieve the global pointer if needed.
 * 2) dump every idt entry.
 */

t_error			idt_dump(t_ia32_idt*			table)
{
  t_uint16		i;
  t_ia32_gate		gate;
  const char*		type;

  /*
   * 1)
   */

  if (!table)
    table = &idt;

  /*
   * 2)
   */

  for (i = 0; i < table->count; i++)
    {
      if (idt_get_gate(table, i, &gate) != ERROR_NONE)
	continue;

      type = NULL;
      if (gate.type == ia32_type_gate_task)
	type = "Call gate";
      if (gate.type == ia32_type_gate_interrupt)
	type = "Interrupt gate";
      if (gate.type == ia32_type_gate_trap)
	type = "Trap gate";
      if (!type)
	type = "other";

      printf("gate %d, offset = 0x%x, type = %x\n",
      i, gate.offset, type);
    }

  return ERROR_NONE;
}

/*
 * return the size of an idt.
 */

t_error			idt_size(t_ia32_idt*			table,
				 t_uint16			*size)
{
  if (!table)
    table = &idt;

  *size = table->count;

  return ERROR_NONE;
}

/*
 * build an idt.
 *
 * steps:
 *
 * 1) check the number of entries.
 * 2) align address (for best performances).
 * 3) build the idt structure.
 * 4) clear the table if necessary.
 */

t_error			idt_build(t_uint16			entries,
				  t_paddr			base,
				  t_uint8			clear,
				  t_ia32_idt*			table)
{
  /*
   * 1)
   */

  if (entries > IDT_MAX_ENTRIES)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (base % sizeof(t_ia32_idte))
    base += sizeof(t_ia32_idte) - (base % sizeof(t_ia32_idte));

  /*
   * 3)
   */

  table->descriptor = (t_ia32_idte*)base;
  table->count = entries;

  /*
   * 4)
   */

  if (clear)
    memset(table->descriptor, 0, entries * sizeof(t_ia32_idte));

  return ERROR_NONE;
}

/*
 * activate an idt.
 *
 * steps:
 *
 * 1) upate idt record.
 * 2) build and load the new idt register.
 */

t_error			idt_activate(t_ia32_idt*		table)
{
  t_ia32_idtr		idtr;

  /*
   * 1)
   */

  idt.descriptor = table->descriptor;
  idt.count = table->count;

  /*
   * 2
   */

  idtr.address = (t_paddr)idt.descriptor;
  idtr.size = idt.count * sizeof(t_ia32_idte);
  LIDT(idtr);

  return ERROR_NONE;
}

/*
 * import descriptors from previous idt.
 *
 * steps:
 *
 * 1) get the current idt register.
 * 2) check old and new idt's sizes.
 * 3) copy to new idt address.
 */

t_error			idt_import(t_ia32_idt*			table)
{
  t_ia32_idtr		sidtr;
  t_ia32_idte*		source;
  t_ia32_idte*		dest;

  /*
   * 1)
   */

  SIDT(sidtr);

  /*
   * 2
   */

  if (sidtr.size > table->count * sizeof(t_ia32_idte))
    return ERROR_UNKNOWN;

  /*
   * 3
   */

  source = (t_ia32_idte*)sidtr.address;
  dest = table->descriptor;

  return ERROR_NONE;
}

/*
 * add a gate, overwritting previous present one.
 *
 * steps:
 *
 * 1) retrieve the global pointer if needed.
 * 2) check the size of the table.
 * 3) set the type field.
 * 4) set the segsel field.
 * 5) set the offset fields.
 * 6) set the reserved field.
 */

t_error			idt_add_gate(t_ia32_idt*		table,
				     t_uint16			index,
				     t_ia32_gate		gate)
{
  /*
   * 1)
   */

  if (!table)
    table = &idt;

  /*
   * 2)
   */

  if (index >= table->count)
    return ERROR_UNKNOWN;

  /*
   * 3
   */

  table->descriptor[index].type = DESC_TYPE_PRESENT |
    DESC_MK_DPL(gate.privilege);
  table->descriptor[index].type |= gate.type;

  /*
   * 4)
   */

  table->descriptor[index].segsel = gate.segsel;

  /*
   * 5)
   */

  table->descriptor[index].offset_00_15 = gate.offset & 0xffff;
  table->descriptor[index].offset_16_31 = (gate.offset >> 16) & 0xffff;

  /*
   * 6)
   */

  table->descriptor[index].reserved = 0;

  return ERROR_NONE;
}

/*
 * get an idt entry.
 *
 * steps:
 *
 * 1) retrieve the global pointer if needed.
 * 2) check the segment existence.
 * 3) get the type.
 * 4) get the segsel.
 * 5) get the offset.
 * 6) get the privileges.
 */

t_error			idt_get_gate(t_ia32_idt*		table,
				     t_uint16			index,
				     t_ia32_gate*		gate)
{
  /*
   * 1)
   */

  if (!table)
    table = &idt;

  /*
   * 2)
   */

  if (index >= table->count ||
      !(table->descriptor[index].type & DESC_TYPE_PRESENT))
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  gate->type = IDT_GET_TYPE(table->descriptor[index].type);

  /*
   * 4)
   */

  gate->segsel = table->descriptor[index].segsel;

  /*
   * 5)
   */

  gate->offset = table->descriptor[index].offset_00_15 |
    table->descriptor[index].offset_16_31;

  /*
   * 6
   */

  gate->privilege = DESC_GET_DPL(table->descriptor[index].type);

  return ERROR_NONE;
}

/*
 * delete a gate from the idt.
 *
 * steps:
 *
 * 1) get the global pointer if needed.
 * 2) check gate existence.
 * 3) mark the gate as non-present.
 */

t_error			idt_delete_gate(t_ia32_idt*		table,
					t_uint16		gate_id)
{
  /*
   * 1)
   */

  if (!table)
    table = &idt;

  /*
   * 2)
   */

  if (gate_id >= table->count)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  table->descriptor[gate_id].type &= ~DESC_TYPE_PRESENT;

  return ERROR_UNKNOWN;
}

/*                                                                 [cut] /k3 */
