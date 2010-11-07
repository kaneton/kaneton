/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/multiple/multiple.h
 *
 * created       julien quintard   [wed oct 20 16:12:39 2010]
 * updated       julien quintard   [sat nov  6 22:40:15 2010]
 */

#ifndef TESTS_KANETON_SEGMENT_MULTIPLE_MULTIPLE_H
#define TESTS_KANETON_SEGMENT_MULTIPLE_MULTIPLE_H

/*
 * ---------- macro functions -------------------------------------------------
 */

#define MULTIPLE_ALLOCATE(_as_, _pages_, _segment_)			\
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
