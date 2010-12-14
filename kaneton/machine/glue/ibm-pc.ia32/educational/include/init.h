/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/ibm-pc.ia32/educational/include/init.h
 *
 * created       julien quintard   [wed jun  6 16:16:48 2007]
 * updated       julien quintard   [mon dec 13 11:13:15 2010]
 */

#ifndef GLUE_INIT_H
#define GLUE_INIT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_data_s_init()					\
  struct								\
  {									\
    t_ia32_gdt			gdt;					\
    t_ia32_directory		pd;					\
  }				machine;

#endif
