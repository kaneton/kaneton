/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/bootloader.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:13 2007]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H		1

#define		machine_data(_object_)					\
  machine_data_##_object_()

#define		machine_data_init()					\
  struct								\
  {									\
    t_ia32_gdt			gdt;					\
    t_ia32_idt			idt;					\
    t_ia32_directory		pd;					\
  }

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "init.h"

#include "multiboot.h"
#include "isa.h"
#include "elf.h"
#include "libia32.h"
#include "libc.h"
#include "cons.h"
#include "apic.h"
#include "spinlock.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_PMODE_GDT_ENTRIES		256

#define IA32_PMODE_BOOTLOADER_CS	0x1
#define IA32_PMODE_BOOTLOADER_DS	0x2

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	apic.c
 *      bootloader.c
 *      cons.c
 *      init.c
 *	interrupt.c
 *	mp.c
 *      paging.c
 *      pmode.c
 */

/*
 * apic.c
 */

void	bootloader_apic_calibrate_tick(void);

/*
 * this function calibrates the APIC timer.
 *
 * steps:
 *
 * 1) setup the timer for one shot with a divisor equal to 1.
 * 2) setup the initial count to a big number.
 * 3) wait for the sample amout of time.
 * 4) compute the time reference.
 */

void			bootloader_apic_calibrate_timer(void);

void			bootloader_apic_wait(t_uint32			delay);

void			apic_write(t_uint32		reg,
				   t_uint32		value);

t_uint32		apic_read(t_uint32			reg);

t_uint32		apic_id(void);

void			apic_enable(void);

void			ipi_send_init(void);

void			ipi_send_startup(void);

void			ipi_send_vector(t_uint8		vector,
					t_ipi_dest	dest,
					t_id		cpu);

void			ipi_acknowledge(void);


/*
 * bootloader.c
 */

void			bootloader_error(void);

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi);


/*
 * cons.c
 */

void			bootloader_cons_clear(void);

void			bootloader_cons_scroll(t_uint16		lines);

void			bootloader_cons_attr(t_sint8		attr);

int			bootloader_cons_print_char(char		c);

void			bootloader_cons_print_string(char*	string);

void			bootloader_cons_msg(char		indicator,
					    char*		fmt,
					    ...);

int			bootloader_cons_init(void);


/*
 * init.c
 */

void			bootloader_init_dump(void);

void			bootloader_init_segments(void);

void			bootloader_init_regions(void);

t_paddr			bootloader_init_alloc(t_psize		size,
					      t_psize*		psize);

t_vaddr			bootloader_init_relocate(multiboot_info_t*	mbi);


/*
 * interrupt.c
 */

extern void bootloader_interrupt_dummy(void);

/*
 * this function initialize interrupts for the boot processor.
 *
 * steps:
 *
 * 1) build an IDT.
 * 2) load it.
 * 3) setup default handlers.
 * 4) setup the PIC and activate interrupts.
 */

void			bootloader_interrupt_init(void);

void			bootloader_interrupt_ap_init(void);


/*
 * mp.c
 */

void			bootloader_mp_init(void);

t_sint32		bootloader_get_cpu(t_uint32		apicid);

t_sint32		bootloader_add_cpu(t_uint32		apicid);

void			bootloader_mp_ap_init(void);


/*
 * paging.c
 */

void			bootloader_paging_init(void);

void			bootloader_paging_ap_init(void);


/*
 * pmode.c
 */

void			bootloader_pmode_init(void);

void			bootloader_pmode_ap_init(void);


/*
 * eop
 */

#endif
