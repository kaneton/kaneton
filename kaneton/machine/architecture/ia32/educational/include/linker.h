/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ecture/ia32/educational/include/linker.h
 *
 * created       julien quintard   [sat oct 16 13:54:01 2010]
 * updated       julien quintard   [mon jan 10 06:32:14 2011]
 */

#ifndef ARCHITECTURE_LINKER_H
#define ARCHITECTURE_LINKER_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function is used to locate the memory address of a
 * linker-specific symbol.
 */

#define ARCHITECTURE_LINKER_SYMBOL(_symbol_)				\
  ((t_vaddr)&(_symbol_))

/*
 * this macro-function defines an attribute which is to be attached to
 * definitions in order to place them at a precise location in the kernel
 * binary image.
 *
 * this is used to place the handlers' code and data all together. indeed,
 * let us recall that in the ia32/educational implementation, every interrupt
 * being a system call, an IRQ, etc. triggers a handler shell which switches
 * to the kernel address space. then, in the kernel environment, the interrupt
 * is handled before returning to the task address space.
 *
 * this scheme has been adopted in order to avoid mapping the whole kernel
 * in every task since these mappings must be constantly adjusted whenever
 * the kernel modifies its page directory/table structure i.e
 * allocates/deallocates memory. therefore, kaneton relies on small static
 * region of memory which is mapped in every address space and contains the
 * interrupt handlers' code and data, all that is necessary to switch to
 * the kernel address space in order to treat the interrupt.
 */

#define ARCHITECTURE_LINKER_LOCATION(_section_)				\
  __attribute__ ((section(_section_)))

#endif
