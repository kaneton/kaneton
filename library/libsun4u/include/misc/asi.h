/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/misc/asi.h
 *
 * created       matthieu bucchianeri   [sat aug 26 19:18:36 2006]
 * updated       matthieu bucchianeri   [sun aug 27 16:52:33 2006]
 */

#ifndef LIBSUN4U_ASI_H
#define LIBSUN4U_ASI_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * load/store 64-bit word to alt. space.
 */

#define LDXA(_asi_,_va_)						\
  ({									\
    t_uint64	r;							\
									\
    asm volatile("ldxa [%2] %1, %0"					\
		 : "=r" (r)						\
		 : "i" (_asi_), "r" (_va_));				\
    r;									\
  })

#define STXA(_asi_,_va_,_r_)						\
  asm volatile("stxa %2, [%0] %1"					\
	       :							\
	       : "r" (_va_), "i" (_asi_), "r" (_r_))

/*
 * load/store byte to alt. space.
 */

#define LDUBA(_asi_,_va_)						\
  ({									\
    t_uint64	r;							\
									\
    asm volatile("lduba [%2] %1, %0"					\
		 : "=r" (r)						\
		 : "i" (_asi_), "r" (_va_));				\
    r;									\
  })

#define LDSBA(_asi_,_va_)						\
  ({									\
    t_uint64	r;							\
									\
    asm volatile("ldsba [%2] %1, %0"					\
		 : "=r" (r)						\
		 : "i" (_asi_), "r" (_va_));				\
    r;									\
  })

#define STBA(_asi_,_va_,_r_)						\
  asm volatile("stba %2, [%0] %1"					\
	       :							\
	       : "r" (_va_), "i" (_asi_), "r" (_r_))

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * address space without MMU translation.
 */

#define ASI_BYPASS_MMU		0x14

/*
 * Load-Store Unit control register asi and address.
 */

#define ASI_LSU_CONTROL		0x45
#define REG_LSU_CONTROL		0x0

/*
 * I-MMU registers
 */

#define ASI_IMMU		0x50
#define REG_IMMU_TAG_TARGET	0x0
#define REG_IMMU_FAULT_STATUS	0x18
#define REG_IMMU_TSB		0x28
#define REG_IMMU_TAG_ACCESS	0x30

#define ASI_IMMU_8K_TSB		0x51
#define REG_IMMU_8K_TSB		0x0

#define ASI_IMMU_64K_TSB	0x52
#define REG_IMMU_64K_TSB	0x0

#define ASI_IMMU_DATA_IN	0x54
#define REG_IMMU_DATA_IN	0x0

#define ASI_IMMU_DATA_ACCESS	0x55
#define ASI_IMMU_TAG_READ	0x56
#define ASI_IMMU_DEMAP		0x57
#define REG_IMMU_DEMAP		0x0

/*
 * D-MMU registers
 */

#define ASI_DMMU		0x58
#define REG_DMMU_TAG_TARGET	0x0
#define REG_DMMU_FAULT_STATUS	0x18
#define REG_DMMU_FAULT_ADDRESS	0x20
#define REG_DMMU_TSB		0x28
#define REG_DMMU_TAG_ACCESS	0x30
#define REG_DMMU_VA_WATCH	0x38
#define REG_DMMU_PA_WATCH	0x40

#define ASI_DMMU_8K_TSB		0x59
#define REG_DMMU_8K_TSB		0x0

#define ASI_DMMU_64K_TSB	0x5A
#define REG_DMMU_64K_TSB	0x0

#define ASI_DMMU_DATA_IN	0x5C
#define REG_DMMU_DATA_IN	0x0

#define ASI_DMMU_DIRECT_TSB	0x5D
#define REG_DMMU_DIRECT_TSB	0x0

#define ASI_DMMU_DATA_ACCESS	0x5D
#define ASI_DMMU_TAG_READ	0x5E
#define ASI_DMMU_DEMAP		0x5F
#define REG_DMMU_DEMAP		0x0

/*
 * I-MMU & D-MMU registers
 */

#define REG_PRIMARY_CONTEXT	0x8
#define REG_SECONDARY_CONTEXT	0x10

#endif
