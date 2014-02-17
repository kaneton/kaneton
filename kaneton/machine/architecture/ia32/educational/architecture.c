/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...itecture/ia32/educational/architecture.c
 *
 * created       julien quintard   [sun jan 16 13:30:38 2011]
 * updated       julien quintard   [sat feb  5 12:52:46 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the IA32 manager.
 */

ARCHITECTURE_LINKER_LOCATION(".handler_data")
am			_architecture;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the architecture manager.
 */

t_status		architecture_dump(void)
{
  module_call(console, message,
	      '#',
	      "architecture: error(0x%x)\n",
	      _architecture.error);

  module_call(console, message,
	      '#',
	      "  kernel: ds(0x%x) pdbr(0x%x)\n",
	      _architecture.kernel.ds,
	      _architecture.kernel.pdbr);

  module_call(console, message,
	      '#',
	      "    kis: base(0x%x) size(0x%x) pointer(0x%x)\n",
	      _architecture.kernel.kis.base,
	      _architecture.kernel.kis.size,
	      _architecture.kernel.kis.pointer);

  module_call(console, message,
	      '#',
	      "  thread: pdbr(0x%x) pointer(0x%x)\n",
	      _architecture.thread.pdbr,
	      _architecture.thread.pointer);

  MACHINE_LEAVE();
}
