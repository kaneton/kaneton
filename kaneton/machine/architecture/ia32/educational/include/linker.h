/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ecture/ia32/educational/include/linker.h
 *
 * created       julien quintard   [sat oct 16 13:54:01 2010]
 * updated       julien quintard   [thu jan  6 09:17:33 2011]
 */

#ifndef ARCHITECTURE_LINKER_H
#define ARCHITECTURE_LINKER_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

// XXX rajouter ici aussi les sections .handler etc.

#define LINKER_SYMBOL(_symbol_)						\
  ((t_vaddr)&(_symbol_))

#endif
