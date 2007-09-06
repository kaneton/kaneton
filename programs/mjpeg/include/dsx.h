/*
 * This file is part of DSX, development environment for static
 * SoC applications.
 *
 * This file is distributed under the terms of the GNU General Public
 * License.
 *
 * Copyright (c) 2005, Nicolas Pouillon, <nipo@ssji.net>
 *     Laboratoire d'informatique de Paris 6 / ASIM, France
 *
 *  $Id: dsx.h 173 2006-03-27 19:35:08Z nipo $
 */
#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <libc.h>
#include <libkaneton.h>
#include <pthread.h>
#include "../spinlock.h"
#include <dsx_endianness.h>
#include "dsx_ext.h"

typedef struct _barrier_t *dsx_barrier_t;
struct _barrier_t {
  i_thread *thr;
  t_uint8 idx;
  t_uint8 c;
  t_uint8 _c;
};

typedef struct _mwmr_t *dsx_mwmr_t;
struct _mwmr_t {
  int width;
  int depth;
  int usage;
  int *begin;
  int *end;
  int *rptr;
  int *wptr;
  const char *name;
  void *fd;
  int offset;

  pthread_mutex_t	lock;
  pthread_cond_t	nfull;
  pthread_cond_t	nempty;
};

typedef struct _memspace_t* dsx_memspace_t;
struct _memspace_t {
    void *base;
    unsigned int size;
};

void dsx_memspace_alloc(dsx_memspace_t *memsp, size_t size);

#define DSX_MEMSPACE_ADDR(x) ((x)->base)
#define DSX_MEMSPACE_SIZE(x) ((x)->size)

typedef struct _lock_t *dsx_lock_t;
struct _lock_t
{
  t_uint8	v;
};

typedef void *dsx_signal_t;

void dsx_mwmr_alloc( dsx_mwmr_t fifo, unsigned int width, unsigned int depth, const char *name );

#define dsx_mwmr_reset(a) _dsx_mwmr_reset(a, __FILE__, __LINE__)
void _dsx_mwmr_reset( dsx_mwmr_t fifo, const char *file, const int line );
/* Bloquant */
#define dsx_mwmr_read(a, m, l) _dsx_mwmr_read(a, m, l, __FILE__, __LINE__)
void _dsx_mwmr_read( dsx_mwmr_t fifo, unsigned char *mem, unsigned int len, const char *file, const int line );
#define dsx_mwmr_write(a, m, l) _dsx_mwmr_write(a, m, l, __FILE__, __LINE__)
void _dsx_mwmr_write( dsx_mwmr_t fifo, unsigned char *mem, unsigned int len, const char *file, const int line );
/* Non bloquant */
#define dsx_mwmr_try_read(a, m, l) _dsx_mwmr_try_read(a, m, l, __FILE__, __LINE__)
void _dsx_mwmr_try_read( dsx_mwmr_t fifo, unsigned char *mem, unsigned int len, const char *file, const int line );
#define dsx_mwmr_try_write(a, m, l) _dsx_mwmr_try_write(a, m, l, __FILE__, __LINE__)
void _dsx_mwmr_try_write( dsx_mwmr_t fifo, unsigned char *mem, unsigned int len, const char *file, const int line );

void dsx_barrier_wait( dsx_barrier_t barrier );
void dsx_barrier_reset( dsx_barrier_t barrier );

void dsx_sig_send( dsx_signal_t sig );

void dsx_thread_reset( void );

static __attribute__((unused)) void	_dsx_log(int			level,
						 const char*		msg)
{
  //  printf("%s\n", msg);
}

static __attribute__((unused))void	_dsx_log_printf(int		level,
							const char*	fmt,
							...)
{
#if 0
  va_list		args;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
#endif
}

static __attribute__((unused)) void	dsx_putc(const char		c)
{
}

#define dsx_log( l, c ) /*_dsx_log( VERB_ ## l, c )*/
#define dsx_log_printf( l, c... ) /*_dsx_log_printf( VERB_ ## l, c )*/

#define dsx_assert(expr)                                           \
    do {                                                            \
        if ( ! (expr) ) {                                           \
            dsx_log_printf( NONE, "assertion (%s) failed on %s:%d !\n",  \
                             #expr, __FILE__, __LINE__ );           \
        }                                                           \
    } while(0)

enum __dsx_verbosity {
    VERB_NONE,
    VERB_TRACE,
    VERB_DEBUG,
    VERB_MAX,
};

#endif
