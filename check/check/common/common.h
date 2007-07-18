/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/check/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [sun may 27 21:09:10 2007]
 */

#ifndef CHECK_COMMON_H_
# define CHECK_COMMON_H_

# include <core/core.h>
# include <libc.h>

# ifndef PAGESZ
#  define PAGESZ	4096
# endif /* !PAGESZ */

/*
 * Exception catching.
 */

typedef struct
{
  t_uint8	idt[32 * 8];
  t_uint8	old_idtr[6];
  t_uint8	idtr[6];
  t_uint8	flag_if;
  t_uint32	ebp;
}		t_exc;

/*
 * Classic test enter/leave.
 */

#if 0
# define TEST_ENTER()							\
  static t_exc	__exceptions;						\
  void		__exc()							\
  {									\
    check_exc_catch(&__exceptions);					\
  }									\
  printf("Test %s\n", __FUNCTION__);					\
  check_exc_init(&__exceptions, __exc)
#else
# define TEST_ENTER()							\
  printf("Test %s\n", __FUNCTION__);
#endif

#if 0
# define TEST_LEAVE()							\
  check_exc_reset(&__exceptions);					\
  printf("%s done.\n", __FUNCTION__);					\
  return
#else
# define TEST_LEAVE()							\
  printf("%s done.\n", __FUNCTION__);					\
  return
#endif

/*
 * W/ leaks detection enter/leave.
 */

# define TEST_ENTER_LEAKS()						\
  static t_exc	__exceptions;						\
  t_leaks	__memory_leaks;						\
  void		__exc()							\
  {									\
    check_exc_catch(&__exceptions);					\
  }									\
  printf("Test %s\n", __FUNCTION__);					\
  check_exc_init(&__exceptions, __exc);					\
  check_leaks_init(&__memory_leaks)

# define TEST_LEAVE_LEAKS()						\
  check_exc_reset(&__exceptions);					\
  check_leaks_display(&__memory_leaks);					\
  printf("%s done.\n", __FUNCTION__);					\
  return

#undef ASSERT
#define ASSERT(Test,Error)						\
  if (!(Test))								\
    {									\
      printf(Error);							\
      TEST_LEAVE();							\
    }

#define TEST_NEW_AS(Task,As)						\
  ASSERT(task_reserve(TASK_CLASS_PROGRAM, TASK_BEHAV_INTERACTIVE,	\
		      TASK_PRIOR_INTERACTIVE, &(Task)) == ERROR_NONE,	\
	 "Error creating task\n");					\
  ASSERT(as_reserve((Task), &(As)) == ERROR_NONE, "Error creating as\n")

#define TEST_LEAVE_AS(Task,As)						\
  ASSERT(as_release(As) == ERROR_NONE, "Failed to release as\n");	\
  ASSERT(task_release(Task) == ERROR_NONE, "Failed to release task\n")

typedef struct
{

  t_uint32	nb_alloc;
  t_uint32	nb_free;

}		t_leaks;

extern i_as	kasid;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 */

/*
 * common.c
 */

void	check_check_common(void);

void	check_tests(void);

void	check_leaks_init(t_leaks*	l);

void	check_leaks_display(t_leaks*	l);

void		check_exc_init(t_exc*		exc,
			       void		(*f)());

void	check_exc_catch(t_exc*		exc);

void	check_exc_reset(t_exc*		exc);


/*
 * eop
 */

#endif /* !CHECK_COMMON_H_ */
