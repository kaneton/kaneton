#ifndef HEXO_SEMAPHORE_H
#define HEXO_SEMAPHORE_H

/* should not be used */

typedef int	sem_t;

static inline int sem_wait(sem_t *sem)
{
  asm volatile("hlt");
  return -1;
}

static inline int sem_post(sem_t *sem)
{
  asm volatile("hlt");
  return -1;
}

#endif
