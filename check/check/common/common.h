/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/check/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [sat oct 14 17:03:05 2006]
 */

#ifndef CHECK_COMMON_H_
# define CHECK_COMMON_H_

# include <klibc.h>

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

# define TEST_ENTER()							\
  static t_exc	__exceptions;						\
  void		__exc()							\
  {									\
    check_exc_catch(&__exceptions);					\
  }									\
  printf("Test %s\n", __FUNCTION__);					\
  check_exc_init(&__exceptions, __exc)

# define TEST_LEAVE()							\
  check_exc_reset(&__exceptions);					\
  printf("%s done.\n", __FUNCTION__);					\
  return

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


#define ASSERT(Test,Error)						\
  if (!(Test))								\
    {									\
      printf(Error);							\
      TEST_LEAVE();							\
    }

typedef struct
{

  t_uint32	nb_alloc;
  t_uint32	nb_free;

}		t_leaks;

/*
 * backward compatibility types.
 */

typedef i_as		t_asid;
typedef i_segment	t_segid;
typedef i_region	t_regid;

void	check_set_tests(void);
void	check_stats_tests(void);
void	check_malloc_tests(void);
void	check_printf_tests(void);
void	check_id_tests(void);
void	check_libia32_tests(void);
void	check_event_tests(void);

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
