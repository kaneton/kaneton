/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/architecture/ia32/NEW/include/linker.h
 *
 * created       julien quintard   [sat oct 16 13:54:01 2010]
 * updated       julien quintard   [sat oct 16 13:55:05 2010]
 */

#ifndef ARCHITECTURE_LINKER_H
#define ARCHITECTURE_LINKER_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define LINKER_SYMBOL(_symbol_)						\
  ((t_vaddr)&(_symbol_))

#endif
