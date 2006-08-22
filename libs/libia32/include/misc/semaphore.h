/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/misc/semaphore.h
 *
 * created       matthieu bucchianeri   [mon aug 21 18:12:06 2006]
 * updated       matthieu bucchianeri   [mon aug 21 18:13:38 2006]
 */

#ifndef IA32_SEMAPHORE_H
#define IA32_SEMAPHORE_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define SEM_DECLARE(_sem_)						\
  volatile t_uint16		(_sem_) = 1

#define SEM_TAKE(_sem_)							\
  asm volatile("xor %%ax, %%ax\n\t"					\
	       ".L1:\n\t"						\
	       "xchg %%ax, %0\n\t"					\
	       "or %%ax, %%ax\n\t"					\
	       "jz .L1\n\t"						\
	       "dec %%ax\n\t"						\
	       "xchg %%ax, %0"						\
	       :							\
	       : "m" (_sem_)						\
	       : "%ax");

#define SEM_RELEASE(_sem_)						\
  asm volatile("xchg %%ax, %0\n\t"					\
	       "inc %%ax\n\t"						\
	       "xchg %%ax, %0"						\
	       :							\
	       : "m" (_sem_)						\
	       : "%ax");

#endif
