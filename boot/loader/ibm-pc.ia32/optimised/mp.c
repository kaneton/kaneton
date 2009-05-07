/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-smp/mp.c
 *
 * created       matthieu bucchianeri   [tue jul 25 11:21:34 2006]
 * updated       matthieu bucchianeri   [wed mar 21 22:47:49 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * AP boot code start and end.
 */

extern char ap_boot_start;
extern char ap_boot_end;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * AP init spinlock.
 */

static SPIN_DECLARE(spin);

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function identifies and boots all the processors.
 *
 * steps:
 *
 * 1) check CPUID for MP compliance.
 * 2) calibrate the APIC timer.
 * 3) setup the BSP.
 * 4) copy the AP bootstrap code & temporary GDT.
 * 5) enable the local APIC.
 * 6) send the INIT IPI.
 * 7) send the SIPI.
 * 8) send the second SIPI.
 * 9) dump the collected data.
 */

void			bootloader_mp_init(void)
{
  t_ia32_gdtr*		gdtr = (t_ia32_gdtr*)0x7FF6;
  t_uint32*		core_ptr = (t_uint32*)0x7FFC;
  void			*temp_gdtr;
  t_uint32		apicid;

  bootloader_cons_msg('+', "initializing other processors\n");

  init->ncpus = 0;

  /*
   * 1)
   */

  if (!cpuid_has_apic())
    {
      bootloader_cons_msg('!', "non-MP compliant system\n");

      bootloader_add_cpu(0);
      return;
    }

  /*
   * 2)
   */

  bootloader_apic_calibrate_timer();

  /*
   * 3)
   */

  apicid = apic_id();
  bootloader_cons_msg('#', " BSP APIC ID: %d\n", apicid);
  bootloader_add_cpu(apicid);

  /*
   * 4)
   */

  memcpy((void*)0x8000, &ap_boot_start, &ap_boot_end - &ap_boot_start);
  *core_ptr = (t_uint32)bootloader_mp_ap_init;
  temp_gdtr = (void*)(0x8000 + &ap_boot_end - &ap_boot_start);
  gdtr->address = (t_paddr)temp_gdtr;
  gdtr->size = init->machine.gdt.count * sizeof (t_ia32_gdte);
  memcpy(temp_gdtr, init->machine.gdt.descriptor, gdtr->size);

  asm volatile("wbinvd");

  /*
   * 5)
   */

  apic_enable();

  /*
   * 6)
   */

  ipi_send_init();
  bootloader_apic_wait(10000);

  /*
   * 7)
   */

  ipi_send_startup();
  bootloader_apic_wait(200);

  /*
   * 8)
   */

  ipi_send_startup();
  bootloader_apic_wait(200);

  /*
   * 9)
   */

  bootloader_apic_wait(100000);

  bootloader_cons_msg('+', " multiprocessor ok, %d processor(s)\n",
		      init->ncpus);

  bootloader_apic_wait(1000000);
}

/*
 * this function get a processor by its APIC ID.
 */

t_sint32		bootloader_get_cpu(t_uint32		apicid)
{
  t_uint32		i;

  for (i = 0; i < init->ncpus; i++)
    if (init->cpus[i].cpuid == apicid)
      return i;
  return -1;
}

/*
 * this function is called to register a new processor.
 *
 * steps:
 *
 * 1) check if already registered.
 * 2) if too many processors, warn and abort current processor.
 * 3) add the new processor.
 */

t_sint32		bootloader_add_cpu(t_uint32		apicid)
{
  t_id			id;

  /*
   * 1)
   */

  if (bootloader_get_cpu(apicid) != -1)
    return -1;

  /*
   * 2)
   */

  if (init->ncpus == 15)
    {
      bootloader_cons_msg('!', "too many processors. ia32-smp only"
			  " manages up to 15 processors\n");
      return -1;
    }

  /*
   * 3)
   */

  id = init->ncpus;

  init->cpus[id].cpuid = apicid;

  init->ncpus++;

  return id;
}

/*
 * this function boots an AP.
 *
 * steps:
 *
 * 1) take the lock.
 * 2) add the current CPU to the MP table.
 * 3) release the lock.
 * 4) enter in halt state.
 */

void			bootloader_mp_ap_init(void)
{
  t_uint32		apicid;
  t_sint32		id;

  /*
   * 1)
   */

  SPIN_LOCK(spin);

  /*
   * 2)
   */

  apicid = apic_id();

  if ((id = bootloader_add_cpu(apicid)) >= 0)
    {
      bootloader_cons_msg('#', " AP-%d APIC ID: %d\n", init->ncpus, apicid);

      apic_enable();

      bootloader_pmode_ap_init();

      bootloader_paging_ap_init();

      bootloader_interrupt_ap_init();
    }

  /*
   * 3)
   */

  SPIN_UNLOCK(spin);

  /*
   * 4)
   */

//  HLT(); XXX why not working ?
  while(1)
    ;
}
