/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/include/task/task.h
 *
 * created       renaud voltz   [tue apr  4 22:01:00 2006]
 * updated       renaud voltz   [tue apr  4 22:01:00 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

#ifndef IA32_TASK_TASK_H
#define IA32_TASK_TASK_H       1

/*								[cut] k4 */

/*
 * ---------- defines ---------------------------------------------------------
 */

#define CONTEXT_SAVE(_ss_, _esp_)					\
  asm volatile("pusha\n\t"						\
	       "push %%ds\n\t"						\
	       "pushf\n\t"						\
	       "mov %%ss,%0\n\t"					\
	       "movl %%esp,%1\n\t"					\
	       : "=m" (_ss_), "=m" (_esp_)				\
	       :)

#define CONTEXT_RESTORE(_ss_, _esp_)					\
  asm volatile("mov %0,%%ss\n\t"					\
	       "movl %1,%%esp \n\t"					\
	       "popf\n\t"						\
	       "pop %%ds\n\t"						\
	       "popa\n\t"						\
	       :							\
	       : "m" (_ss_), "m" (_esp_))


/*								[cut] /k4 */

#endif
