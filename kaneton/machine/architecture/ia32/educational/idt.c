/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/educational/idt.c
 *
 * created       renaud voltz   [sun feb 12 02:02:19 2006]
 * updated       julien quintard   [sat jan  8 16:17:55 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file provides functionalities for managing the IDT - Interrupt
 * Descriptor Table.
 *
 * for more information regarding the handlers triggered through the IDT,
 * please have a look at the handler.c file.
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
 * the current system IDT.
 */

as_idt_descriptor		_architecture_idt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the current IDT.
 *
 * steps:
 *
 * 1) display the header message.
 * 2) go through the IDT entries.
 *   a) ignore non-present entries.
 *   b) compute the offset, selector, privilege.
 *   c) build a type string.
 *   d) display the IDT entry.
 */

t_error			architecture_idt_dump(void)
{
  t_uint16		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "IDT: table(0x%08x) size(%u)\n",
	      _architecture_idt.table, _architecture_idt.size);

  /*
   * 2)
   */

  for (i = 0; i < _architecture_idt.size; i++)
    {
      char*		type;
      t_uint16		selector;
      t_vaddr		offset;
      at_privilege	privilege;

      /*
       * a)
       */

      if (!(_architecture_idt.table[i].type & ARCHITECTURE_IDT_TYPE_PRESENT))
	continue;

      /*
       * b)
       */

      offset =
	_architecture_idt.table[i].offset_00_15 |
	(_architecture_idt.table[i].offset_16_31 << 16);
      selector = _architecture_idt.table[i].selector;
      privilege = ARCHITECTURE_IDT_DPL_GET(_architecture_idt.table[i].type);

      /*
       * c)
       */

      switch (ARCHITECTURE_IDT_TYPE(_architecture_idt.table[i].type))
	{
	case ARCHITECTURE_IDT_TYPE_TASK:
	  {
	    type = "task gate";

	    break;
	  }
	case ARCHITECTURE_IDT_TYPE_INTERRUPT:
	  {
	    type = "interrupt gate";

	    break;
	  }
	case ARCHITECTURE_IDT_TYPE_TRAP:
	  {
	    type = "trap gate";

	    break;
	  }
	default:
	  MACHINE_ESCAPE("unknown gate type '%u'",
			 _architecture_idt.table[i].type);
	}

      /*
       * d)
       */

      module_call(console, message,
		  '#', "  %u: offset(0x%08x) selector(0x%x) type(%s) "
		  "privilege(%u)\n",
		  i, offset, selector, type, privilege);
    }

  MACHINE_LEAVE();
}

/*
 * this function builds an IDT according to the given parameters.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) align the base address if necessary.
 * 2) construct the descriptor.
 * 3) initialize the table entries.
 */

t_error			architecture_idt_build(t_paddr		base,
					       t_uint16		size,
					       as_idt_descriptor* descriptor)
{
  /*
   * 0)
   */

  if (descriptor == NULL)
    MACHINE_ESCAPE("the 'descriptor' argument is null");

  if (size > ARCHITECTURE_IDT_SIZE)
    MACHINE_ESCAPE("the given size is too large as exceeding the IDT's "
		   "theoretically maximum capacity");

  /*
   * 1)
   */

  if (base % sizeof(as_idt_entry))
    base += sizeof(as_idt_entry) - (base % sizeof(as_idt_entry));

  /*
   * 2)
   */

  descriptor->table = (as_idt_entry*)base;
  descriptor->size = size;

  /*
   * 3)
   */

  memset(descriptor->table, 0x0, size * sizeof(as_idt_entry));

  MACHINE_LEAVE();
}

/*
 * this function imports the given IDT making it the current IDT.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) initialize the IDT register.
 * 2) load the IDT register making the given IDT the current one.
 * 3) copy the given descriptor into the system's current one.
 */

t_error			architecture_idt_import(as_idt_descriptor* descriptor)
{
  as_idt_register	idtr;

  /*
   * 0)
   */

  if (descriptor == NULL)
    MACHINE_ESCAPE("the 'descriptor' argument is null");

  /*
   * 1)
   */

  idtr.address = (t_paddr)descriptor->table;
  idtr.size = descriptor->size * sizeof(as_idt_entry);

  /*
   * 2)
   */

  ARCHITECTURE_LIDT(idtr);

  /*
   * 3)
   */

  memcpy(&_architecture_idt, descriptor, sizeof(as_idt_descriptor));

  MACHINE_LEAVE();
}

/*
 * this function exports the current IDT into the given descriptor.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the current IDT register.
 * 2) copy the current IDT's content into the given descriptor's table.
 * 3) set the given descriptor's size.
 */

t_error			architecture_idt_export(as_idt_descriptor* descriptor)
{
  as_idt_register	idtr;
  as_idt_entry*		source;
  as_idt_entry*		dest;

  /*
   * 0)
   */

  if (descriptor == NULL)
    MACHINE_ESCAPE("the 'descriptor' argument is null");

  /*
   * 1)
   */

  ARCHITECTURE_SIDT(idtr);

  /*
   * 2)
   */

  source = (as_idt_entry*)idtr.address;
  dest = descriptor->table;

  memcpy(dest, source, idtr.size);

  /*
   * 3)
   */

  descriptor->size = idtr.size / sizeof(as_idt_entry);

  MACHINE_LEAVE();
}

/*
 * this function inserts an entry in the table, at a precise index, hence
 * possibly overwriting the previous entry.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) set the type.
 * 2) set the segment selector.
 * 3) set the offset.
 * 4) set the reserved bits to zero.
 */

t_error			architecture_idt_insert(t_uint16	index,
						t_vaddr		offset,
						t_uint16	selector,
						at_privilege	privilege,
						at_idt_type	type)
{
  /*
   * 0)
   */

  if (index >= _architecture_idt.size)
    MACHINE_ESCAPE("out-of-bound insertion");

  /*
   * 1)
   */

  _architecture_idt.table[index].type =
    ARCHITECTURE_IDT_TYPE_PRESENT |
    ARCHITECTURE_IDT_DPL_SET(privilege) |
    type;

  /*
   * 2)
   */

  _architecture_idt.table[index].selector = selector;

  /*
   * 3)
   */

  _architecture_idt.table[index].offset_00_15 = offset & 0xffff;
  _architecture_idt.table[index].offset_16_31 = (offset >> 16) & 0xffff;

  /*
   * 4)
   */

  _architecture_idt.table[index].reserved = 0;

  MACHINE_LEAVE();
}

/*
 * this function deletes an entry from the IDT.
 *
 * steps:
 *
 * 0) verify the arguments
 * 1) re-initialize the entry's memory.
 */

t_error			architecture_idt_delete(t_uint16	index)
{
  /*
   * 0)
   */

  if (index >= _architecture_idt.size)
    MACHINE_ESCAPE("out-of-bound index");

  /*
   * 1)
   */

  memset(&_architecture_idt.table[index], 0x0, sizeof(as_idt_entry));

  MACHINE_LEAVE();
}
