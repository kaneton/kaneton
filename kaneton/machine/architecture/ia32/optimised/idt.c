/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/idt.c
 *
 * created       renaud voltz   [sun feb 12 02:02:19 2006]
 * updated       matthieu bucchianeri   [fri mar 16 21:38:28 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage interrupt descriptor table.
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
 * idt table pointer
 */

t_ia32_idt           ia32_idt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * dumps an idt.
 *
 * steps:
 *
 * 1) retrieve the global pointer if needed.
 * 2) dump every idt entry.
 */

t_status		ia32_idt_dump(t_ia32_idt*		table)
{
  t_uint16		i;
  t_ia32_gate		gate;
  const char*		type;

  /*
   * 1)
   */

  if (!table)
    table = &ia32_idt;

  /*
   * 2)
   */

  for (i = 0; i < table->count; i++)
    {
      if (ia32_idt_get_gate(table, i, &gate) != STATUS_OK)
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

  return STATUS_OK;
}

/*
 * return the size of an idt.
 */

t_status		ia32_idt_size(t_ia32_idt*		table,
				      t_uint16*			size)
{
  assert(size != NULL);

  if (!table)
    table = &ia32_idt;

  *size = table->count;

  return STATUS_OK;
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

t_status		ia32_idt_build(t_uint16		entries,
				       t_paddr		base,
				       t_uint8		clear,
				       t_ia32_idt*	table)
{
  assert(table != NULL);

  /*
   * 1)
   */

  if (entries > IA32_IDT_MAX_ENTRIES)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (base % sizeof (t_ia32_idte))
    base += sizeof (t_ia32_idte) - (base % sizeof (t_ia32_idte));

  /*
   * 3)
   */

  table->descriptor = (t_ia32_idte*)base;
  table->count = entries;

  /*
   * 4)
   */

  if (clear)
    memset(table->descriptor, 0, entries * sizeof (t_ia32_idte));

  return STATUS_OK;
}

/*
 * activate an idt.
 *
 * steps:
 *
 * 1) upate idt record.
 * 2) build and load the new idt register.
 */

t_status		ia32_idt_activate(t_ia32_idt*		table)
{
  t_ia32_idtr		idtr;

  assert(table != NULL);

  /*
   * 1)
   */

  ia32_idt.descriptor = table->descriptor;
  ia32_idt.count = table->count;

  /*
   * 2
   */

  idtr.address = (t_paddr)ia32_idt.descriptor;
  idtr.size = ia32_idt.count * sizeof (t_ia32_idte);
  LIDT(idtr);

  return STATUS_OK;
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

t_status		ia32_idt_import(t_ia32_idt*		table)
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

  if (sidtr.size > table->count * sizeof (t_ia32_idte))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3
   */

  source = (t_ia32_idte*)sidtr.address;
  dest = table->descriptor;

  return STATUS_OK;
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

t_status		ia32_idt_add_gate(t_ia32_idt*		table,
					  t_uint16		index,
					  t_ia32_gate		gate)
{
  /*
   * 1)
   */

  if (!table)
    table = &ia32_idt;

  /*
   * 2)
   */

  if (index >= table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3
   */

  table->descriptor[index].type = IA32_DESC_TYPE_PRESENT |
    IA32_DESC_MK_DPL(gate.privilege);
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

  return STATUS_OK;
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

t_status		ia32_idt_get_gate(t_ia32_idt*		table,
					  t_uint16		index,
					  t_ia32_gate*		gate)
{
  assert(gate != NULL);

  /*
   * 1)
   */

  if (!table)
    table = &ia32_idt;

  /*
   * 2)
   */

  if (index >= table->count ||
      !(table->descriptor[index].type & IA32_DESC_TYPE_PRESENT))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  gate->type = IA32_IDT_GET_TYPE(table->descriptor[index].type);

  /*
   * 4)
   */

  gate->segsel = table->descriptor[index].segsel;

  /*
   * 5)
   */

  gate->offset = table->descriptor[index].offset_00_15 |
    table->descriptor[index].offset_16_31 << 16;

  /*
   * 6
   */

  gate->privilege = IA32_DESC_GET_DPL(table->descriptor[index].type);

  return STATUS_OK;
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

t_status		ia32_idt_delete_gate(t_ia32_idt*	table,
					     t_uint16		gate_id)
{
  /*
   * 1)
   */

  if (!table)
    table = &ia32_idt;

  /*
   * 2)
   */

  if (gate_id >= table->count)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  table->descriptor[gate_id].type &= ~IA32_DESC_TYPE_PRESENT;

  return STATUS_UNKNOWN_ERROR;
}
