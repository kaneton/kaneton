#ifndef __TEMPLATE_LOCK_HEXO_H_
#define __TEMPLATE_LOCK_HEXO_H_

#include <hexo/lock.h>

#include <gpct/_platform.h>

/*************************************************************
 *	HEXO_SPIN mutex lock functions to use with container
 */

#define GPCT_LOCK_HEXO_SPIN_INITIALIZER	LOCK_INITIALIZER
#define gpct_lock_HEXO_SPIN_initializer	LOCK_INITIALIZER

typedef lock_t gpct_lock_HEXO_SPIN_type_t;

static inline gpct_error_t
gpct_lock_HEXO_SPIN_init(lock_t *lock)
{
  return lock_init(lock);
}

static inline void
gpct_lock_HEXO_SPIN_destroy(lock_t *lock)
{
  lock_destroy(lock);
}

static inline void
gpct_lock_HEXO_SPIN_wrlock(lock_t *lock)
{
  lock_spin(lock);
}

static inline void
gpct_lock_HEXO_SPIN_rdlock(lock_t *lock)
{
  lock_spin(lock);
}

static inline void
gpct_lock_HEXO_SPIN_unlock(lock_t *lock)
{
  lock_release(lock);
}

/*************************************************************
 *	HEXO_SPIN_IRQ mutex lock functions to use with container
 */

#define GPCT_LOCK_HEXO_SPIN_IRQ_INITIALIZER	LOCK_INITIALIZER
#define gpct_lock_HEXO_SPIN_IRQ_initializer	LOCK_INITIALIZER

typedef lock_t gpct_lock_HEXO_SPIN_IRQ_type_t;

static inline gpct_error_t
gpct_lock_HEXO_SPIN_IRQ_init(lock_t *lock)
{
  return lock_init(lock);
}

static inline void
gpct_lock_HEXO_SPIN_IRQ_destroy(lock_t *lock)
{
  lock_destroy(lock);
}

static inline void
gpct_lock_HEXO_SPIN_IRQ_wrlock(lock_t *lock)
{
  lock_spin_irq(lock);
}

static inline void
gpct_lock_HEXO_SPIN_IRQ_rdlock(lock_t *lock)
{
  lock_spin_irq(lock);
}

static inline void
gpct_lock_HEXO_SPIN_IRQ_unlock(lock_t *lock)
{
  lock_release_irq(lock);
}

#endif

