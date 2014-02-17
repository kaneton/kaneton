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
 * updated       julien quintard   [fri jan 14 22:59:44 2011]
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
			  (_pages_) * ___kaneton$pagesz,		\
			  PERMISSION_READ,				\
			  SEGMENT_OPTION_NONE,				\
			  (_segment_)) != STATUS_OK)			\
	TEST_ERROR("[segment_reserve] error");				\
									\
      if (segment_get(*(_segment_), &o) != STATUS_OK)			\
	TEST_ERROR("[segment_get] error");				\
									\
      if (o->id != *(_segment_))					\
	TEST_ERROR("invalid segment's identifier");			\
									\
      if (o->as != (_as_))						\
	TEST_ERROR("invalid segment's address space identifier");	\
									\
      if (o->size != (_pages_) * ___kaneton$pagesz)			\
	TEST_ERROR("invalid segment's size");				\
									\
      if (o->permissions != PERMISSION_READ)				\
	TEST_ERROR("invalid segment's permissions");			\
    } while (0)

#endif
