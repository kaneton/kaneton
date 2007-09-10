/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/common/common.h
 *
 * created       matthieu bucchianeri   [wed mar 15 23:12:48 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:57:51 2007]
 */

#ifndef CHECK_SEGMENT_COMMON_H_
# define CHECK_SEGMENT_COMMON_H_

# include <libc/libc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

# define RESERVE_AND_CHECK(As,Nbpages,Seg)						\
  do											\
  {											\
    o_segment*	o;									\
											\
    ASSERT(segment_reserve((As), (Nbpages) * PAGESZ, PERM_READ, (Seg)) == ERROR_NONE,	\
	 "error reserving segment\n");							\
    ASSERT(segment_get(*(Seg), &o) == ERROR_NONE, "error getting segment\n");		\
    ASSERT(o->segid == *(Seg), "Bad segid field\n");					\
    ASSERT(o->asid == (As), "Bad asid field\n");					\
    ASSERT(o->address == (t_uint32)(*(Seg)), "Bad address field\n");			\
    ASSERT(o->size == (Nbpages) * PAGESZ, "Bad size field\n");				\
    ASSERT(o->perms == PERM_READ, "Bad perms field\n");					\
  }											\
  while (0)


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../simple_reserve_01/simple_reserve_01.c
 *      ../simple_reserve_02/simple_reserve_02.c
 *      ../simple_reserve_03/simple_reserve_03.c
 *      ../simple_reserve_release_01/simple_reserve_release_01.c
 *      ../multiple_reserve_01/multiple_reserve_01.c
 *      ../multiple_reserve_02/multiple_reserve_02.c
 *      ../multiple_reserve_03/multiple_reserve_03.c
 *      ../multiple_reserve_release_01/multiple_reserve_release_01.c
 *	../flush_01/flush_01.c
 *	../perms_01/perms_01.c
 *	../read_write_01/read_write_01.c
 *	../read_write_02/read_write_02.c
 *	../clone_01/clone_01.c
 *	../perms_02/perms_02.c
 *	../perms_03/perms_03.c
 *	../inject_01/inject_01.c
 *	../copy_01/copy_01.c
 *	../perms_04/perms_04.c
 *	../perms_05/perms_05.c
 *	../give_01/give_01.c
 *	../type_01/type_01.c
 *	../catch_01/catch_01.c
 *	../resize_01/resize_01.c
 *	../resize_02/resize_02.c
 *	../split_01/split_01.c
 *	../coalesce_01/coalesce_01.c
 */

/*
 * common.c
 */

void	check_segment_common(void);

void	check_segment_tests(void);


/*
 * ../simple_reserve_01/simple_reserve_01.c
 */

void		check_segment_simple_reserve_01(void);


/*
 * ../simple_reserve_02/simple_reserve_02.c
 */

void		check_segment_simple_reserve_02(void);


/*
 * ../simple_reserve_03/simple_reserve_03.c
 */

void		check_segment_simple_reserve_03(void);


/*
 * ../simple_reserve_release_01/simple_reserve_release_01.c
 */

void		check_segment_simple_reserve_release_01(void);


/*
 * ../multiple_reserve_01/multiple_reserve_01.c
 */

void		check_segment_multiple_reserve_01(void);


/*
 * ../multiple_reserve_02/multiple_reserve_02.c
 */

void		check_segment_multiple_reserve_02(void);


/*
 * ../multiple_reserve_03/multiple_reserve_03.c
 */

void		check_segment_multiple_reserve_03(void);


/*
 * ../multiple_reserve_release_01/multiple_reserve_release_01.c
 */

void		check_segment_multiple_reserve_release_01(void);


/*
 * ../flush_01/flush_01.c
 */

void		check_segment_flush_01(void);


/*
 * ../perms_01/perms_01.c
 */

void		check_segment_perms_01(void);


/*
 * ../read_write_01/read_write_01.c
 */

void		check_segment_read_write_01(void);


/*
 * ../read_write_02/read_write_02.c
 */

void		check_segment_read_write_02(void);


/*
 * ../clone_01/clone_01.c
 */

void		check_segment_clone_01(void);


/*
 * ../perms_02/perms_02.c
 */

void		check_segment_perms_02(void);


/*
 * ../perms_03/perms_03.c
 */

void		check_segment_perms_03(void);


/*
 * ../inject_01/inject_01.c
 */

void		check_segment_inject_01(void);


/*
 * ../copy_01/copy_01.c
 */

void		check_segment_copy_01(void);


/*
 * ../perms_04/perms_04.c
 */

void		check_segment_perms_04(void);


/*
 * ../perms_05/perms_05.c
 */

void		check_segment_perms_05(void);


/*
 * ../give_01/give_01.c
 */

void		check_segment_give_01(void);


/*
 * ../type_01/type_01.c
 */

void		check_segment_type_01(void);


/*
 * ../catch_01/catch_01.c
 */

void		check_segment_catch_01(void);


/*
 * ../resize_01/resize_01.c
 */

void		check_segment_resize_01(void);


/*
 * ../resize_02/resize_02.c
 */

void		check_segment_resize_02(void);


/*
 * ../split_01/split_01.c
 */

void		check_segment_split_01(void);


/*
 * ../coalesce_01/coalesce_01.c
 */

void		check_segment_coalesce_01(void);


/*
 * eop
 */

#endif /* !CHECK_SEGMENT_COMMON_H_ */
