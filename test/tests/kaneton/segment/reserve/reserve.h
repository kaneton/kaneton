/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/segment/reserve/reserve.h
 *
 * created       julien quintard   [wed oct 20 16:12:39 2010]
 * updated       julien quintard   [mon nov  8 09:40:55 2010]
 */

#ifndef TESTS_KANETON_SEGMENT_RESERVE_RESERVE_H
#define TESTS_KANETON_SEGMENT_RESERVE_RESERVE_H

/*
 * ---------- macro functions -------------------------------------------------
 */

#define TEST_ALLOCATE(_as_, _pages_, _segment_)				\
  do									\
    {									\
      o_segment*	o;						\
									\
      if (segment_reserve((_as_),					\
			  (_pages_) * PAGESZ,				\
			  PERM_READ,					\
			  (_segment_)) != ERROR_NONE)			\
	TEST_ERROR("[segment_reserve] error\n");			\
									\
      if (segment_get(*(_segment_), &o) != ERROR_NONE)			\
	TEST_ERROR("[segment_get] error\n");				\
									\
      if (o->segid != *(_segment_))					\
	TEST_ERROR("invalid segment's identifier\n");			\
									\
      if (o->asid != (_as_))						\
	TEST_ERROR("invalid segment's address space identifier\n");	\
									\
      if (o->address != (t_uint32)(*(_segment_)))			\
	TEST_ERROR("invalid segment's address\n");			\
									\
      if (o->size != (_pages_) * PAGESZ)				\
	TEST_ERROR("invalid segment's size\n");				\
									\
      if (o->perms != PERM_READ)					\
	TEST_ERROR("invalid segment's permissions\n");			\
    } while (0)

#endif
