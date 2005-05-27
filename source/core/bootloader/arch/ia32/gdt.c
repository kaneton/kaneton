/*
 * kaneton
 *
 * gdt.c
 *
 * path          /home/mycure/data/research/projects/kaneton/source/core/bootloader/arch/machdep
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Fri May 27 12:02:40 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

t_gdt* gdt = (t_gdt *) PMAP_GDT_ADDR;
t_gdtr gdtr;

voidgdt_init()
{
  t_gdtksegcode;
  t_gdtksegdata;
  t_gdtusegcode;
  t_gdtusegdata;

  memset(gdt, 0, sizeof (t_gdt) * GDT_ENTRIES);
  memset(&ksegcode, 0, sizeof (t_gdt));
  memset(&ksegdata, 0, sizeof (t_gdt));
  memset(&usegcode, 0, sizeof (t_gdt));
  memset(&usegdata, 0, sizeof (t_gdt));
  memset(&gdtr, 0, sizeof(t_gdtr));

  /*
   * init gdt kernel and user code/data descriptor
   */

  gdt_get_segment(&ksegcode, 0xfffff, 0,
		    GDT_GRAN | GDT_USE32 | GDT_DPL0 | GDT_SYS | GDT_PRESENT
		  | GDT_TYPE_CODE);
  gdt_get_segment(&ksegdata, 0xfffff, 0,
		    GDT_GRAN | GDT_USE32 | GDT_DPL0 | GDT_SYS | GDT_PRESENT
		  | GDT_TYPE_DATA);
  gdt_get_segment(&usegcode, 0xfffff, 0,
		    GDT_GRAN | GDT_USE32 | GDT_DPL3 | GDT_SYS | GDT_PRESENT
		  | GDT_TYPE_CODE);
  gdt_get_segment(&usegdata, 0xfffff, 0,
		    GDT_GRAN | GDT_USE32 | GDT_DPL3 | GDT_SYS | GDT_PRESENT
		  | GDT_TYPE_DATA);

  gdt_set_segment(gdt, &ksegcode);
  gdt_set_segment(gdt, &ksegdata);
  gdt_set_segment(gdt, &usegcode);
  gdt_set_segment(gdt, &usegdata);

  gdtr.size = (u_int16_t) 5 * sizeof (t_gdt);
  gdtr.addr = (u_int32_t) gdt;

  gdt_install();

  gdt_cpu(KERNEL_CS, KERNEL_DS);

  gdt_protected();
}

voidgdt_reinit()
{
  gdtr.size = (u_int16_t) 5 * sizeof (t_gdt);
  gdtr.addr = (u_int32_t) VMAP_GDT_ADDR;

  gdt_install();
  gdt_cpu(KERNEL_CS, KERNEL_DS);
}

voidgdt_protected()
{
  __asm__ __volatile__ ("mov %%cr0,%%eax\n"
			"or %%ax,1\n"
			"mov %%eax, %%cr0\n"
			: :);
}

voidgdt_install()
{
  __asm__ __volatile__ ("lgdt %0":: "m" (gdtr));
}

voidgdt_cpu(intsegcode,
	    intsegdata)
{
  __asm__ __volatile__ ("pushl %0\n"
			"pushl $cs_jump\n"
			"lret\n"
                        "cs_jump:\n"
			"mov %1, %%eax\n"
			"mov %%ax, %%ds\n"
			"mov %%ax, %%es\n"
			"mov %%ax, %%fs\n"
			"mov %%ax, %%gs\n"
			"mov %%ax, %%ss\n"
			: : "g" (segcode), "g" (segdata));
}

voidgdt_get_segment(t_gdt*seg,
		    u_int32_tlimit,
		    u_int32_tbase,
		    u_int16_tflags)
{
  seg->limit_0_15 = limit & 0xffff;
  seg->flags = flags | (((limit >> 16) & 0xf) << 8);
  seg->base_0_15 = base & 0xffff;
  seg->base_16_23 = (base >> 16) & 0xff;
  seg->base_24_31 = (base >> 24) & 0xff;;
}

intgdt_set_segment(t_gdt*gdt,
		   t_gdt*seg)
{
  intind;

  for (ind = 1; ind < GDT_ENTRIES; ind++)
    {
      if (!gdt[ind].limit_0_15 &&
	    !gdt[ind].base_0_15 &&
	    !gdt[ind].base_16_23 &&
	    !gdt[ind].flags &&
	  !gdt[ind].base_24_31)
	{
	  memcpy(gdt + ind, seg, sizeof (t_gdt));
	  break;
	}
    }
  return ind * 8;
}
