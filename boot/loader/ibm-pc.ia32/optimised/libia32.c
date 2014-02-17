/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...otloader/ibm-pc.ia32/optimised/libia32.c
 *
 * created       matthieu bucchianeri   [mon jun 25 22:52:22 2007]
 * updated       matthieu bucchianeri   [mon dec 24 18:18:31 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * gdt table pointer
 */

t_ia32_gdt		ia32_gdt;

/*
 * active page directory.
 */

t_ia32_directory	ia32_pd;

/*
 * idt table pointer
 */

t_ia32_idt		ia32_idt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * sets code/data/stack segment registers.
 */

t_status		pmode_set_segment_registers(t_uint16	seg_code,
					    t_uint16	seg_data)
{
  asm volatile("pushl %0\n\t"
	       "pushl $1f\n\t"
	       "lret\n\t"
	       "1:\n\t"
	       "movw %1, %%ax\n\t"
	       "movw %%ax, %%ds\n\t"
	       "movw %%ax, %%ss\n\t"
	       "movw %%ax, %%es\n\t"
	       "movw %%ax, %%fs\n\t"
	       "movw %%ax, %%gs\n\t"
	       :
	       : "g" (seg_code), "g" (seg_data)
	       : "memory", "%eax"
	       );

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

t_status			gdt_build(t_uint16		entries,
				  t_paddr		base,
				  t_ia32_gdt*		gdt,
				  t_uint8		clear)
{

  /*
   * 1)
   */

  if (entries > IA32_GDT_MAX_ENTRIES)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  if (base % sizeof(t_ia32_gdte))
    {
      base += sizeof(t_ia32_gdte) - (base % sizeof(t_ia32_gdte));
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
      memset(gdt->descriptor, 0, entries * sizeof(t_ia32_gdte));
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

t_status			gdt_activate(t_ia32_gdt		new_gdt)
{
  t_ia32_gdtr		gdtr;

  /*
   * 1)
   */

  gdtr.address = (t_paddr)new_gdt.descriptor;
  gdtr.size = new_gdt.count * sizeof(t_ia32_gdte);
  LGDT(gdtr);

  /*
   * 2)
   */

  ia32_gdt.descriptor = new_gdt.descriptor;
  ia32_gdt.count = new_gdt.count;

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

t_status			gdt_add_segment(t_ia32_gdt*	table,
					t_uint16	segment,
					t_ia32_segment	descriptor)
{
  t_uint32		size;

  /*
   * 1)
   */

  if (!table)
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
 * builds a selector with specified segment and privileges.
 *
 * steps:
 *
 * 1) checks segment entry boundary.
 * 2) builds the selector.
 */

t_status			gdt_build_selector(t_uint16	segment,
					   t_ia32_prvl	privilege,
					   t_uint16*	selector)
{

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

/*
 * builds a new page directory.
 *
 * steps:
 *
 * 1) checks address alignement.
 * 2) fills the record.
 * 3) clears the directory if needed.
 */

t_status			pd_build(t_paddr			base,
				 t_ia32_directory*		directory,
				 t_uint8			clear)
{

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  *directory = (t_ia32_directory)base;

  /*
   * 3)
   */

  if (clear)
    {
      memset((void*)base, 0, IA32_PD_MAX_ENTRIES * sizeof(t_ia32_pde));
    }

  return STATUS_OK;
}

/*
 * activates a directory.
 *
 * steps:
 *
 * 1) computes the pdbr value.
 * 2) loads the pdbr.
 * 3) sets the global variable.
 */

t_status			pd_activate(t_ia32_directory	dir,
				    t_uint32		cached,
				    t_uint32		writeback)
{
  t_uint32		pdbr;
  t_uint32		mask = 0xfffff000;

  /*
   * 1)
   */

  if (cached == IA32_PD_NOTCACHED)
    mask |= (1 << 4);
  if (writeback == IA32_PD_WRITETHROUGH)
    mask |= (1 << 3);

  pdbr = ((t_uint32)dir & mask);

  /*
   * 2)
   */

  LCR3(pdbr);

  /*
   * 3)
   */

  ia32_pd = dir;

  return STATUS_OK;
}

/*
 * adds a table to a directory.
 *
 * steps:
 *
 * 1) gets the directory address.
 * 2) setups the entry.
 * 3) adds the entry.
 */


t_status			pd_add_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table	table)
{
  t_ia32_pde*		d;
  t_uint32		opts = 0;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (table.present)
    opts |= IA32_PDE_FLAG_P;

  if (table.cached == IA32_PT_NOTCACHED)
    opts |= IA32_PDE_FLAG_CD;
  if (table.writeback == IA32_PT_WRITETHROUGH)
    opts |= IA32_PDE_FLAG_WT;

  opts |= (table.rw == IA32_PT_WRITABLE ? IA32_PDE_FLAG_RW : IA32_PDE_FLAG_RO);

  opts |= (table.user == IA32_PT_USER ? IA32_PDE_FLAG_USER : IA32_PDE_FLAG_SUPERVISOR);

  opts |= IA32_PDE_FLAG_USED;

  /*
   * 3)
   */

  d[entry] = IA32_MK_BASE(table.entries) | opts;

  return STATUS_OK;
}

/*
 * gets a table entry from a directory.
 *
 * steps:
 *
 * 1) gets the directory address.
 * 2) checks entry validity.
 * 3) fills the page record.
 */

t_status			pd_get_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table*	table)
{
  t_ia32_directory	d;

  /*
   * 1)
   */

  if (dir)
    d = *dir;
  else
    d = ia32_pd;

  /*
   * 2)
   */

  if (!(d[entry] & IA32_PDE_FLAG_USED))
    return STATUS_UNKNOWN_ERROR;

  /*
   * 3)
   */

  table->rw = (d[entry] & IA32_PDE_FLAG_RW) ?
    IA32_PT_WRITABLE : IA32_PT_READONLY;
  table->present = !!(d[entry] & IA32_PDE_FLAG_P);
  table->user = (d[entry] & IA32_PDE_FLAG_USER) ?
    IA32_PT_USER : IA32_PT_PRIVILEGED;
  table->writeback = (d[entry] & IA32_PDE_FLAG_WT) ?
    IA32_PT_WRITETHROUGH : IA32_PT_WRITEBACK;
  table->cached = (d[entry] & IA32_PDE_FLAG_CD) ?
    IA32_PT_NOTCACHED : IA32_PT_CACHED;
  table->entries = IA32_MK_BASE(d[entry]);

  return STATUS_OK;
}

/*
 * builds a new page table.
 *
 * steps:
 *
 * 1) checks address encoding.
 * 2) initializes the structure.
 * 3) clears the table if necessary.
 */

t_status			pt_build(t_paddr		base,
				 t_ia32_table*		table,
				 t_uint8		clear)
{

  /*
   * 1)
   */

  if (IA32_MK_BASE(base) != base)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  table->entries = base;

  /*
   * 3)
   */

  if (clear)
    {
      memset((void*)base, 0, IA32_PT_MAX_ENTRIES * sizeof(t_ia32_pte));
    }

  return STATUS_OK;
}

/*
 * adds a page to a table.
 *
 * steps:
 *
 * 1) checks the address of the page.
 * 2) gets the table pointer.
 * 3) setups the entry.
 */

t_status			pt_add_page(t_ia32_table*	tab,
				    t_uint16		entry,
				    t_ia32_page		page)
{
  t_ia32_pte*		t;
  t_uint32		opts = 0;

  /*
   * 1)
   */

  if (IA32_MK_BASE(page.addr) != (t_uint32)page.addr)
    return STATUS_UNKNOWN_ERROR;

  /*
   * 2)
   */

  t = (t_ia32_pte*)tab->entries;

  /*
   * 3)
   */

  if (page.present)
    opts |= IA32_PTE_FLAG_P;
  else
    printf ("warning: adding non-present page\n");

  if (page.cached == IA32_PG_NOTCACHED)
    opts |= IA32_PDE_FLAG_CD;
  if (page.writeback == IA32_PG_WRITETHROUGH)
    opts |= IA32_PDE_FLAG_WT;

  opts |= (page.rw == IA32_PG_WRITABLE ? IA32_PTE_FLAG_RW : IA32_PTE_FLAG_RO);
  opts |= (page.user == IA32_PG_USER ?
	   IA32_PTE_FLAG_USER : IA32_PTE_FLAG_SUPERVISOR);

  opts |= (page.global == IA32_PG_GLOBAL ? IA32_PTE_FLAG_G : 0);

  opts |= IA32_PTE_FLAG_USED;

  t[entry] = IA32_MK_BASE(page.addr) | opts;

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

t_status			idt_build(t_uint16		entries,
				  t_paddr		base,
				  t_uint8		clear,
				  t_ia32_idt*		table)
{
  /*
   * 1)
   */

  if (entries > IA32_IDT_MAX_ENTRIES)
    return STATUS_UNKNOWN_ERROR;

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

t_status			idt_activate(t_ia32_idt*		table)
{
  t_ia32_idtr		idtr;

  /*
   * 1)
   */

  ia32_idt.descriptor = table->descriptor;
  ia32_idt.count = table->count;

  /*
   * 2
   */

  idtr.address = (t_paddr)ia32_idt.descriptor;
  idtr.size = ia32_idt.count * sizeof(t_ia32_idte);
  LIDT(idtr);

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

t_status			idt_add_gate(t_ia32_idt*		table,
				     t_uint16			index,
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
 * initialize the pics
 *
 * steps:
 *
 * 1) send ICW1
 * 2) send ICW2
 * 3) send ICW3
 * 4) send ICW4
 * 5) all interrupts are set off at initialization
 *
 */

t_status			pic_init(void)
{
  /*
   * 1)
   */

  OUTB(IBMPC_MASTER_PORT_A, IBMPC_MASTER_ICW1);
  OUTB(IBMPC_SLAVE_PORT_A, IBMPC_SLAVE_ICW1);

  /*
   * 2)
   */

  OUTB(IBMPC_MASTER_PORT_B, IBMPC_MASTER_ICW2);
  OUTB(IBMPC_SLAVE_PORT_B, IBMPC_SLAVE_ICW2);

  /*
   * 3)
   */

  OUTB(IBMPC_MASTER_PORT_B, IBMPC_MASTER_ICW3);
  OUTB(IBMPC_SLAVE_PORT_B, IBMPC_SLAVE_ICW3);

  /*
   * 4)
   */

  OUTB(IBMPC_MASTER_PORT_B, IBMPC_MASTER_ICW4);
  OUTB(IBMPC_SLAVE_PORT_B, IBMPC_SLAVE_ICW4);

  /*
   * 5)
   */

  OUTB(IBMPC_MASTER_PORT_B, 0xfb);
  OUTB(IBMPC_SLAVE_PORT_B, 0xff);

  return (STATUS_OK);
}

/*
 * turn on a given interrupt enabling its flag
 */

t_status			pic_enable_irq(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    return (STATUS_UNKNOWN_ERROR);

  if (irq < 8)
    {
      INB(IBMPC_MASTER_PORT_B, mask);
      OUTB(IBMPC_MASTER_PORT_B, mask & ~(1 << irq));
    }
  else
    {
      INB(IBMPC_SLAVE_PORT_B, mask);
      OUTB(IBMPC_SLAVE_PORT_B, mask & ~(1 << (irq - 8)));
    }

  return (STATUS_OK);
}

/*
 * turn off a given interrupt
 */

t_status			pic_disable_irq(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    return (STATUS_UNKNOWN_ERROR);

  if (irq < 8)
    {
      INB(IBMPC_MASTER_PORT_B, mask);
      OUTB(IBMPC_MASTER_PORT_B, mask | (1 << irq));
    }
  else
    {
      INB(IBMPC_SLAVE_PORT_B, mask);
      OUTB(IBMPC_SLAVE_PORT_B, mask | (1 << (irq - 8)));
    }

  return (STATUS_OK);
}

/*
 * acknowlegde the pics
 *
 * steps:
 *
 * 1) check irq bounds
 * 2) send normal EOI
 */

t_status			pic_acknowledge(t_uint8		irq)
{
  t_uint8		mask;

  /*
   * 1)
   */

  if (irq > 15)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (irq < 8)
    {
      INB(IBMPC_MASTER_PORT_B, mask);
      OUTB(IBMPC_MASTER_PORT_B, mask);
      OUTB(IBMPC_MASTER_PORT_A, 0x60 + irq);
    }
  else
    {
      INB(IBMPC_SLAVE_PORT_B, mask);
      OUTB(IBMPC_SLAVE_PORT_B, mask);
      OUTB(IBMPC_SLAVE_PORT_A, 0x60 + (irq - 8));
      OUTB(IBMPC_MASTER_PORT_A, 0x60 + 2);
    }

  return (STATUS_OK);
}

/*
 * initialize the pit 8254.
 *
 * steps:
 *
 * 1) calculate and check counter limit for the given frequency.
 * 2) the pit 8254 interprets value 0 as 65536.
 * 3) setup the timer0 to run in mode 2 (rate generator).
 */

t_status			pit_init(t_uint32	freq)
{
  t_uint32	latch;

  /*
   * 1)
   */

  latch = IBMPC_CLOCK_TICK_RATE / freq;

  if (!latch || latch > 65536)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (latch == 65536)
    latch = 0;

  /*
   * 3)
   */

  OUTB(IBMPC_PIT_8254_CTRL, 0x34);
  OUTB(IBMPC_TIMER_0, latch & 0xFF);
  OUTB(IBMPC_TIMER_0, (latch >> 8) & 0xFF);

  return (STATUS_OK);
}

/*
 * tell if the CPU embed a local APIC
 */

t_uint32		cpuid_has_apic(void)
{
  t_uint32		eax;
  t_uint32		ebx;
  t_uint32		ecx;
  t_uint32		edx;

  CPUID(0x1, eax, ebx, ecx, edx);

  return (!!(edx & (1 << 9)));
}
