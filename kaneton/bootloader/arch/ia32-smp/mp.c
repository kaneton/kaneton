/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ia32-smp/mp.c
 *
 * created       matthieu bucchianeri   [tue jul 25 11:21:34 2006]
 * updated       matthieu bucchianeri   [tue jul 25 16:15:20 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SEM_TAKE(Sem)							\
  asm volatile("xor %%ax, %%ax\n\t"					\
	       ".L1:\n\t"						\
	       "xchg %%ax, %0\n\t"					\
	       "or %%ax, %%ax\n\t"					\
	       "jz .L1\n\t"						\
	       "dec %%ax\n\t"						\
	       "xchg %%ax, %0"						\
	       :							\
	       : "m" (Sem)						\
	       : "%ax");

#define SEM_RELE(Sem)							\
  asm volatile("xchg %%ax, %0\n\t"					\
	       "inc %%ax\n\t"						\
	       "xchg %%ax, %0"						\
	       :							\
	       : "m" (Sem)						\
	       : "%ax");

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * the processor count.
 */

static volatile t_uint32		count;

/*
 * AP init semaphore.
 */

static volatile t_uint16		sem = 1;

/*
 * MP table.
 */

struct
{
  t_uint32	id;
  /* XXX */
}		mp[15];

/*
 * ---------- functions -------------------------------------------------------
 */

#define AP_CODE_ADDR		0x8000

/*
 * AP bootstrap code. correspond to ap-boot.o file.
 */

static t_uint8 code[] = "\xfa\xb8\x5d\x80\xbb\x45\x80\x29\xd8\xa3\x5d\x80\x66\xb8\x45\x80\x00\x00\x66\xa3\x5f\x80\x0f\x01\x16\x5d\x80\x0f\x20\xc0\x0d\x01\x00\x0f\x22\xc0\xea\x29\x80\x08\x00\x66\xb8\x10\x00\x8e\xd8\x8e\xd0\x8e\xc0\x8e\xe0\x8e\xe8\xbc\x00\x90\x00\x00\xbe\xfc\x7f\x00\x00\x8b\x16\xff\xe2\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\x00\x9b\xdf\x00\xff\xff\x00\x00\x00\x93\xdf\x00\x00\x00\x00\x00\x00\x00";

/*
 * this function identifies and boots all the processors.
 *
 * steps:
 *
 * 1) check CPUID for MP compliance.
 * 2) calibrate the APIC timer.
 * 3) setup the BSP.
 * 4) copy the AP bootstrap code.
 * 5) enable the local APIC.
 * 6) send the INIT IPI.
 * 7) send the SIPI.
 * 8) send the second SIPI.
 * 9) dump the collected data.
 */

void			bootloader_mp_init(void)
{
  t_uint32*		core_ptr = (t_uint32*)0x7FFC;
  t_uint32	       apicid;

  bootloader_cons_msg('+', "initializing other processors\n");

  /*
   * 1)
   */

  // XXX check cpuid

  /*
   * 2)
   */

  bootloader_cons_msg('+', " calibrate the APIC timer\n");
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

  bootloader_cons_msg('+', " setup AP bootstrap code\n");
  memcpy((void*)AP_CODE_ADDR, code, sizeof(code));
  *core_ptr = (t_uint32)bootloader_mp_ap_init;

  WBINVD();

  /*
   * 5)
   */

  bootloader_cons_msg('+', " enable local APIC\n");
  apic_enable();

  /*
   * 6)
   */

  bootloader_cons_msg('+', " broadcast INIT IPI\n");
  ipi_send_init();
  bootloader_apic_wait(10000);

  /*
   * 7)
   */

  bootloader_cons_msg('+', " broadcast Startup IPI\n");
  ipi_send_startup();
  bootloader_apic_wait(200);

  /*
   * 8)
   */

  bootloader_cons_msg('+', " broadcast 2nd Startup IPI\n");
  ipi_send_startup();
  bootloader_apic_wait(200);

  bootloader_apic_wait(100000);

  /*
   * 9)
   */

  bootloader_cons_msg('+', " multiprocessor ok, %d processor(s)\n", count);

  while(1);
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
  t_uint32		i;

  /*
   * 1)
   */

  for (i = 0; i < count; i++)
    if (mp[i].id == apicid)
      return -1;

  /*
   * 2)
   */

  if (count == 15)
    {
      bootloader_cons_msg('!', "too many processors. kaneton-smp only"
			  " manages up to 15 processors\n");
      return -1;
    }

  /*
   * 3)
   */

  mp[count++].id = apicid;

  return count - 1;
}

/*
 * this function boots an AP.
 *
 * steps:
 *
 * 1) take the semaphore.
 * 2) add the current CPU to the MP table.
 * 3) release the semaphore.
 * 4) enter in halt state.
 */

void			bootloader_mp_ap_init(void)
{
  t_uint32		apicid;
  t_sint32		id;

  /*
   * 1)
   */

  SEM_TAKE(sem);

  /*
   * 2)
   */

  apicid = apic_id();

  if ((id = bootloader_add_cpu(apicid)) >= 0)
    {
      bootloader_cons_msg('#', " AP-%d APIC ID: %d\n", count, apicid);

      bootloader_pmode_ap_init();

      bootloader_paging_ap_init();
    }

  /*
   * 3)
   */

  SEM_RELE(sem);

  /*
   * 4)
   */

  HLT();
}
