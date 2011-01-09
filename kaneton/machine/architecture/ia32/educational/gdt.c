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
 * updated       julien quintard   [sat jan  8 21:19:12 2011]
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

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the current system GDT.
 */

as_gdt_descriptor		_architecture_gdt;

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
 *   c) depending on the segment type flags, build a type string.
 *     A) the entry represents a system segment.
 *     B) the entry represents a regular code/data segment.
 *   d) display the segment.
 */

t_error			architecture_gdt_dump(void)
{
  t_uint16		i;

  /*
   * 1)
   */

  module_call(console, message,
	      '#', "GDT: table(0x%08x) size(%u)\n",
	      _architecture_gdt.table, _architecture_gdt.size);

  /*
   * 2)
   */

  for (i = 1; i < _architecture_gdt.size; i++)
    {
      t_paddr		base;
      t_paddr		limit;
      char*		type;
      at_privilege	privilege;

      /*
       * a)
       */

      if (!(_architecture_gdt.table[i].type & ARCHITECTURE_GDT_TYPE_PRESENT))
	continue;

      /*
       * b)
       */

      base =
	_architecture_gdt.table[i].base_00_15 |
	(_architecture_gdt.table[i].base_16_23 << 16) |
	(_architecture_gdt.table[i].base_24_31 << 24);

      limit =
	_architecture_gdt.table[i].limit_00_15 |
	(_architecture_gdt.table[i].limit_16_19 << 16);

      if (_architecture_gdt.table[i].flags & ARCHITECTURE_GDT_FLAG_GRANULARITY)
	limit *= ___kaneton$pagesz;

      privilege = ARCHITECTURE_GDT_DPL_GET(_architecture_gdt.table[i].type);

      /*
       * c)
       */

      if (!(_architecture_gdt.table[i].type & ARCHITECTURE_GDT_TYPE_S))
	{
	  /*
	   * A)
	   */

	  switch (ARCHITECTURE_GDT_TYPE_SYSTEM(_architecture_gdt.table[i].type))
	    {
	    case ARCHITECTURE_GDT_TYPE_LDT:
	      {
		type = "ldt";

		break;
	      }
	    case ARCHITECTURE_GDT_TYPE_TSS:
	      {
		type = "tss";

		break;
	      }
	    case ARCHITECTURE_GDT_TYPE_CALL:
	      {
		type = "call gate";

		break;
	      }
	    case ARCHITECTURE_GDT_TYPE_TRAP:
	      {
		type = "trap gate";

		break;
	      }
	    case ARCHITECTURE_GDT_TYPE_INTERRUPT:
	      {
		type = "interrupt gate";

		break;
	      }
	    default:
	      MACHINE_ESCAPE("unknown system segment type '%u'",
			     _architecture_gdt.table[i].type);
	    }
	}
      else
	{
	  /*
	   * B)
	   */

	  switch (ARCHITECTURE_GDT_TYPE_SEGMENT(_architecture_gdt.table[i].type))
	    {
	    case ARCHITECTURE_GDT_TYPE_CODE:
	      {
		type = "code";

		break;
	      }
	    case ARCHITECTURE_GDT_TYPE_DATA:
	      {
		type = "data";

		break;
	      }
	    default:
	      MACHINE_ESCAPE("unknown regular segment type '%u'",
			     _architecture_gdt.table[i].type);
	    }
	}

      /*
       * d)
       */

      module_call(console, message,
		  '#', "  %u: base(0x%08x) limit(0x%08x) type(%s) "
		  "privilege(%u)\n",
		  i, base, limit, type, privilege);
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

t_error			architecture_gdt_build(t_paddr		base,
					       t_psize		size,
					       as_gdt_descriptor* descriptor)
{
  /*
   * 0)
   */

  if (descriptor == NULL)
    MACHINE_ESCAPE("the 'descriptor' argument is null");

  if (size > (ARCHITECTURE_GDT_SIZE * sizeof(as_gdt_entry)))
    MACHINE_ESCAPE("the given size is too large as exceeding the GDT's "
		   "theoretically maximum capacity");

  /*
   * 1)
   */

  if (base % sizeof(as_gdt_entry))
    base += sizeof(as_gdt_entry) - (base % sizeof(as_gdt_entry));

  /*
   * 2)
   */

  descriptor->table = (as_gdt_entry*)base;
  descriptor->size = size / sizeof(as_gdt_entry);

  /*
   * 3)
   */

  memset(descriptor->table, 0x0, descriptor->size * sizeof(as_gdt_entry));

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

t_error			architecture_gdt_import(as_gdt_descriptor* descriptor)
{
  as_gdt_register	gdtr;

  /*
   * 0)
   */

  if (descriptor == NULL)
    MACHINE_ESCAPE("the 'descriptor' argument is null");

  /*
   * 1)
   */

  gdtr.address = (t_paddr)descriptor->table;
  gdtr.size = descriptor->size * sizeof(as_gdt_entry);

  /*
   * 2)
   */

  ARCHITECTURE_LGDT(gdtr);

  /*
   * 3)
   */

  memcpy(&_architecture_gdt, descriptor, sizeof(as_gdt_descriptor));

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

t_error			architecture_gdt_export(as_gdt_descriptor* descriptor)
{
  as_gdt_register	gdtr;
  as_gdt_entry*		source;
  as_gdt_entry*		dest;

  /*
   * 0)
   */

  if (descriptor == NULL)
    MACHINE_ESCAPE("the 'descriptor' argument is null");

  /*
   * 1)
   */

  ARCHITECTURE_SGDT(gdtr);

  /*
   * 2)
   */

  source = (as_gdt_entry*)gdtr.address;
  dest = descriptor->table;

  memcpy(dest, source, gdtr.size);

  /*
   * 3)
   */

  descriptor->size = gdtr.size / sizeof(as_gdt_entry);

  MACHINE_LEAVE();
}

/*
 * this function inserts a segment in the table, at a precise index, hence
 * possibly erasing a previously recorded segment.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) set the base address.
 * 2) set the type according to the given class.
 * 3) set the flags.
 * 4) compute the limit address accordin to the granularity flag.
 */

t_error			architecture_gdt_insert(t_uint16	index,
						t_paddr		base,
						t_paddr		limit,
						at_privilege	privilege,
						at_gdt_class	class,
						at_gdt_type	type)
{
  t_paddr		l;

  /*
   * 0)
   */

  if (index >= _architecture_gdt.size)
    MACHINE_ESCAPE("out-of-bound insertion");

  if (index == 0)
    MACHINE_ESCAPE("the first GDT entry cannot be used");

  /*
   * 1)
   */

  _architecture_gdt.table[index].base_00_15 = base & 0xffff;
  _architecture_gdt.table[index].base_16_23 = (base >> 16) & 0xff;
  _architecture_gdt.table[index].base_24_31 = (base >> 24) & 0xff;

  /*
   * 2)
   */

  _architecture_gdt.table[index].type =
    ARCHITECTURE_GDT_TYPE_PRESENT |
    ARCHITECTURE_GDT_DPL_SET(privilege);

  switch (class)
    {
    case ARCHITECTURE_GDT_CLASS_SYSTEM:
      {
	_architecture_gdt.table[index].type |= type;

	break;
      }
    case ARCHITECTURE_GDT_CLASS_SEGMENT:
      {
	_architecture_gdt.table[index].type |= ARCHITECTURE_GDT_TYPE_S | type;

	break;
      }
    default:
      {
	MACHINE_ESCAPE("unknown segment class '%u'",
		       class);
      }
    }

  /*
   * 3)
   */

  _architecture_gdt.table[index].flags = ARCHITECTURE_GDT_FLAG_32BIT;

  /*
   * 4)
   */

  if (limit >= ___kaneton$pagesz)
    {
      _architecture_gdt.table[index].flags |=
	ARCHITECTURE_GDT_FLAG_GRANULARITY;

      l = limit / ___kaneton$pagesz;
    }
  else
    {
      l = limit;
    }

  _architecture_gdt.table[index].limit_00_15 = l & 0xffff;
  _architecture_gdt.table[index].limit_16_19 = (l >> 16) & 0xf;

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

t_error			architecture_gdt_reserve(t_paddr	base,
						 t_paddr	limit,
						 at_privilege	privilege,
						 at_gdt_class	class,
						 at_gdt_type	type,
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

  for (i = 1; i < _architecture_gdt.size; i++)
    if (!(_architecture_gdt.table[i].type & ARCHITECTURE_GDT_TYPE_PRESENT))
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
			      base, limit, privilege, class, type) != ERROR_OK)
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

t_error			architecture_gdt_delete(t_uint16	index)
{
  /*
   * 0)
   */

  if (index >= _architecture_gdt.size)
    MACHINE_ESCAPE("out-of-bound insertion");

  if (index == 0)
    MACHINE_ESCAPE("the first GDT entry cannot be used");

  /*
   * 1)
   */

  memset(&_architecture_gdt.table[index], 0x0, sizeof(as_gdt_entry));

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

t_error			architecture_gdt_selector(t_uint16	index,
						  at_privilege	privilege,
						  t_uint16*	selector)
{
  /*
   * 0)
   */

  if (selector == NULL)
    MACHINE_ESCAPE("the 'selector' argument is null");

  if (index >= _architecture_gdt.size)
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
