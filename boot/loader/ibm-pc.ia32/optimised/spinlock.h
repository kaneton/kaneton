/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...tloader/ibm-pc.ia32/optimised/spinlock.h
 *
 * created       matthieu bucchianeri   [mon dec 24 18:17:39 2007]
 * updated       matthieu bucchianeri   [mon dec 24 18:17:59 2007]
 */

#ifndef BOOTLOADER_SPINLOCK_H
#define BOOTLOADER_SPINLOCK_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define SPIN_DECLARE(_spin_)						\
  volatile t_uint8	(_spin_) = 0;

#define SPIN_LOCK(_spin_)						\
  asm volatile("	movb		$1, %%al		\n"	\
	       "1:						\n"	\
	       "	lock xchgb      %0, %%al		\n"	\
	       "	test		%%al, %%al		\n"	\
	       "	jnz		1b"				\
	       :							\
	       : "m" (_spin_)						\
	       : "%al");

#define SPIN_UNLOCK(_spin_)						\
  asm volatile("	movb		$0, %0			\n"	\
	       :							\
	       : "m" (_spin_));

#endif
