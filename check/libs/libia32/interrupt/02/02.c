/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/libs/libia32/interrupt/02/01.c
 *
 * created       renaud voltz   [mon mar 13 12:58:38 2006]
 * updated       renaud voltz   [mon mar 13 12:58:38 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * check that every idt entry is a valid interrupt gate.
 */

void			check_interrupt_02(void)
{
  t_idtr		idtr;
  t_idte*		idte;
  t_uint32		i;

  LIBIA32_TEST_ENTER;

  SIDT(idtr);

  for (i = 0; i < idtr.size / sizeof(t_idte); i++)
    {
      idte = (t_idte*)idtr.address;

      if (idte->type & DESC_TYPE_PRESENT)
	{
	  if (idte->segsel != (PMODE_GDT_CORE_CS << 3))
	    {
	      printf("error in idt entry %u: invalid segment selector.\n", i);

	      LIBIA32_TEST_LEAVE;
	    }

	  if (IDT_GET_TYPE(idte->type) != type_gate_interrupt)
	    {
	      printf("error in idt entry %u: "
		     "type does not define an interrupt gate\n", i);

	      LIBIA32_TEST_LEAVE;
	    }
	}

      idtr.address += sizeof(t_idte);
    }

  LIBIA32_TEST_LEAVE;
}
