/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/pmode/pmode.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:48:31 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * protected mode structures and macros.
 *
 */

#ifndef IA32_IA32_PMODE_H
#define IA32_IA32_PMODE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * privilege levels
 */

#define PRIV_RING0		0
#define PRIV_RING1		1
#define PRIV_RING2		2
#define PRIV_RING3		3

/*
 * descriptor management defines
 */

#define DESC_TYPE_PRESENT       (1 << 7)

#define DESC_MK_DPL(Priv)                                               \
  ((Priv) << 5)

#define DESC_GET_DPL(Type)                                              \
  (((Type) >> 5) & 0x3)

/*
 * segment selector defines
 */

/*
 * null segment selector index
 */

#define SEGSEL_NULL             0

/*
 * segment selector table indicator
 */

#define SEGSEL_TI_GDT           (0 << 2)
#define SEGSEL_TI_LDT           (1 << 2)

/*
 * build a segment selector
 */

#define SEGSEL(_index_, _rpl_)						\
  ((t_uint16)(((_index_) << 3) + (_rpl_)))

/*
 * segment selector requested privilege level
 */

#define SEGSEL_MK_RPL(Entry)                                            \
  ((Entry) << 0)

/*
 * segment selector index
 */

#define SEGSEL_MK_INDEX(Entry)                                          \
  ((Entry) << 3)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * privilege levels
 */

typedef enum
  {
    ia32_prvl_supervisor	= PRIV_RING0,
    ia32_prvl_privileged	= PRIV_RING1,
    ia32_prvl_user		= PRIV_RING3,
  }				t_ia32_prvl;

#endif
