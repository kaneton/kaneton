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
 * updated       julien quintard   [thu apr  7 14:55:37 2011]
 */

#ifndef GLUE_INIT_H
#define GLUE_INIT_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function includes data in the 's_init' structure: the addresses
 * of both the GDT - Global Descriptor Table and PD - Page Directory.
 */

#define		machine_data_s_init()					\
  struct								\
  {									\
    t_paddr			gdt;					\
    t_paddr			pd;					\
  }				machine;

#endif
