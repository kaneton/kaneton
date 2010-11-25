/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...s/kaneton/core/segment/reserve/reserve.h
 *
 * created       julien quintard   [wed oct 20 16:12:39 2010]
 * updated       julien quintard   [wed nov 24 09:43:37 2010]
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
			  PERMISSION_READ,				\
			  (_segment_)) != ERROR_OK)			\
	TEST_ERROR("[segment_reserve] error\n");			\
									\
      if (segment_get(*(_segment_), &o) != ERROR_OK)			\
	TEST_ERROR("[segment_get] error\n");				\
									\
      if (o->id != *(_segment_))					\
	TEST_ERROR("invalid segment's identifier\n");			\
									\
      if (o->as != (_as_))						\
	TEST_ERROR("invalid segment's address space identifier\n");	\
									\
      if (o->size != (_pages_) * PAGESZ)				\
	TEST_ERROR("invalid segment's size\n");				\
									\
      if (o->permissions != PERMISSION_READ)				\
	TEST_ERROR("invalid segment's permissions\n");			\
    } while (0)

#endif
