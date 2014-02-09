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
 * updated       julien quintard   [sun feb  6 15:34:40 2011]
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

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the event manager which contains the current IDT.
 */

extern m_event		_event;

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
 *   c) build the flags string.
 *   d) build a type string.
 *   e) display the IDT entry.
 */

t_status		architecture_idt_dump(void)
{
  t_uint16		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "IDT: table(0x%08x) size(%u)\n",
	      _event.machine.idt.table,
	      _event.machine.idt.size);

  /*
   * 2)
   */

  for (i = 0; i < _event.machine.idt.size; i++)
    {
      char*		type;
      t_uint16		selector;
      t_vaddr		offset;
      t_privilege	privilege;
      char		flags[2];

      /*
       * a)
       */

      if (!(_event.machine.idt.table[i] & ARCHITECTURE_IDTE_PRESENT))
	continue;

      /*
       * b)
       */

      offset = ARCHITECTURE_IDTE_OFFSET_GET(_event.machine.idt.table[i]);
      selector = ARCHITECTURE_IDTE_SELECTOR_GET(_event.machine.idt.table[i]);
      privilege = ARCHITECTURE_IDTE_DPL_GET(_event.machine.idt.table[i]);

      /*
       * c)
       */

      if (_event.machine.idt.table[i] & ARCHITECTURE_IDTE_32BIT)
	flags[0] = 'd';
      else
	flags[0] = '.';

      flags[1] = '\0';

      /*
       * d)
       */

      switch (ARCHITECTURE_IDTE_TYPE(_event.machine.idt.table[i]))
	{
	case ARCHITECTURE_IDTE_TASK:
	  {
	    type = "task gate";

	    break;
	  }
	case ARCHITECTURE_IDTE_INTERRUPT:
	  {
	    type = "interrupt gate";

	    break;
	  }
	case ARCHITECTURE_IDTE_TRAP:
	  {
	    type = "trap gate";

	    break;
	  }
	default:
	  MACHINE_ESCAPE("unknown gate type");
	}

      /*
       * e)
       */

      module_call(console, message,
		  '#', "  %u: offset(0x%08x) selector(0x%x) type(%s) "
		  "privilege(%u) flags(%s)\n",
		  i, offset, selector, type, privilege, flags);
    }

  MACHINE_LEAVE();
}

/*
 * this function builds an IDT according to the given parameters.
 *
 * note that the base address is either physical or virtual depending
 * on the activation of the paging mechanism though a t_paddr type is
 * used here.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) align the base address if necessary.
 * 2) construct the descriptor.
 * 3) initialize the table entries.
 */

t_status		architecture_idt_build(t_paddr		base,
					       t_uint16		size,
					       as_idt*		idt)
{
  /*
   * 0)
   */

  if (idt == NULL)
    MACHINE_ESCAPE("the 'idt' argument is null");

  if (size > ARCHITECTURE_IDT_SIZE)
    MACHINE_ESCAPE("the given size is too large as exceeding the IDT's "
		   "theoretically maximum capacity");

  /*
   * 1)
   */

  if (base % sizeof (at_idte))
    base += sizeof (at_idte) - (base % sizeof (at_idte));

  /*
   * 2)
   */

  idt->table = (at_idte*)base;
  idt->size = size;

  /*
   * 3)
   */

  memset(idt->table, 0x0, size * sizeof (at_idte));

  MACHINE_LEAVE();
}

/*
 * this function imports the given IDT making it the current IDT.
 *
 * note that the IDT base address is either physical or virtual depending
 * on the activation of the paging mechanism though the IDTR structure
 * uses a t_paddr type.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) initialize the IDT register.
 * 2) load the IDT register making the given IDT the current one.
 * 3) copy the given descriptor into the system's current one.
 */

t_status		architecture_idt_import(as_idt*		idt)
{
  as_idtr		idtr;

  /*
   * 0)
   */

  if (idt == NULL)
    MACHINE_ESCAPE("the 'idt' argument is null");

  /*
   * 1)
   */

  idtr.address = (t_paddr)idt->table;
  idtr.size = idt->size * sizeof (at_idte) - 1;

  /*
   * 2)
   */

  ARCHITECTURE_LIDT(idtr);

  /*
   * 3)
   */

  memcpy(&_event.machine.idt, idt, sizeof (as_idt));

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

t_status		architecture_idt_export(as_idt*		idt)
{
  as_idtr		idtr;
  at_idte*		source;
  at_idte*		dest;

  /*
   * 0)
   */

  if (idt == NULL)
    MACHINE_ESCAPE("the 'idt' argument is null");

  /*
   * 1)
   */

  ARCHITECTURE_SIDT(idtr);

  /*
   * 2)
   */

  source = (at_idte*)idtr.address;
  dest = idt->table;

  memcpy(dest, source, idtr.size);

  /*
   * 3)
   */

  idt->size = idtr.size / sizeof (at_idte);

  MACHINE_LEAVE();
}

/*
 * this function inserts an entry in the table, at a precise index, hence
 * possibly overwriting the previous entry.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) update the IDT entry.
 */

t_status		architecture_idt_insert(t_uint16	index,
						t_vaddr		offset,
						t_uint16	selector,
						t_flags		flags)
{
  /*
   * 0)
   */

  if (index >= _event.machine.idt.size)
    MACHINE_ESCAPE("out-of-bound insertion");

  if (_event.machine.idt.table[index] & ARCHITECTURE_IDTE_PRESENT)
    MACHINE_ESCAPE("the IDT entry to insert is already in use");

  /*
   * 1)
   */

  _event.machine.idt.table[index] =
    ARCHITECTURE_IDTE_PRESENT |
    ARCHITECTURE_IDTE_OFFSET_SET(offset) |
    ARCHITECTURE_IDTE_SELECTOR_SET(selector) |
    flags;

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

t_status		architecture_idt_delete(t_uint16	index)
{
  /*
   * 0)
   */

  if (index >= _event.machine.idt.size)
    MACHINE_ESCAPE("out-of-bound index");

  if (!(_event.machine.idt.table[index] & ARCHITECTURE_IDTE_PRESENT))
    MACHINE_ESCAPE("the IDT entry to delete is not present");

  /*
   * 1)
   */

  memset(&_event.machine.idt.table[index], 0x0, sizeof (at_idte));

  MACHINE_LEAVE();
}
