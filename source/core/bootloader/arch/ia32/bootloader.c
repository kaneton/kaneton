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
 * last update   Mon May 30 14:01:14 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * a funny function to display a fatal message.
 */

void			bootloader_error(void)
{
  cons_msg('!', "fatal error during bootloading\n");

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
 */

t_vaddr			bootloader_relocate(multiboot_info_t*	mbi)
{
  module_t*		mod = (module_t*)mbi->mods_addr;
  Elf32_Ehdr*		khdr = (Elf32_Ehdr*)mod[0].mod_start;
  t_uint32		i;

  memcpy(
  for (i = 0; i < mbi->mods_count; i++)
    printf("%s 0x%x-0x%x\n", mod[i].string,
	   mod[i].mod_start, mod[i].mod_end);
  */
}

/*
 * the bootloader entry point.
 */

int			main(t_uint32			magic,
			     multiboot_info_t*		mbi)
{
  cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("                      --- the kaneton operating system ---\n\n\n");

  bootloader_relocate(mbi);
  gdt_init();
  paging_init();

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

  /*
   * penser a initialiser une stack correcte et assez grande
   */

  /*
   * penser a creer une stack, modifier esp et ebp
   * puis a bien la mapper
   */

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

      asm volatile ("mov %0, %%esp" : : "g" (sp_init));

      kernel(VMAP_MNG_ADDR, minfo);
    }
  */

  cons_msg('+', "kaneton bootloaded\n");

  /* XXX jump on the kernel */

  while (1)
    ;
}
