/*
    This file is part of MutekH.

    MutekH is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MutekH is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MutekH; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

    Copyright Alexandre Becoulet <alexandre.becoulet@lip6.fr> (c) 2006
*/

#ifndef __GPCT_PLATFORM_HEXO_H__
#define __GPCT_PLATFORM_HEXO_H__

#include <hexo/error.h>
#include <hexo/types.h>
#include <hexo/atomic.h>

#define GPCT_CONFIG_NOPLATFORM

#define GPCT_ULONG	__compiler_ulong_t
#define GPCT_ULONGLONG	__compiler_ulonglong_t

typedef bool_t		gpct_bool_t;
typedef	error_t		gpct_error_t;
typedef atomic8_t	gpct_atomic_t;
typedef atomic32_t	gpct_atomic_int_t;
typedef size_t		gpct_index_t;
typedef ssize_t		gpct_sindex_t;

/* static value init */
#define GPCT_ATOMIC_INITIALIZER(v)	ATOMIC_INITIALIZER(v)

static inline void gpct_atomic_init(gpct_atomic_t *a)
{
}

static inline void gpct_atomic_destroy(gpct_atomic_t *a)
{
}

static inline void gpct_atomic_set(gpct_atomic_t *a, gpct_atomic_int_t v)
{
  *a = v;
}

static inline gpct_atomic_int_t gpct_atomic_get(gpct_atomic_t *a)
{
  return *a;
}

static inline gpct_bool_t gpct_atomic_inc(gpct_atomic_t *a)
{
  atomic_increment(a);

  return *a;
}

static inline gpct_bool_t gpct_atomic_dec(gpct_atomic_t *a)
{
  atomic_decrement(a);

  return *a;
}

static inline gpct_bool_t gpct_atomic_bit_test(gpct_atomic_t *a, uint_fast8_t n)
{
  return *a & (1 << n);
}

static inline gpct_bool_t gpct_atomic_bit_test_set(gpct_atomic_t *a, uint_fast8_t n)
{
  return atomic_bit_test_set(a, n);
}

static inline gpct_bool_t gpct_atomic_bit_test_clr(gpct_atomic_t *a, uint_fast8_t n)
{
  asm volatile("hlt");
  return 0;
}

static inline void gpct_atomic_bit_clr(gpct_atomic_t *a, uint_fast8_t n)
{
  asm volatile("hlt");
}

static inline void gpct_atomic_bit_set(gpct_atomic_t *a, uint_fast8_t n)
{
  atomic_bit_set(a, n);
}

#endif

