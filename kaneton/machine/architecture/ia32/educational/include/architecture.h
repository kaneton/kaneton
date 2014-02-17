/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ia32/educational/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       julien quintard   [sat feb  5 12:52:42 2011]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro defines the architecture's endianness i.e little endian.
 */

#define ___kaneton$endian		ENDIAN_LITTLE

/*
 * the frame size defines the fundamental unit of physical memory. in the
 * IA32 architecture, this unit is the byte.
 */

#define ___kaneton$framesz		1

/*
 * the page size defines the fundamental unit of virtual memory. because of
 * the way the IA32 architecture handles pagination through a hierarchy of
 * page directory, page tables and page frames, the fundamental unit of
 * such memory is the page i.e a 4096-byte of aligned memory.
 */

#define ___kaneton$pagesz		4096

/*
 * this macro defines the architecture's word size i.e 32-bit.
 */

#define ___kaneton$wordsz		32

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/asm.h>
#include <architecture/context.h>
#include <architecture/environment.h>
#include <architecture/gdt.h>
#include <architecture/handler.h>
#include <architecture/idt.h>
#include <architecture/io.h>
#include <architecture/linker.h>
#include <architecture/paging.h>
#include <architecture/pd.h>
#include <architecture/pmode.h>
#include <architecture/privilege.h>
#include <architecture/pt.h>
#include <architecture/register.h>
#include <architecture/segmentation.h>
#include <architecture/tlb.h>
#include <architecture/tss.h>
#include <architecture/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure contains the architecture-manager-specific information,
 * especially regarding the whole interrupt and context switch mechanism.
 *
 * the manager's attributes have the particularity to be placed at a very
 * specific location within the kernel binary, in the '.handler_data'
 * section. note that in addition, the interrupt shells are placed in
 * the '.handler_code' section. this way, a task mapping both sections will
 * have access to enough information in order to switch to the kernel
 * environment whenever an interrupt occurs so that this interrupt can be
 * treated.
 *
 * this particularity explains why these attributes have not been included
 * in the kernel manager's machine-specific data structure i.e
 * _kernel.machine.*.
 *
 * the following details the attributes embedded in this structure.
 *
 * the 'kernel.ds' attribute (@am_handler + 0) represents the kernel DS - Data
 * Segment selector. this selector is required in order to indicate the
 * CPU where the real interrupt handler lies i.e within the kernel data
 * segment.
 *
 * the 'kernel.pdbr' (@am_handler + 2) represents the PDBR - Page Directory
 * Base Register, also referred to as CR3, to which the system must switch in
 * order to treat the interrupt. this is the kernel address space since all
 * interrupt handlers are located within the kernel's address space.
 *
 * the 'kernel.kis.base' contains the address of the stack used for treating
 * interrupts. this stack is located within the kernel and is jumped on once
 * the interrupt shell has switched the address space to the kernel's.
 *
 * this 'kernel.kis.size' contains the size of the KIS - Kernel Interrupt
 * Stack.
 *
 * the 'kernel.kis.pointer' (@ + 14) contains the stack pointer within the
 * KIS - Kernel Interrupt Stack. whenever an interrupt is to be treated, the
 * CPU stack pointer is placed at the address i.e within the KIS.
 *
 * the 'thread.pdbr' attribute (@ + 18) contains the PDBR of the thread
 * which has been interrupted.
 *
 * the 'thread.pointer' attribute (@ + 22) contains the value of the ESP
 * register. note that this value references either the interrupted thread's
 * stack or pile depending on its privileges. indeed, while the context
 * of ring0 threads is stored in their current stack (no privilege change),
 * it is stored in the pile for threads from other rings.
 *
 * note that the 'thread.*' attributes represent either the state of the
 * thread which has just been interrupted or of the thread which is about
 * to be resumed. indeed, while the ARCHITECTURE_CONTEXT_SAVE() macro-function
 * updates these attributes, the ARCHITECTURE_CONTEXT_RESTORE() uses them
 * in order to know which thread to resume. therefore, the process of
 * context switching comes down to updating two attributes: thread.pdbr and
 * thread.pointer.
 *
 * finally, the 'error' attribute represents the error code which the CPU
 * pushes on the stack for some exceptions.
 *
 * note that this structure is accessed by the ARCHITECTURE_CONTEXT_SAVE()
 * and ARCHITECTURE_CONTEXT_RESTORE() macro-functions, in assembly. since
 * the position of the attributes matter, please take care to adjust the
 * macro-functions according to your modifications.
 */

typedef struct
{
  struct
  {
    t_uint16		ds;
    at_cr3		pdbr;

    struct
    {
      t_vaddr		base;
      t_vsize		size;
      t_reg32		pointer;
    }			kis;
  }			__attribute__ ((packed)) kernel;

  struct
  {
    at_cr3		pdbr;
    t_reg32		pointer;
  }			__attribute__ ((packed)) thread;

  t_uint32		error;
}			__attribute__ ((packed)) am;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../architecture.c
 */

/*
 * ../architecture.c
 */

t_status		architecture_dump(void);


/*
 * eop
 */

#endif
