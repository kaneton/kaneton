/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/check/common/common.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [sat sep  9 18:00:58 2006]
 */

#include <klibc.h>
#include "common.h"

void	check_check_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */

  TEST_ENTER_LEAKS();

  t_uint8*	p = (t_uint8*)0x76543210;

  malloc(10);

  *p = 0; // should #PF

  printf("Didnot #PF\n");

  TEST_LEAVE_LEAKS();
}

/*
 * starts ALL tests manually
 */

void	check_tests(void)
{
  check_check_common();
/*  check_malloc_tests();
  check_set_tests();
  check_malloc_tests();
  check_printf_tests();
  check_stats_tests();
  check_id_tests();
  check_libia32_tests();
  check_event_tests();*/
  /* XXX continue tests */
/*
#if DEBUG & DEBUG_AS
  check_as_tests ();
  check_segment_tests();
  check_region_tests();
  check_task_tests();
  check_map_tests();
#endif
*/
}

/*
 * for leaks tracking
 */

void	check_leaks_init(t_leaks*	l)
{
  l->nb_alloc = alloc_nalloc();
  l->nb_free = alloc_nfree();
}

void	check_leaks_display(t_leaks*	l)
{
  if ((l->nb_alloc - l->nb_free) != (alloc_nalloc() - alloc_nfree()))
/*    printf("error: memory leaks detected: %u/%u -> %u/%u\n",
	   l->nb_alloc, l->nb_free, alloc_nalloc(), alloc_nfree());*/
    printf("-->>LEAK<<--\n");
}

/*
 * exception handling initialization.
 *
 * steps:
 *
 * 1) store caller's ebp in order to be able to restore context as is
      was at the beginning of the function
 * 2) store the old IDT vector pointer.
 * 3) initialize our IDT Register.
 * 4) create our IDT.
 * 5) we do not handler IRQ's.
 * 6) setup our IDT as the active one.
 */

void		check_exc_init(t_exc*		exc,
			       void		(*f)())
{
  typedef struct
  {
    t_uint16	offset_00_15;
    t_uint16	segsel;
    t_uint8	reserved;
    t_uint8	type;
    t_uint16	offset_16_31;
  }		__attribute__ ((packed)) __idte;

  int		i;
  t_uint32	*addr;
  t_uint16	*sz;
  __idte	*idt;

  /*
   * 1)
   */

  asm volatile("movl (%%ebp), %0"
	       : "=r" (exc->ebp));

  /*
   * 2)
   */

  asm volatile("sidt (%0)\n"
	       :
	       : "r" (exc->old_idtr));

  /*
   * 3)
   */

  sz = (t_uint16*)&exc->idtr[0];
  addr = (t_uint32*)&exc->idtr[2];
  *addr = (t_uint32)exc->idt;
  *sz = 32 * 8;

  /*
   * 4)
   */

  idt = (__idte*)exc->idt;

  for (i = 0; i < 32; i++)
    {
      idt[i].offset_00_15 = (t_uint16)(t_uint32)f;
      idt[i].segsel = 0x8;
      idt[i].type = (1 << 7) | (1 << 3) | (1 << 2) | (1 << 1);
      idt[i].offset_16_31 = (t_uint32)f >> 16;
      idt[i].reserved = 0;
    }

  /*
   * 5)
   */

  exc->flag_if = 1; /* XXX */

  asm volatile("cli");

  /*
   * 6)
   */

  asm volatile("lidt (%0)\n"
	       :
	       : "r" (exc->idtr));
}

/*
 * this function is called on exception.
 *
 * steps:
 *
 * 1) print a sweet message.
 * 2) restore old IDT.
 * 3) restore old base pointer. /!\ WE DO NOT RESTORE THE ENTIRE CONTEXT /!\
 * 4) return.
 */

void	check_exc_catch(t_exc*		exc)
{
  /*
   * 1)
   */

  printf("Exception Catched\n");

  /*
   * 2)
   */

  check_exc_reset(exc);

  /*
   * 3)
   */

  asm volatile("movl %0, %%ebp"
	       :
	       : "r" (exc->ebp));
  /*
   * 4)
   */

  asm volatile("leave\n\t"
	       "ret");
}

/*
 * this function restore the interrupt and exception handling
 * environment as it was before.
 *
 * we restore both the IDT and the IF flag.
 */

void	check_exc_reset(t_exc*		exc)
{
  asm volatile("lidt (%0)\n"
	       :
	       : "r" (exc->old_idtr));

  if (exc->flag_if)
    asm volatile("sti");
}
