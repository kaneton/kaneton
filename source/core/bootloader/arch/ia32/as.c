/*
 * kaneton
 *
 * as.c
 *
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Thu Jun 16 15:56:20 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * this function just initializes the address space manager allocating
 * a page of physical memory because an address space structure can
 * be contained in a page of physical memory.
 */

void			as_init(void)
{
  t_psize		asmngsz;

  /*
   * XXX for the moment we preallocate 2 pages.
   */

  init->asmng = init_alloc(2 * PAGESZ, &asmngsz);
  init->asmngsz = asmngsz;
}
