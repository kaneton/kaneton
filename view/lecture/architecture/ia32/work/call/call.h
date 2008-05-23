/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * call.h
 * 
 * path          /home/mycure/kaneton/papers/lectures/prerequisites/work
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Oct 23 15:28:47 2005   mycure
 * last update   Sun Oct 23 15:29:36 2005   mycure
 */

#ifndef CALL_H
#define CALL_H

/*
 * includes
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * this macro makes a function call using a new stack
 *
 * the prototype of the called function is: void (arg1, arg2)
 */

#define call(_stacksz_, _function_, _arg1_, _arg2_)			\
  {									\
    char*	_allocated_ = malloc((_stacksz_));			\
    char*	_stack_ = _allocated_ + (_stacksz_) - 4;		\
									\
    asm("movl %%ebp, %%eax\n\t"						\
	"movl %%esp, %%ebx\n\t"						\
									\
	"movl %0, %%esp\n\t"						\
									\
	"pushl %%ebx\n\t"						\
	"pushl %%eax\n\t"						\
									\
	"movl %%esp, %%ebp\n\t"						\
									\
	"pushl %2\n\t"							\
	"pushl %1\n\t"							\
									\
        "movl %3, %%eax\n\t"						\
	"call *%%eax\n\t"						\
									\
	"movl %%ebp, %%esp\n\t"						\
									\
	"popl %%ebp\n\t"						\
	"popl %%esp\n\t"						\
									\
	:								\
	: "r" ((_stack_)), "r" ((_arg1_)), "r" ((_arg2_)),		\
          "r" ((_function_))						\
	: "eax", "ebx"							\
       );								\
									\
    free((_allocated_));						\
  }

#endif
