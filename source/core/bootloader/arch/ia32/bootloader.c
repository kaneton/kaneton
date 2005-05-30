/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.c
 * 
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon May 30 15:42:49 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * a funny function which do nothing.
 */

void			bootloader_error(void)
{
  while (1)
    ;
}

/*
 * this function relocates the modules over the two megabytes,
 * creates a kernel stack etc.. and returns the kernel entry point address.
 *
 * this is not necessary but we want to it properly.
 *
 * note that the first module will be considered as the kernel module.
 *
 * 1) copies the kernel code into its new location KANETON_KERNEL_CODE
 * 2) initializes the kernel stack located at KANETON_KERNEL_STACK
 * 3) for each module, copies it into its new location, starting at
 *    KANETON_MODULES
 */

t_vaddr			bootloader_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod[0].mod_start;
  t_paddr		addr = KANETON_MODULES;
  t_uint32		modsz;
  t_uint32		i;

  cons_msg('+', "relocating modules...\n");

  /*
   * 1)
   */

  modsz = mod[0].mod_end - mod[0].mod_start;

  if (modsz > KANETON_KERNEL_CODESZ)
    {
      cons_msg('!', "error: the kernel size is over two megabytes\n");
      bootloader_error();
    }

  memcpy((void*)KANETON_KERNEL_CODE, (const void*)mod[0].mod_start, modsz);

  cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	   mod[0].string, mod[0].mod_start, KANETON_KERNEL_CODE, modsz);

  /*
   * 2)
   *
   * nothing to do here.
   */

  /*
   * 3)
   */

  for (i = 1; i < mbi->mods_count; i++)
    {
      modsz = mod[i].mod_end - mod[i].mod_start;

      memcpy((void*)addr, (const void*)mod[i].mod_start, modsz);

      addr += (modsz / PAGESZ + ((modsz % PAGESZ) ? 1 : 0)) * PAGESZ;

      cons_msg('+', " %s relocated from 0x%x to 0x%x (0x%x)\n",
	       mod[i].string, mod[i].mod_start, addr, modsz);
    }
}

/*
 * the bootloader entry point.
 *
 * the booloder:
 *
 * 1) relocates binaries, data, stack
 * 2) installs the protected mode
 * 3) installs the paging mode
 */

int			main(t_uint32			magic,
			     multiboot_info_t*		mbi)
{
  cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("                      --- the kaneton operating system ---\n\n\n");

  bootloader_relocate(mbi);

  pmode_init();

  paging_init();

#if 1
  printf("memory: 0x%x to %u\n", mbi->mem_lower, mbi->mem_upper);
  printf("mmap: 0x%x size 0x%x\n", mbi->mmap_addr, mbi->mmap_length);

  printf("main function: 0x%x\n", main);

  {
    int		ebp, esp;

    asm volatile ("movl %%ebp, %0\n"
		  "movl %%esp, %1\n"
		  : "=m" (ebp), "=m" (esp)
		  :);

    printf("ebp: 0x%x esp: 0x%x\n", ebp, esp);
  }

  printf("elf: 0x%x size 0x%x num %u\n", mbi->u.elf_sec.addr,
	 mbi->u.elf_sec.size, mbi->u.elf_sec.num);
#endif

  /* XXX passer une sorte de stack au kernel qui contiendrait
   * une liste des zones memoire avec attributs: BIOS, KERNEL etc..
   *
   * le fait de la passer au kernel rend l'implementation du bootloader
   * independant de la structure de donnees utilisee par le kernel
   */

  /*
    {
      pmap_init(minfo);
      vmap_init(minfo);
      gdt_reinit();

      header = (Elf32_Ehdr *)mod->mod_start;
      kernel = (t_func)header->e_entry;

      // penser a creer une stack, modifier esp et ebp
      // puis a bien la mapper

      asm volatile ("mov %0, %%esp" : : "g" (sp_init));

      kernel(VMAP_MNG_ADDR, minfo);
    }
  */

  cons_msg('+', "kaneton bootloaded\n");

  /* XXX jump on the kernel */

  while (1)
    ;
}
