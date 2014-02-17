/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chitecture/ia32/region/release/release.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue apr 12 22:29:56 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "release.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		thrown = 0;
static volatile i_segment	seg;
static volatile t_vaddr		addr;

/*
 * ---------- prototypes ------------------------------------------------------
 */

void			test_architecture_region_release(void);

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_region_release_handler(t_id	id,
								 t_vaddr pv)
{
  i_region		reg;

  thrown = 1;

  if (region_reserve(_kernel.as,
		     seg,
		     0,
		     REGION_OPTION_FORCE,
		     addr,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");
}

void			test_architecture_region_release(void)
{
  i_region		reg;
  o_region*		o;
  t_uint32*		p;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      (i_segment*)&seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     0,
		     REGION_OPTION_NONE,
		     0,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as, reg, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  addr = o->address;

  p = (t_uint32*)o->address;

  *p = 0x42;

  event_release(ARCHITECTURE_IDT_EXCEPTION_PF);

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_PF,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_region_release_handler),
		    EVENT_DATA(NULL)) != STATUS_OK)
    TEST_ERROR("[event_reserve] error");

  if (region_release(_kernel.as, o->id) != STATUS_OK)
    TEST_ERROR("[region_release] error");

  *p = 0x84;

  if (event_release(ARCHITECTURE_IDT_EXCEPTION_PF) != STATUS_OK)
    TEST_ERROR("[event_release] error");

  if (thrown != 1)
    TEST_ERROR("the exception does not seem to have been thrown");

  TEST_SIGNATURE(r2309ifsif203j2jigg3);

  TEST_LEAVE();
}
