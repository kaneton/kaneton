/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/misc/spinlock.h
 *
 * created       matthieu bucchianeri   [wed nov  1 18:05:54 2006]
 * updated       matthieu bucchianeri   [wed nov  1 18:15:24 2006]
 */

#ifndef ARCHITECTURE_SPINLOCK_H
#define ARCHITECTURE_SPINLOCK_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define IA32_SPIN_DECLARE(_spin_)					\
  volatile t_uint8	(_spin_) = 0;

#define IA32_SPIN_LOCK(_spin_)						\
  asm volatile("	movb		$1, %%al		\n"	\
	       "1:						\n"	\
	       "	lock xchgb      %0, %%al		\n"	\
	       "	test		%%al, %%al		\n"	\
	       "	jnz		1b"				\
	       :							\
	       : "m" (_spin_)						\
	       : "%al");

#define IA32_SPIN_UNLOCK(_spin_)					\
  asm volatile("	movb		$0, %0			\n"	\
	       :							\
	       : "m" (_spin_));

#endif
