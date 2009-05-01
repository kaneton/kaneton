/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/bootloader.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:07 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define LINKER_SYMBOL(_symbol_)						\
  ((t_vaddr)&(_symbol_))

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * these three variable are globals to avoid them to be lost when the
 * bootloader changes its stack.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_init*);

t_uint32		ebp;
t_uint32		esp;

t_uint32		chksums[16];
multiboot_info_t*	multiboot;
module_t*		mod;
t_uint32		i;

extern t_vaddr		_bootloader_begin;
extern t_vaddr		_bootloader_end;
extern t_vaddr		_bootloader_data_begin;
extern t_vaddr		_bootloader_data_end;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * a funny function which does nothing.
 *
 * this function is called when a fatal error occurs.
 */

void			bootloader_error(void)
{
  while (1)
    ;
}

/*
 * compute a buffer's checksum.
 */

t_uint32		sum2(t_vaddr				begin,
			     t_vaddr				end)
{
  register t_uint32	thecrc;
  register t_uint32	total;
  register t_uint8	*p;
  char*			buf = (char*)begin;
  t_vsize		size = end - begin;

  /*
   * Draft 8 POSIX 1003.2:
   *
   *      s = sum of all bytes
   *      r = s % 2^16 + (s % 2^32) / 2^16
   * thecrc = (r % 2^16) + r / 2^16
   */

  thecrc = total = 0;

  for (total += (t_uint32)size, p = (t_uint8*)buf; size--; ++p)
    thecrc += *p;

  thecrc = (thecrc & 0xffff) + (thecrc >> 16);
  thecrc = (thecrc & 0xffff) + (thecrc >> 16);

  printf("%u\n", thecrc);

  return (thecrc);
}

/*
 * the bootloader entry point.
 *
 * steps:
 *
 * 1) initialize the console and checks the magic number.
 * 2) relocate binaries, data, stack...
 * 3) install the protected mode.
 * 4) install the paging mode.
 * 5) compute the segments and regions to pass to the kernel.
 * 6) dump the init structure if required.
 * 7) update registers for the new kernel stack.
 * 8) then, the kernel is launched.
 * 9) this part is only reached if the kernel exit.
 */

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi)
{
  /*
   * 1)
   */

  /* compute checksums */
  chksums[1] = sum2((t_vaddr)mbi,
		    (t_vaddr)(mbi + 1));
  multiboot = mbi;

  mod = (module_t*)mbi->mods_addr;

  for (i = 0; i < mbi->mods_count; i++)
    {
      chksums[2 + i] = sum2(mod[i].mod_start,
			    mod[i].mod_end);
    }

  /* ok, let's move */
 restart:
  bootloader_cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("\n");
  printf("                --- the kaneton microkernel project ---\n");
  printf("\n");

  /*
   * 2)
   */

  kernel = (void (*)(t_init*))bootloader_init_relocate(mbi);

  /*
   * 3)
   */

  bootloader_pmode_init();

  /*
   * 4)
   */

  bootloader_paging_init();

  /*
   * 5)
   */

  bootloader_init_segments();
  bootloader_init_regions();

  /*
   * 6)
   */

#if (IA32_DEBUG & IA32_DEBUG_INIT)
  bootloader_init_dump();
#endif

  /*
   * 7)
   */

  asm("movl %%ebp, %0\n\t"
      "movl %%esp, %1\n\t"
      : "=m" (ebp), "=m" (esp)
      :
      : "memory"
     );

  /* last checksum */
  chksums[0] = 0;
  chksums[0] = sum2(LINKER_SYMBOL(_bootloader_begin),
		    LINKER_SYMBOL(_bootloader_end));

  asm("movl %0, %%ebp\n\t"
      "movl %0, %%esp\n\t"
      "pushl %1\n"
      :
      : "g" (init->kstack + init->kstacksz - 4),
        "g" (init)
     );

  /*
   * 8)
   */

  kernel(init);

  printf("exited...\n");

  /* reset a few things */
  asm volatile("cli");			/* IF */

					/* PIC */

					/* PIT */

					/* mem */

					/* IDTR */

  /* check for protected area contents */
  printf("testing %u against ", chksums[0]);
  if (chksums[0] != sum2(LINKER_SYMBOL(_bootloader_begin),
			 LINKER_SYMBOL(_bootloader_end)))
    goto reboot;
  printf("bootloader checksum OK\n");

  printf("testing %u against ", chksums[1]);
  if (chksums[1] != sum2((t_vaddr)multiboot,
			 (t_vaddr)(multiboot + 1)))
    goto reboot;
  printf("multiboot checksum OK\n");

  mod = (module_t*)multiboot->mods_addr;

  for (i = 0; i < multiboot->mods_count; i++)
    {
      printf("testing %u against ", chksums[2 + i]);

      if (chksums[2 + i] != sum2(mod[i].mod_start,
				 mod[i].mod_end))
	goto reboot;
      printf("module[%d] checksum OK\n", i);
    }

  /*
  memset((void*)LINKER_SYMBOL(_bootloader_data_begin),
	 0,
	 LINKER_SYMBOL(_bootloader_data_end) -
	 LINKER_SYMBOL(_bootloader_data_begin));
  */
  /* reset stack */
  asm("movl %0, %%ebp\n\t"
      "movl %1, %%esp\n\t"
      :
      : "g" (ebp), "g" (esp)
     );

  /*
   * 9)
   */

  /* rerun the kernel */
  goto restart;

 reboot:
  bootloader_cons_msg('!', "error: bad checksum, forced to reboot\n");

  /* send reboot command */

  return (0);
}
