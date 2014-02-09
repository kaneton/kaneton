/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hine/architecture/ia32/educational/gdt.c
 *
 * created       matthieu bucchianeri   [mon dec 10 13:54:28 2007]
 * updated       julien quintard   [sun jan 30 20:44:09 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functionalities for managing the GDT - Global Descriptor
 * Table.
 *
 * note that the educational implementation retrieves the GDT provided
 * by the boot loader in order to re-use it. however, the entries set
 * by the boot loaders are then replaced by the kernel's.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the segment manager which contains the current GDT.
 */

extern m_segment	_segment;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps a GDT given by its structure's address.
 *
 * steps:
 *
 * 1) display a header message.
 * 2) go through the GDT entries.
 *   a) ignore non-present entries.
 *   b) compute the base and limit addresses.
 *   c) build the flags string.
 *   d) depending on the segment type flags, build a type string.
 *     A) the entry represents a system segment.
 *     B) the entry represents a regular code/data segment.
 *   e) display the segment.
 */

t_status		architecture_gdt_dump(void)
{
  t_uint16		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "GDT: table(0x%08x) size(%u)\n",
	      _segment.machine.gdt.table,
	      _segment.machine.gdt.size);

  /*
   * 2)
   */

  for (i = 0; i < _segment.machine.gdt.size; i++)
    {
      t_privilege	privilege;
      t_paddr		base;
      t_paddr		limit;
      char*		type;
      char		flags[4];

      /*
       * a)
       */

      if (!(_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_PRESENT))
	continue;

      /*
       * b)
       */

      base = ARCHITECTURE_GDTE_BASE_GET(_segment.machine.gdt.table[i]);

      limit = ARCHITECTURE_GDTE_LIMIT_GET(_segment.machine.gdt.table[i]);

      if (_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_GRANULARITY)
	limit *= ___kaneton$pagesz;

      privilege = ARCHITECTURE_GDTE_DPL_GET(_segment.machine.gdt.table[i]);

      /*
       * c)
       */

      if (_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_AVAILABLE)
	flags[0] = 'f';
      else
	flags[0] = '.';

      if (_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_32BIT)
	flags[1] = 's';
      else
	flags[1] = '.';

      if (_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_GRANULARITY)
	flags[2] = 'g';
      else
	flags[2] = '.';

      flags[3] = '\0';

      /*
       * d)
       */

      if (!(_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_S))
	{
	  /*
	   * A)
	   */

	  switch (ARCHITECTURE_GDTE_TYPE_SYSTEM(_segment.machine.gdt.table[i]))
	    {
	    case ARCHITECTURE_GDTE_LDT:
	      {
		type = "ldt";

		break;
	      }
	    case ARCHITECTURE_GDTE_TSS_AVAILABLE:
	    case ARCHITECTURE_GDTE_TSS_BUSY:
	      {
		type = "tss";

		break;
	      }
	    case ARCHITECTURE_GDTE_CALL:
	      {
		type = "call gate";

		break;
	      }
	    case ARCHITECTURE_GDTE_TRAP:
	      {
		type = "trap gate";

		break;
	      }
	    case ARCHITECTURE_GDTE_INTERRUPT:
	      {
		type = "interrupt gate";

		break;
	      }
	    default:
	      MACHINE_ESCAPE("unknown system segment type");
	    }
	}
      else
	{
	  /*
	   * B)
	   */

	  switch (ARCHITECTURE_GDTE_TYPE_SEGMENT(_segment.machine.gdt.table[i]))
	    {
	    case ARCHITECTURE_GDTE_CODE:
	      {
		type = "code";

		break;
	      }
	    case ARCHITECTURE_GDTE_DATA:
	      {
		type = "data";

		break;
	      }
	    default:
	      MACHINE_ESCAPE("unknown regular segment type");
	    }
	}

      /*
       * e)
       */

      module_call(console, message,
		  '#', "  %u: base(0x%08x) limit(0x%08x) type(%s) "
		  "privilege(%u) flags(%s)\n",
		  i, base, limit, type, privilege, flags);
    }

  MACHINE_LEAVE();
}

/*
 * this function builds a GDT according to the given parameters.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) align the base address if necessary.
 * 2) initialized the gdt structure.
 * 3) initialize the table's memory.
 */

t_status		architecture_gdt_build(t_paddr		base,
					       t_psize		size,
					       as_gdt*		gdt)
{
  /*
   * 0)
   */

  if (gdt == NULL)
    MACHINE_ESCAPE("the 'gdt' argument is null");

  if (size > (ARCHITECTURE_GDT_SIZE * sizeof (at_gdte)))
    MACHINE_ESCAPE("the given size is too large as exceeding the GDT's "
		   "theoretically maximum capacity");

  /*
   * 1)
   */

  if (base % sizeof (at_gdte))
    base += sizeof (at_gdte) - (base % sizeof (at_gdte));

  /*
   * 2)
   */

  gdt->table = (at_gdte*)base;
  gdt->size = size / sizeof (at_gdte);

  /*
   * 3)
   */

  memset(gdt->table, 0x0, gdt->size * sizeof (at_gdte));

  MACHINE_LEAVE();
}

/*
 * this function imports the given GDT, making it the current GDT.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) set the GDT register according to the given GDT.
 * 2) load the GDT.
 * 3) set the given GDT as being the current one by copying its content
 *    in the current system GDT's structure.
 */

t_status		architecture_gdt_import(as_gdt*		gdt)
{
  as_gdtr		gdtr;

  /*
   * 0)
   */

  if (gdt == NULL)
    MACHINE_ESCAPE("the 'gdt' argument is null");

  /*
   * 1)
   */

  gdtr.address = (t_paddr)gdt->table;
  gdtr.size = gdt->size * sizeof (at_gdte);

  /*
   * 2)
   */

  ARCHITECTURE_LGDT(gdtr);

  /*
   * 3)
   */

  memcpy(&_segment.machine.gdt, gdt, sizeof (as_gdt));

  MACHINE_LEAVE();
}

/*
 * this function exports the current GDT by copying its content in the
 * given structure.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the GDT register's value.
 * 2) copy the current GDT's content into the given one.
 * 3) updates the given GDT's size.
 */

t_status		architecture_gdt_export(as_gdt*		gdt)
{
  as_gdtr		gdtr;
  at_gdte*		source;
  at_gdte*		dest;

  /*
   * 0)
   */

  if (gdt == NULL)
    MACHINE_ESCAPE("the 'gdt' argument is null");

  /*
   * 1)
   */

  ARCHITECTURE_SGDT(gdtr);

  /*
   * 2)
   */

  source = (at_gdte*)gdtr.address;
  dest = gdt->table;

  memcpy(dest, source, gdtr.size);

  /*
   * 3)
   */

  gdt->size = gdtr.size / sizeof (at_gdte);

  MACHINE_LEAVE();
}

/*
 * this function inserts a segment in the table, at a precise index, hence
 * possibly erasing a previously recorded segment.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) update the GDT entry.
 * 2) set the limit and granularity according to its value.
 */

t_status		architecture_gdt_insert(t_uint16	index,
						t_paddr		base,
						t_paddr		limit,
						t_flags		flags)
{
  /*
   * 0)
   */

  if (index >= _segment.machine.gdt.size)
    MACHINE_ESCAPE("out-of-bound insertion");

  if (index == 0)
    MACHINE_ESCAPE("the first GDT entry cannot be used");

  if (_segment.machine.gdt.table[index] & ARCHITECTURE_GDTE_PRESENT)
    MACHINE_ESCAPE("the GDT entry to update is already in use");

  /*
   * 1)
   */

  _segment.machine.gdt.table[index] =
    ARCHITECTURE_GDTE_PRESENT |
    ARCHITECTURE_GDTE_BASE_SET(base) |
    ARCHITECTURE_GDTE_32BIT |
    flags;

  /*
   * 2)
   */

  if (limit >= ___kaneton$pagesz)
    {
      _segment.machine.gdt.table[index] |=
	ARCHITECTURE_GDTE_GRANULARITY |
	ARCHITECTURE_GDTE_LIMIT_SET(limit / ___kaneton$pagesz);
    }
  else
    {
      _segment.machine.gdt.table[index] |= ARCHITECTURE_GDTE_LIMIT_SET(limit);
    }

  MACHINE_LEAVE();
}

/*
 * this function reserves an available slot for the given segment.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) look for an available slot.
 * 2) insert the segment in the GDT.
 */

t_status		architecture_gdt_reserve(t_paddr	base,
						 t_paddr	limit,
						 t_flags	flags,
						 t_uint16*	index)
{
  t_uint16		i;

  /*
   * 0)
   */

  if (index == NULL)
    MACHINE_ESCAPE("the 'index' argument is null");

  /*
   * 1)
   */

  *index = 0;

  for (i = 1; i < _segment.machine.gdt.size; i++)
    if (!(_segment.machine.gdt.table[i] & ARCHITECTURE_GDTE_PRESENT))
      {
	*index = i;

	break;
      }

  if (*index == 0)
    MACHINE_ESCAPE("unable to find an available GDT entry");

  /*
   * 2)
   */

  if (architecture_gdt_insert(*index,
			      base, limit, flags) != STATUS_OK)
    MACHINE_ESCAPE("unable to insert the segment in the GDT");

  MACHINE_LEAVE();
}

/*
 * this function deletes a GDT entry, making it available.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) re-initialize the GDT entry's memory.
 */

t_status		architecture_gdt_delete(t_uint16	index)
{
  /*
   * 0)
   */

  if (index >= _segment.machine.gdt.size)
    MACHINE_ESCAPE("out-of-bound insertion");

  if (index == 0)
    MACHINE_ESCAPE("the first GDT entry cannot be used");

  if (!(_segment.machine.gdt.table[index] & ARCHITECTURE_GDTE_PRESENT))
    MACHINE_ESCAPE("the GDT entry to delete is not present");

  /*
   * 1)
   */

  memset(&_segment.machine.gdt.table[index], 0x0, sizeof (at_gdte));

  MACHINE_LEAVE();
}

/*
 * this function builds a segment selector according to the given
 * parameters.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) compute the selector according to the given parameters.
 */

t_status		architecture_gdt_selector(t_uint16	index,
						  t_privilege	privilege,
						  t_uint16*	selector)
{
  /*
   * 0)
   */

  if (selector == NULL)
    MACHINE_ESCAPE("the 'selector' argument is null");

  if (index >= _segment.machine.gdt.size)
    MACHINE_ESCAPE("out-of-bound index");

  /*
   * 1)
   */

  *selector =
    ARCHITECTURE_GDT_SELECTOR_TI_GDT |
    ARCHITECTURE_GDT_SELECTOR_RPL(privilege) |
    ARCHITECTURE_GDT_SELECTOR_INDEX(index);

  MACHINE_LEAVE();
}
