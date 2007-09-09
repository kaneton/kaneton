#ifndef LOCK_H_
#define LOCK_H_

#include <hexo/error.h>
#include <hexo/types.h>
#include <spinlock.h>

#define LOCK_INITIALIZER		0

typedef uint8_t	lock_t;

static inline error_t lock_init(lock_t *l)
{
  *l = 0;

  return (0);
}

static inline void lock_destroy(lock_t *l)
{

}

static inline void lock_spin(lock_t *l)
{
  IA32_SPIN_LOCK(*l);
}

static inline void lock_release(lock_t *l)
{
  IA32_SPIN_UNLOCK(*l);
}

static inline void lock_spin_irq(lock_t *l)
{
  IA32_SPIN_LOCK_IRQ(*l);
}

static inline void lock_release_irq(lock_t *l)
{
  IA32_SPIN_UNLOCK_IRQ(*l);
}

#endif
