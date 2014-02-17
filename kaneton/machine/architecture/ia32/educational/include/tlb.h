/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/include/tlb.h
 *
 * created       julien quintard   [sat jan  8 16:44:31 2011]
 * updated       julien quintard   [sat jan  8 16:44:59 2011]
 */

#ifndef ARCHITECTURE_TLB_H
#define ARCHITECTURE_TLB_H	1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../tlb.c
 */

/*
 * ../tlb.c
 */

t_status		architecture_tlb_invalidate(t_vaddr	address);

t_status		architecture_tlb_flush(void);


/*
 * eop
 */

#endif
